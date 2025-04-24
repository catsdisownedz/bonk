// recieves input from clients and updates el game
// broadcatss el updates state to kol el clients
// handles password verification and player queues for mid roun djoining

#include "../../include/network/Server.h"
#include "../../include/network/web.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

Server::Server() : server(nullptr), running(false) {}

// Clean up server and deinitialize ENet library when done
Server::~Server()
{
    // Stop health check thread
    running = false;
    if (healthCheckThread.joinable())
    {
        healthCheckThread.join();
    }

    if (server)
        enet_host_destroy(server);
    enet_deinitialize();
}

bool Server::initialize()
{
    if (enet_initialize() != 0)
    {
        cerr << "ENet failed to initialize\n";
        return false;
    }

    // Get the PORT environment variable (for Cloud Run)
    const char *port_env = getenv("PORT");
    int port = port_env ? stoi(port_env) : 8080;

    address.host = ENET_HOST_ANY;
    address.port = port;

    // Server has a max of 32 clients, and 2 channels
    server = enet_host_create(&address, 32, 2, 0, 0);

    if (!server)
    {
        cerr << "Failed to create ENet server host\n";
        return false;
    }

    cout << "Server initialized on port " << port << "\n";

    // Start health check thread
    startHealthCheck();

    return true;
}

void Server::startHealthCheck()
{
    running = true;
    healthCheckThread = thread(&Server::runHealthCheck, this);
}

void Server::runHealthCheck()
{
    // Create a simple HTTP server for health checks
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Health check socket failed");
        return;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Health check setsockopt failed");
        close(server_fd);
        return;
    }

    // Get the PORT environment variable (for Cloud Run)
    // Health check should run on the same port as the game server for Cloud Run
    const char *port_env = getenv("PORT");
    int port = port_env ? stoi(port_env) : 8080;

    // Set up server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port); // Use the same port as the game server

    // Bind socket to address/port
    int bind_status = ::bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_status == -1)
    {
        perror("Health check bind failed");
        close(server_fd);
        return;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        perror("Health check listen failed");
        close(server_fd);
        return;
    }

    cout << "Health check and web interface server started on port " << port << endl;

    // Main health check loop
    while (running)
    {
        // Accept connection (non-blocking with timeout)
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(server_fd + 1, &readfds, NULL, NULL, &timeout);

        if (activity < 0)
        {
            if (errno != EINTR)
            {
                perror("Health check select error");
            }
            continue;
        }

        if (activity == 0 || !FD_ISSET(server_fd, &readfds))
        {
            // Timeout or no data, continue
            continue;
        }

        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Health check accept failed");
            continue;
        }

        // Buffer for HTTP request
        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, sizeof(buffer));

        // Simple parsing to check if it's a browser request or health check
        bool isHealthCheck = false;
        bool isBrowserRequest = false;

        if (valread > 0)
        {
            string request(buffer);
            // Check if it's a health check request
            if (request.find("GET /health") != string::npos)
            {
                isHealthCheck = true;
            }
            // Check if it's likely a browser request
            else if (request.find("GET /") != string::npos &&
                     (request.find("User-Agent") != string::npos ||
                      request.find("Accept: text/html") != string::npos))
            {
                isBrowserRequest = true;
            }
        }

        string response;
        if (isHealthCheck)
        {
            // Health check response
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\nOK";
        }
        else
        {
            // Assume it's a browser request or client testing connection
            string htmlContent = getWebInterface();
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
                       to_string(htmlContent.length()) + "\r\n\r\n" + htmlContent;
        }

        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }

    close(server_fd);
}

void Server::run()
{
    ENetEvent event;

    while (true)
    {
        while (enet_host_service(server, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                cout << "Client Connected!\n";
                clients.push_back(event.peer); // Add client to client list
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                cout << "Message Received: " << event.packet->data << "\n";
                broadcast(string((char *)event.packet->data)); // Broadcast message to client
                enet_packet_destroy(event.packet);             // Destroy packet after use
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                cout << "Client Disconnected\n";
                // Remove client from client list
                for (auto it = clients.begin(); it != clients.end(); ++it)
                {
                    if (*it == event.peer)
                    {
                        clients.erase(it);
                        break;
                    }
                }
                break;
            }
        }
    }
}

// Create packet out of message and send it to all clients
void Server::broadcast(const string &message)
{
    ENetPacket *packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);
}
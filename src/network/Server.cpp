// recieves input from clients and updates el game
// broadcatss el updates state to kol el clients
// handles password verification and player queues for mid roun djoining

#include "../../include/network/Server.h"
#include <iostream>

using namespace std;

Server::Server() : server(nullptr) {}

// Clean up server and deinitialize ENet library when done
Server::~Server()
{
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

    address.host = ENET_HOST_ANY;
    address.port = 8080;

    // Server has a max of 32 clients, and 2 channels
    server = enet_host_create(&address, 32, 2, 0, 0);

    if (!server)
    {
        cerr << "Failed to create ENet server host\n";
        return false;
    }

    cout << "Server initialized on port 1234\n";
    return true;
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
                enet_packet_destroy(event.packet); // Destroy packet after use
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                cout << "Client Disconnected\n";
                break;
            }
        }
    }
}

// Create packet out of message and send it to all clients
void Server::broadcast(const string& message) {
    ENetPacket *packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);
}
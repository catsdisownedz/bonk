// Server implementation using WebSocket++
// Handles game state updates and client connections

#include <network/Server.h>
#include <network/web.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <functional>
#include <atomic>

// Debug logging helper
#define LOG(msg) std::cout << "[" << std::chrono::system_clock::now().time_since_epoch().count() / 1000000 << "ms] " << msg << std::endl

Server::Server() : running(false)
{
    // Configure WebSocket server
    server.clear_access_channels(websocketpp::log::alevel::all);
    server.set_access_channels(websocketpp::log::alevel::app);
    // Enable error logs
    server.clear_error_channels(websocketpp::log::elevel::all);
    server.set_error_channels(websocketpp::log::elevel::fatal);

    // Initialize Asio transport
    server.init_asio();

    // Set WebSocket handlers
    server.set_open_handler(std::bind(&Server::on_open, this, std::placeholders::_1));
    server.set_close_handler(std::bind(&Server::on_close, this, std::placeholders::_1));
    server.set_message_handler(std::bind(&Server::on_message, this, std::placeholders::_1, std::placeholders::_2));

    // Set HTTP handler for health checks and web interface
    server.set_http_handler(std::bind(&Server::on_http, this, std::placeholders::_1));

    // Always accept WebSocket connections on any path
    server.set_validate_handler([](websocketpp::connection_hdl)
                                {
        LOG("WebSocket validation request received - accepting");
        return true; });

    LOG("Server constructor completed");
}

Server::~Server()
{
    LOG("Server shutting down...");

    // Stop the server
    if (running)
    {
        server.stop_listening();
        server.stop();
    }

    LOG("Server shutdown complete");
}

bool Server::initialize()
{
    LOG("Initializing server...");

    try
    {
        // Get the PORT environment variable (for Cloud Run)
        const char *port_env = getenv("PORT");
        int port = port_env ? std::stoi(port_env) : 8080;

        // Try up to 10 ports starting from the specified one
        bool bound = false;
        int max_attempts = 10;

        for (int attempt = 0; attempt < max_attempts; attempt++)
        {
            try
            {
                int current_port = port + attempt;
                // Set up the endpoint
                server.listen(current_port);
                LOG("Server initialized on port " << current_port);
                bound = true;
                break;
            }
            catch (const websocketpp::exception &e)
            {
                if (attempt < max_attempts - 1)
                {
                    LOG("Port " << (port + attempt) << " in use, trying next port...");
                }
                else
                {
                    throw;
                }
            }
        }

        if (!bound)
        {
            LOG("Could not bind to any port after " << max_attempts << " attempts");
            return false;
        }

        server.get_alog().write(websocketpp::log::alevel::app, "Server ready for connections");
        return true;
    }
    catch (const std::exception &e)
    {
        LOG("Error initializing server: " << e.what());
        return false;
    }
}

void Server::run()
{
    LOG("Server starting main loop");

    // Start the ASIO io_service run loop
    running = true;

    try
    {
        // Start the server accept loop
        server.start_accept();

        // Start the ASIO io_service run loop
        LOG("Entering main server loop");
        server.run();
    }
    catch (const std::exception &e)
    {
        LOG("Error in server run loop: " << e.what());
    }
}

void Server::on_open(connection_hdl hdl)
{
    {
        std::lock_guard<std::mutex> lock(connection_mutex);
        connections.insert(hdl);
    }

    LOG("Client connected - total connections: " << connections.size());
}

void Server::on_close(connection_hdl hdl)
{
    {
        std::lock_guard<std::mutex> lock(connection_mutex);
        connections.erase(hdl);
    }

    LOG("Client disconnected - remaining connections: " << connections.size());
}

void Server::on_message(connection_hdl hdl, WebSocketServer::message_ptr msg)
{
    // Get message payload
    std::string payload = msg->get_payload();
    LOG("Message received: " << payload);

    // Echo the message back to all clients (broadcast)
    broadcast(payload);
}

void Server::on_http(connection_hdl hdl)
{
    auto conn = server.get_con_from_hdl(hdl);
    std::string path = conn->get_resource();

    LOG("HTTP request received for: " << path);

    // Handle health check endpoint
    if (path == "/health")
    {
        LOG("Handling health check request");
        std::string health_json = getHealthCheckResponse();

        conn->set_status(websocketpp::http::status_code::ok);
        conn->set_body(health_json);
        conn->append_header("Content-Type", "application/json");

        LOG("Sent health check response");
    }
    // Handle WebSocket path for browser clients that don't auto-upgrade
    else if (path == "/ws")
    {
        LOG("WebSocket endpoint requested via HTTP");

        conn->set_status(websocketpp::http::status_code::ok);
        conn->set_body("This endpoint is for WebSocket connections only. Please use a WebSocket client.");
        conn->append_header("Content-Type", "text/plain");

        LOG("Sent WebSocket endpoint response");
    }
    // Handle root path for web interface
    else if (path == "/" || path == "/index.html")
    {
        LOG("Handling root request");
        std::string html_content = getWebInterface();

        conn->set_status(websocketpp::http::status_code::ok);
        conn->set_body(html_content);
        conn->append_header("Content-Type", "text/html");

        LOG("Sent HTML response");
    }
    // Default response for unknown paths
    else
    {
        LOG("Unrecognized request, sending generic OK");

        conn->set_status(websocketpp::http::status_code::ok);
        conn->set_body("OK");
        conn->append_header("Content-Type", "text/plain");

        LOG("Sent generic response");
    }
}

void Server::broadcast(const std::string &message)
{
    LOG("Broadcasting message to " << connections.size() << " client(s): " << message);

    std::lock_guard<std::mutex> lock(connection_mutex);

    for (auto it = connections.begin(); it != connections.end(); ++it)
    {
        try
        {
            server.send(*it, message, websocketpp::frame::opcode::text);
        }
        catch (const std::exception &e)
        {
            LOG("Error sending message: " << e.what());
        }
    }
}
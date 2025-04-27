// sends user input and authentication ll server, rrecives and applies el game state from el server
// manage ui updates like joining timers

#include "../../include/network/Client.h"
#include <iostream>
#include <chrono>
#include <functional>

#define LOG(msg) std::cout << "[" << std::chrono::system_clock::now().time_since_epoch().count() / 1000000 << "ms] " << msg << std::endl

Client::Client() : running(false)
{
    // Configure WebSocket client
    client.clear_access_channels(websocketpp::log::alevel::all);
    client.clear_error_channels(websocketpp::log::elevel::all);

    // Set WebSocket handlers
    client.set_open_handler(std::bind(&Client::on_open, this, std::placeholders::_1));
    client.set_close_handler(std::bind(&Client::on_close, this, std::placeholders::_1));
    client.set_message_handler(std::bind(&Client::on_message, this, std::placeholders::_1, std::placeholders::_2));
    client.set_fail_handler(std::bind(&Client::on_fail, this, std::placeholders::_1));

    // Initialize ASIO
    client.init_asio();
}

Client::~Client()
{
    disconnect();
}

bool Client::connectToServer(const std::string &uri)
{
    try
    {
        LOG("Connecting to " << uri);

        // Create a connection to the given URI
        websocketpp::lib::error_code ec;
        WebSocketClient::connection_ptr con = client.get_connection(uri, ec);

        if (ec)
        {
            LOG("Could not create connection: " << ec.message());
            return false;
        }

        // Store the connection for later use
        {
            std::lock_guard<std::mutex> lock(connection_mutex);
            connection = con->get_handle();
        }

        // Start the ASIO io_service run loop
        running = true;
        client.connect(con);

        // Start the client thread
        client_thread = std::thread([this]()
                                    {
            try {
                client.run();
            } catch (const std::exception& e) {
                LOG("Exception in client thread: " << e.what());
            } });

        return true;
    }
    catch (const std::exception &e)
    {
        LOG("Error in connect: " << e.what());
        return false;
    }
}

void Client::disconnect()
{
    if (!running)
    {
        return;
    }

    LOG("Disconnecting...");

    try
    {
        std::lock_guard<std::mutex> lock(connection_mutex);

        if (connection.lock())
        {
            client.close(connection, websocketpp::close::status::normal, "Closing connection");
        }

        running = false;

        // Stop the client's run loop and join the thread
        client.stop();

        if (client_thread.joinable())
        {
            client_thread.join();
        }

        LOG("Disconnected");
    }
    catch (const std::exception &e)
    {
        LOG("Error in disconnect: " << e.what());
    }
}

void Client::sendMessage(const std::string &message)
{
    try
    {
        std::lock_guard<std::mutex> lock(connection_mutex);

        if (!connection.lock())
        {
            LOG("Not connected");
            return;
        }

        client.send(connection, message, websocketpp::frame::opcode::text);
        LOG("Sent message: " << message);
    }
    catch (const std::exception &e)
    {
        LOG("Error sending message: " << e.what());
    }
}

void Client::setMessageHandler(std::function<void(const std::string &)> handler)
{
    message_handler = handler;
}

void Client::on_open(websocketpp::connection_hdl hdl)
{
    LOG("Connection opened");
}

void Client::on_close(websocketpp::connection_hdl hdl)
{
    LOG("Connection closed");
}

void Client::on_message(websocketpp::connection_hdl hdl, WebSocketClient::message_ptr msg)
{
    std::string payload = msg->get_payload();
    LOG("Received from server: " << payload);

    // Call the message handler if set
    if (message_handler)
    {
        message_handler(payload);
    }
}

void Client::on_fail(websocketpp::connection_hdl hdl)
{
    WebSocketClient::connection_ptr con = client.get_con_from_hdl(hdl);
    LOG("Connection failed: " << con->get_ec().message());
}
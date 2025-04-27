#pragma once

// Include Boost headers first
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/version.hpp>

// Include compatibility header
#include "websocketpp/websocketpp/compat.hpp"

// WebSocket++ headers
#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/client.hpp"

// Standard library
#include <string>
#include <functional>
#include <mutex>
#include <memory>
#include <thread>

typedef websocketpp::client<websocketpp::config::asio_client> WebSocketClient;

class Client
{
public:
    Client();
    ~Client(); // Destructor for clean up

    bool connectToServer(const std::string &uri);
    void sendMessage(const std::string &message);
    void disconnect();

    void setMessageHandler(std::function<void(const std::string &)> handler);

private:
    // WebSocket callbacks
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, WebSocketClient::message_ptr msg);
    void on_fail(websocketpp::connection_hdl hdl);

    WebSocketClient client;
    websocketpp::connection_hdl connection;
    std::mutex connection_mutex;
    std::thread client_thread;
    bool running;

    std::function<void(const std::string &)> message_handler;
};

#pragma once

// Include Boost headers first
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/version.hpp>

// Include compatibility header
#include "websocketpp/websocketpp/compat.hpp"

// WebSocket++ headers
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

// Standard library
#include <vector>
#include <thread>
#include <string>
#include <memory>
#include <functional>
#include <mutex>
#include <set>

using websocketpp::connection_hdl;
typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;
typedef std::set<connection_hdl, std::owner_less<connection_hdl>> ConnectionList;

class Server
{
public:
    Server();
    ~Server(); // Destructor for clean up
    bool initialize();
    void run();
    void broadcast(const std::string &message);

private:
    // WebSocket handlers
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, WebSocketServer::message_ptr msg);

    // HTTP handlers
    void on_http(connection_hdl hdl);

    WebSocketServer server;
    ConnectionList connections;
    std::mutex connection_mutex;
    bool running;
};

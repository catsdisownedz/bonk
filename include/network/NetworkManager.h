#pragma once

#include "Client.h"
#include "Server.h"
#include <memory>
#include <string>

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    // Server methods
    bool startServer(int port = 8080);
    void stopServer();
    bool isServerRunning() const;

    // Client methods
    bool connectToServer(const std::string &serverUri);
    void disconnectFromServer();
    bool isConnectedToServer() const;
    void sendMessage(const std::string &message);

    // Set message handler for client
    void setMessageHandler(std::function<void(const std::string &)> handler);

private:
    std::unique_ptr<Server> server;
    std::unique_ptr<Client> client;
    bool serverRunning;
    bool clientConnected;
};

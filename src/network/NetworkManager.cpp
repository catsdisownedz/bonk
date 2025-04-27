// delegates to server we client 7sb el game mode
// we hayhandle el suncing 3ala el network we el mfrood b2a y3ml authetication bl password we session management
// abstractor ml a5er

#include "../../include/network/NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager() : serverRunning(false), clientConnected(false) {}

NetworkManager::~NetworkManager()
{
    stopServer();
    disconnectFromServer();
}

bool NetworkManager::startServer(int port)
{
    try
    {
        if (serverRunning)
        {
            stopServer();
        }

        server = std::make_unique<Server>();
        if (!server->initialize())
        {
            std::cerr << "Failed to initialize server" << std::endl;
            return false;
        }

        // Start server in a separate thread
        std::thread([this]()
                    { 
                        try {
                            server->run();
                        } catch (const std::exception& e) {
                            std::cerr << "Error in server thread: " << e.what() << std::endl;
                        } })
            .detach();

        serverRunning = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error starting server: " << e.what() << std::endl;
        serverRunning = false;
        return false;
    }
}

void NetworkManager::stopServer()
{
    if (serverRunning && server)
    {
        server.reset();
        serverRunning = false;
    }
}

bool NetworkManager::isServerRunning() const
{
    return serverRunning;
}

bool NetworkManager::connectToServer(const std::string &serverUri)
{
    try
    {
        if (clientConnected)
        {
            disconnectFromServer();
        }

        client = std::make_unique<Client>();
        clientConnected = client->connectToServer(serverUri);
        return clientConnected;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error connecting to server: " << e.what() << std::endl;
        clientConnected = false;
        return false;
    }
}

void NetworkManager::disconnectFromServer()
{
    if (clientConnected && client)
    {
        client->disconnect();
        clientConnected = false;
    }
}

bool NetworkManager::isConnectedToServer() const
{
    return clientConnected;
}

void NetworkManager::sendMessage(const std::string &message)
{
    if (clientConnected && client)
    {
        client->sendMessage(message);
    }
}

void NetworkManager::setMessageHandler(std::function<void(const std::string &)> handler)
{
    if (client)
    {
        client->setMessageHandler(handler);
    }
}
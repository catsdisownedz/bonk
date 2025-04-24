#pragma once

#include <enet/enet.h>
#include <vector>

using namespace std;

class Server
{
public:
    Server();
    ~Server(); // Destructor for clean up
    bool initialize();
    void run();
    void broadcast(const string &message);

private:
    ENetAddress address;
    ENetHost *server;
    vector<ENetPeer *> clients;
};
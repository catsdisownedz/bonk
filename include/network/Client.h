#pragma once

#include <enet/enet.h>
#include <string>

using namespace std;

class Client {
    public:
        Client();
        ~Client(); // Destructor for clean up

        bool connectToServer(const string &ip, enet_uint16 port);
        void sendMessage(const string &message);
        void receiveLoop();

    private:
        ENetHost *client;
        ENetPeer *peer;
};
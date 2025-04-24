// sends user input and authentication ll server, rrecives and applies el game state from el server
// manage ui updates like joining timers

#include "../../include/network/Client.h"
#include <iostream>

using namespace std;

Client::Client() : client(nullptr), peer(nullptr) {}

Client::~Client()
{
    if (client)
        enet_host_destroy(client);
    enet_deinitialize();
}

bool Client::connectToServer(const string &ip, enet_uint16 port)
{
    if (enet_initialize() != 0)
    {
        cerr << "ENet initialization failed\n";
        return false;
    }

    // Create ENet client host
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client)
    {
        cerr << "Failed to create ENet client\n";
        return false;
    }

    ENetAddress address;

    // Set server IP and port
    enet_address_set_host(&address, ip.c_str());
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer)
    {
        cerr << "No available peers for initializing connection\n";
        return false;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        cout << "Connected to " << ip << ":" << port << "\n";
        return true;
    }
    else
    {
        cerr << "Connection to " << ip << "failed\n";
        enet_peer_reset(peer); // Reset peer
        return false;
    }
}

// Create packet from message and send it to server 
void Client::sendMessage(const string &message)
{
    ENetPacket *packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Client::receiveLoop()
{
    ENetEvent event;

    while (true)
    {
        while (enet_host_service(client, &event, 1000) > 0)
        {
            if (event.type == ENET_EVENT_TYPE_RECEIVE)
            {
                cout << "Received from server: " << event.packet->data << "\n";
                enet_packet_destroy(event.packet);
            }
        }
    }
}
#include "../include/network/NetworkManager.h"
#include <iostream>
#include <signal.h>
#include <atomic>
#include <thread>

// Flag to control server shutdown
std::atomic<bool> running(true);

// Network manager instance
NetworkManager *networkManager = nullptr;

// Signal handler to perform clean shutdown
void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", shutting down server..." << std::endl;
    running = false;
    if (networkManager)
    {
        delete networkManager;
        networkManager = nullptr;
    }
    exit(0);
}

int main()
{
    // Register signal handlers for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << "Starting Bonk.io WebSocket server..." << std::endl;

    // Get the PORT environment variable (for Cloud Run)
    const char *port_env = getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 8080;

    std::cout << "Using port: " << port << std::endl;

    networkManager = new NetworkManager();

    if (!networkManager->startServer(port))
    {
        std::cerr << "Failed to initialize server." << std::endl;
        delete networkManager;
        return 1;
    }

    std::cout << "Server initialized successfully on port " << port << std::endl;

    // Keep the main thread alive
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    delete networkManager;
    return 0;
}

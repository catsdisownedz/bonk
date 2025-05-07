#include <network/web.h>
#include <string>
#include <sstream>
#include <chrono>

// Returns HTML for the web interface
std::string getWebInterface()
{
    std::stringstream ss;

    ss << "<!DOCTYPE html>\n";
    ss << "<html lang=\"en\">\n";
    ss << "<head>\n";
    ss << "    <meta charset=\"UTF-8\">\n";
    ss << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ss << "    <title>Bonk Game Server</title>\n";
    ss << "    <style>\n";
    ss << "        body {\n";
    ss << "            font-family: Arial, sans-serif;\n";
    ss << "            line-height: 1.6;\n";
    ss << "            max-width: 800px;\n";
    ss << "            margin: 0 auto;\n";
    ss << "            padding: 20px;\n";
    ss << "            color: #333;\n";
    ss << "        }\n";
    ss << "        h1 {\n";
    ss << "            color: #007bff;\n";
    ss << "            border-bottom: 2px solid #eee;\n";
    ss << "            padding-bottom: 10px;\n";
    ss << "        }\n";
    ss << "        h2 {\n";
    ss << "            color: #0056b3;\n";
    ss << "            margin-top: 30px;\n";
    ss << "        }\n";
    ss << "        .info-box {\n";
    ss << "            background-color: #f8f9fa;\n";
    ss << "            border-left: 4px solid #007bff;\n";
    ss << "            padding: 15px;\n";
    ss << "            margin: 20px 0;\n";
    ss << "        }\n";
    ss << "        code {\n";
    ss << "            background-color: #f1f1f1;\n";
    ss << "            padding: 2px 5px;\n";
    ss << "            border-radius: 3px;\n";
    ss << "            font-family: Consolas, monospace;\n";
    ss << "        }\n";
    ss << "        #connect-btn {\n";
    ss << "            background-color: #007bff;\n";
    ss << "            color: white;\n";
    ss << "            border: none;\n";
    ss << "            padding: 10px 20px;\n";
    ss << "            border-radius: 5px;\n";
    ss << "            cursor: pointer;\n";
    ss << "            font-size: 16px;\n";
    ss << "        }\n";
    ss << "        #connect-btn:hover {\n";
    ss << "            background-color: #0056b3;\n";
    ss << "        }\n";
    ss << "        #status {\n";
    ss << "            font-weight: bold;\n";
    ss << "        }\n";
    ss << "        #chat {\n";
    ss << "            margin-top: 20px;\n";
    ss << "            border: 1px solid #ddd;\n";
    ss << "            padding: 10px;\n";
    ss << "            height: 200px;\n";
    ss << "            overflow-y: auto;\n";
    ss << "        }\n";
    ss << "        #message {\n";
    ss << "            width: 80%;\n";
    ss << "            padding: 8px;\n";
    ss << "        }\n";
    ss << "    </style>\n";
    ss << "</head>\n";
    ss << "<body>\n";
    ss << "    <h1>Bonk Game Server</h1>\n";
    ss << "    <p>This is a WebSocket-based recreation of the Bonk.io game.</p>\n";
    ss << "    \n";
    ss << "    <div class=\"info-box\">\n";
    ss << "        <strong>Status:</strong> Server is running and accepting connections!\n";
    ss << "        <p>WebSocket endpoint: <code>ws://[your-host]/ws</code></p>\n";
    ss << "        <p>Health check: <code>http://[your-host]/health</code></p>\n";
    ss << "    </div>\n";
    ss << "    \n";
    ss << "    <h2>WebSocket Test</h2>\n";
    ss << "    <p>Test the WebSocket connection below:</p>\n";
    ss << "    <button id=\"connect-btn\">Connect to WebSocket</button>\n";
    ss << "    <p>Status: <span id=\"status\">Disconnected</span></p>\n";
    ss << "    <div id=\"chat\"></div>\n";
    ss << "    <div style=\"margin-top: 10px;\">\n";
    ss << "        <input type=\"text\" id=\"message\" placeholder=\"Type a message\">\n";
    ss << "        <button id=\"send-btn\">Send</button>\n";
    ss << "    </div>\n";
    ss << "    \n";
    ss << "    <h2>What is Bonk.io?</h2>\n";
    ss << "    <p>Bonk.io is a multiplayer physics game where players control circular characters and attempt to push others off the edge of the level.</p>\n";
    ss << "    <p>Key features include:</p>\n";
    ss << "    <ul>\n";
    ss << "        <li>Multiplayer battles with up to 8 players at once</li>\n";
    ss << "        <li>Physics-based gameplay</li>\n";
    ss << "        <li>Last-man-standing and team-based matches</li>\n";
    ss << "    </ul>\n";
    ss << "    \n";
    ss << "    <h2>Technical Details</h2>\n";
    ss << "    <p>This server is built with:</p>\n";
    ss << "    <ul>\n";
    ss << "        <li>C++ for core game logic</li>\n";
    ss << "        <li>WebSocket++ for reliable WebSocket networking</li>\n";
    ss << "        <li>Deployed on Google Cloud Run</li>\n";
    ss << "    </ul>\n";
    ss << "    \n";
    ss << "    <p>Note: This is a simple recreation for educational purposes.</p>\n";
    ss << "    \n";
    ss << "    <footer style=\"margin-top: 50px; border-top: 1px solid #eee; padding-top: 20px; font-size: 0.9em;\">\n";
    ss << "        <p>Bonk Game Clone &copy; " << 2024 << " - A small-scale recreation for demonstration purposes.</p>\n";
    ss << "    </footer>\n";
    ss << "    \n";
    ss << "    <script>\n";
    ss << "        document.addEventListener('DOMContentLoaded', function() {\n";
    ss << "            const status = document.getElementById('status');\n";
    ss << "            const connectBtn = document.getElementById('connect-btn');\n";
    ss << "            const sendBtn = document.getElementById('send-btn');\n";
    ss << "            const messageInput = document.getElementById('message');\n";
    ss << "            const chatDiv = document.getElementById('chat');\n";
    ss << "            let socket;\n";
    ss << "            \n";
    ss << "            connectBtn.addEventListener('click', function() {\n";
    ss << "                if (socket && socket.readyState === WebSocket.OPEN) {\n";
    ss << "                    socket.close();\n";
    ss << "                    return;\n";
    ss << "                }\n";
    ss << "                \n";
    ss << "                // Get the host from the current URL\n";
    ss << "                const wsProtocol = location.protocol === 'https:' ? 'wss://' : 'ws://';\n";
    ss << "                const wsUrl = wsProtocol + location.host + '/ws';\n";
    ss << "                \n";
    ss << "                status.textContent = 'Connecting...';\n";
    ss << "                socket = new WebSocket(wsUrl);\n";
    ss << "                \n";
    ss << "                socket.onopen = function() {\n";
    ss << "                    status.textContent = 'Connected';\n";
    ss << "                    connectBtn.textContent = 'Disconnect';\n";
    ss << "                    addChatMessage('System', 'Connected to server');\n";
    ss << "                };\n";
    ss << "                \n";
    ss << "                socket.onmessage = function(event) {\n";
    ss << "                    addChatMessage('Server', event.data);\n";
    ss << "                };\n";
    ss << "                \n";
    ss << "                socket.onclose = function() {\n";
    ss << "                    status.textContent = 'Disconnected';\n";
    ss << "                    connectBtn.textContent = 'Connect to WebSocket';\n";
    ss << "                    addChatMessage('System', 'Disconnected from server');\n";
    ss << "                };\n";
    ss << "                \n";
    ss << "                socket.onerror = function(error) {\n";
    ss << "                    status.textContent = 'Error';\n";
    ss << "                    addChatMessage('System', 'Error: ' + error);\n";
    ss << "                };\n";
    ss << "            });\n";
    ss << "            \n";
    ss << "            sendBtn.addEventListener('click', sendMessage);\n";
    ss << "            messageInput.addEventListener('keypress', function(e) {\n";
    ss << "                if (e.key === 'Enter') sendMessage();\n";
    ss << "            });\n";
    ss << "            \n";
    ss << "            function sendMessage() {\n";
    ss << "                if (!socket || socket.readyState !== WebSocket.OPEN) {\n";
    ss << "                    addChatMessage('System', 'Not connected to server');\n";
    ss << "                    return;\n";
    ss << "                }\n";
    ss << "                \n";
    ss << "                const message = messageInput.value.trim();\n";
    ss << "                if (message) {\n";
    ss << "                    socket.send(message);\n";
    ss << "                    addChatMessage('You', message);\n";
    ss << "                    messageInput.value = '';\n";
    ss << "                }\n";
    ss << "            }\n";
    ss << "            \n";
    ss << "            function addChatMessage(sender, message) {\n";
    ss << "                const msgDiv = document.createElement('div');\n";
    ss << "                msgDiv.innerHTML = `<strong>${sender}:</strong> ${message}`;\n";
    ss << "                chatDiv.appendChild(msgDiv);\n";
    ss << "                chatDiv.scrollTop = chatDiv.scrollHeight;\n";
    ss << "            }\n";
    ss << "        });\n";
    ss << "    </script>\n";
    ss << "</body>\n";
    ss << "</html>\n";

    return ss.str();
}

// Returns a simple health check response for Cloud Run health checks
std::string getHealthCheckResponse()
{
    std::stringstream ss;

    ss << "{\n";
    ss << "  \"status\": \"healthy\",\n";
    ss << "  \"message\": \"Bonk game server is running with WebSocket++\",\n";
    ss << "  \"timestamp\": " << std::chrono::system_clock::now().time_since_epoch().count() / 1000000 << "\n";
    ss << "}\n";

    return ss.str();
}

// Note: This function is not currently used, but kept as a reference
// The main Server class handles HTTP connections directly
/*
void startHttpServer(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        return;
    }

    // Setup socket code would go here

    while(true) {
        // Accept client connection
        int client_socket = -1;
        std::string request = "";

        // Handle HTTP requests
        if (request.find("GET /health") != std::string::npos) {
            std::string response = "HTTP/1.1 200 OK\r\n\r\n";
            send(client_socket, response.c_str(), response.size(), 0);
        }

        if (client_socket >= 0) {
            close(client_socket);
        }
    }
}
*/
#include <string>
#include <sstream>

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
    ss << "    </style>\n";
    ss << "</head>\n";
    ss << "<body>\n";
    ss << "    <h1>Bonk Game Server</h1>\n";
    ss << "    <p>This is a small-scale recreation of the Bonk.io game, built with C++ and ENet for networking.</p>\n";
    ss << "    \n";
    ss << "    <div class=\"info-box\">\n";
    ss << "        <strong>Status:</strong> Server is running and accepting connections!\n";
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
    ss << "    <h2>How to Connect</h2>\n";
    ss << "    <p>To connect to this game server:</p>\n";
    ss << "    <ol>\n";
    ss << "        <li>Download the Bonk game client</li>\n";
    ss << "        <li>In the client, enter the server address (this URL) in the connection settings</li>\n";
    ss << "        <li>Click Connect and enjoy the game!</li>\n";
    ss << "    </ol>\n";
    ss << "    \n";
    ss << "    <h2>Technical Details</h2>\n";
    ss << "    <p>This server is built with:</p>\n";
    ss << "    <ul>\n";
    ss << "        <li>C++ for core game logic</li>\n";
    ss << "        <li>ENet library for reliable UDP networking</li>\n";
    ss << "        <li>Deployed on Google Cloud Run</li>\n";
    ss << "    </ul>\n";
    ss << "    \n";
    ss << "    <p>Note: This is a simple recreation for educational purposes.</p>\n";
    ss << "    \n";
    ss << "    <footer style=\"margin-top: 50px; border-top: 1px solid #eee; padding-top: 20px; font-size: 0.9em;\">\n";
    ss << "        <p>Bonk Game Clone &copy; " << 2024 << " - A small-scale recreation for demonstration purposes.</p>\n";
    ss << "    </footer>\n";
    ss << "</body>\n";
    ss << "</html>\n";

    return ss.str();
}
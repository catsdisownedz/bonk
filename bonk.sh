#!/bin/bash
set -e

echo "=== Bonk WebSocket Server - All-in-One Build & Run Script ==="
echo ""

# Function to kill any running server processes
kill_servers() {
  echo "🔍 Looking for running server processes..."
  local SERVER_PIDS=$(pgrep bonk_server || echo "")
  
  if [ -z "$SERVER_PIDS" ]; then
    echo "✅ No running server processes found."
  else
    echo "🔴 Found running server processes. Killing them..."
    for pid in $SERVER_PIDS; do
      echo "   Killing process $pid..."
      kill -9 $pid 2>/dev/null || true
    done
    echo "✅ All server processes killed."
  fi
}

# Function to set up WebSocket++
setup_websocketpp() {
  echo "🔧 Setting up WebSocket++ library..."
  mkdir -p include/network/websocketpp
  
  if [ -d "/tmp/websocketpp_src" ]; then
    echo "   Using existing WebSocket++ source in /tmp/websocketpp_src"
  else
    echo "   Cloning WebSocket++ repository..."
    git clone https://github.com/zaphoyd/websocketpp.git /tmp/websocketpp_src
  fi
  
  # Copy WebSocket++ files
  echo "   Copying WebSocket++ files to local directory..."
  cp -r /tmp/websocketpp_src/* include/network/websocketpp/
  
  # Apply compatibility fixes with sed
  echo "   Applying compatibility fixes..."
  find include/network/websocketpp -type f -name "*.hpp" -exec sed -i.bak 's/io_service/io_context/g' {} \;
  find include/network/websocketpp -type f -name "*.hpp" -exec sed -i.bak 's/\([a-zA-Z0-9_]*\)->expires_from_now()/\1->expires_after()/g' {} \;
  find include/network/websocketpp -type f -name "*.hpp" -exec sed -i.bak 's/m_strand.reset(new lib::asio::io_context::strand(\*io_context));/m_strand.reset(new lib::asio::strand<lib::asio::io_context::executor_type>(io_context->get_executor()));/g' {} \;
  
  # Create compatibility header
  mkdir -p include/network/websocketpp/websocketpp
  cat > include/network/websocketpp/websocketpp/compat.hpp << 'EOL'
#ifndef WEBSOCKETPP_COMPAT_HPP
#define WEBSOCKETPP_COMPAT_HPP

// Compatibility header for modern Boost versions (1.70+)
#include <boost/version.hpp>
#include <boost/asio.hpp>

#if BOOST_VERSION >= 107000
    #include <boost/asio/strand.hpp>
    #include <boost/asio/post.hpp>
    #include <boost/asio/executor.hpp>
    
    // Create compatibility layer for modern Boost
    namespace websocketpp {
        namespace lib {
            namespace asio {
                template <typename Executor>
                using strand = boost::asio::strand<Executor>;
                
                using boost::asio::post;
            }
        }
    }
#endif

#endif // WEBSOCKETPP_COMPAT_HPP
EOL
  
  # Clean up backups
  find include/network/websocketpp -name "*.bak" -delete
  
  # Update Client.h and Server.h
  cat > include/network/Client.h << 'EOL'
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
EOL

  cat > include/network/Server.h << 'EOL'
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
EOL

  echo "✅ WebSocket++ setup complete!"
}

# Function to create and configure CMakeLists.txt
create_cmake_file() {
  echo "📝 Creating CMakeLists.txt..."
  cat > CMakeLists.txt << 'EOL'
cmake_minimum_required(VERSION 3.10)
project(BonkServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Silence deprecated OpenGL warnings on macOS
if(APPLE)
  add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Find Boost
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

# Find OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Include directories
include_directories(include)

# Add WebSocket++ (our local version)
include_directories(include/network)

# Debug message
message(STATUS "Source files directory: ${CMAKE_SOURCE_DIR}/src")

# All source files
add_executable(bonk_server
  src/main.cpp
  src/network/Server.cpp
  src/network/Client.cpp
  src/network/NetworkManager.cpp
  src/network/web.cpp
  src/physics/Player.cpp
  src/physics/Platform.cpp
  src/physics/GameObject.cpp
  src/physics/PhysicsEngine.cpp
  src/core/InputManager.cpp
  src/glad.c
)

# Link libraries
target_link_libraries(bonk_server
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${OPENGL_LIBRARIES}
  pthread
)

if(APPLE)
  find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
  find_library(CORE_SERVICES_FRAMEWORK CoreServices)
  target_link_libraries(bonk_server ${CORE_FOUNDATION_FRAMEWORK} ${CORE_SERVICES_FRAMEWORK})
endif()
EOL
  echo "✅ CMakeLists.txt created!"
}

# Function to build the server
build_server() {
  echo "🔨 Building bonk_server..."
  mkdir -p build
  cd build
  
  # Run CMake with better error handling
  cmake .. || {
    echo "❌ CMake configuration failed!"
    echo "Check the error messages above for details."
    exit 1
  }
  
  # Build using make with parallel jobs
  make -j$(nproc 2>/dev/null || echo 4) || {
    echo "❌ Build failed!"
    echo "Check the error messages above for details."
    exit 1
  }
  
  cd ..
  echo "✅ bonk_server successfully built!"
}

# Function to run the server
run_server() {
  local PORT=${1:-8080}
  echo "🚀 Starting bonk_server on port $PORT..."
  
  # Check if the executable exists
  if [ ! -f "build/bonk_server" ]; then
    echo "❌ Server executable not found!"
    exit 1
  fi
  
  # Export the port environment variable
  export PORT=$PORT
  
  # Run the server in the foreground
  echo "📢 Server is starting. Press Ctrl+C to stop."
  echo "📢 Endpoints: http://localhost:$PORT (web interface), ws://localhost:$PORT/ws (WebSocket)"
  build/bonk_server
}

# Parse command line arguments
MODE="all"
PORT=8080
CLEAN=false

for arg in "$@"; do
  case $arg in
    --help|-h)
      echo "Usage: ./bonk.sh [OPTIONS]"
      echo ""
      echo "Options:"
      echo "  --help, -h       Show this help message"
      echo "  --clean          Clean build before rebuilding"
      echo "  --build-only     Only build the server, don't run it"
      echo "  --run-only       Run the server without rebuilding"
      echo "  --port=PORT      Specify the port (default: 8080)"
      exit 0
      ;;
    --clean)
      CLEAN=true
      ;;
    --build-only)
      MODE="build"
      ;;
    --run-only)
      MODE="run"
      ;;
    --port=*)
      PORT="${arg#*=}"
      ;;
  esac
done

# Main execution
if [ "$CLEAN" = true ]; then
  echo "🧹 Cleaning previous build artifacts..."
  rm -rf build
fi

# Kill any running servers
kill_servers

# Run the selected mode
if [ "$MODE" = "all" ] || [ "$MODE" = "build" ]; then
  setup_websocketpp
  create_cmake_file
  build_server
fi

if [ "$MODE" = "all" ] || [ "$MODE" = "run" ]; then
  run_server $PORT
fi

exit 0 
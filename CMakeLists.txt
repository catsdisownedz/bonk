cmake_minimum_required(VERSION 3.10)
project(BonkServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Silence deprecated OpenGL warnings on macOS
if(APPLE)
  add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Find Boost
cmake_policy(SET CMP0074 NEW)
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

# Find OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Find OpenGL
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
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

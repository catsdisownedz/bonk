cmake_minimum_required(VERSION 3.10)
project(bonk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ENet package
find_package(PkgConfig REQUIRED)
pkg_check_modules(ENET REQUIRED libenet)

include_directories(${ENET_INCLUDE_DIRS})
link_directories(${ENET_LIBRARY_DIRS})

# Include directories
include_directories(include)

# Server executable
add_executable(server 
    src/main.cpp
    src/network/Server.cpp
    src/network/web.cpp
)

target_link_libraries(server ${ENET_LIBRARIES})

# Client executable
add_executable(client
    src/network/Client.cpp
)

target_link_libraries(client ${ENET_LIBRARIES})

# Installation
install(TARGETS server client
    RUNTIME DESTINATION bin
) 
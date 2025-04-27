cmake_minimum_required(VERSION 3.15)
project(BonkServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependency Management ---
# Must come FIRST before any target operations
find_package(Boost 1.71 REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)
# Add GLUT detection for Linux/Windows ▼
if(NOT APPLE)
  find_package(GLUT REQUIRED)
endif()

# --- Executable Setup ---
file(GLOB_RECURSE SOURCE_FILES 
  src/*.cpp 
  src/*.c
  src/network/*.cpp
  src/physics/*.cpp
  src/core/*.cpp
  src/glad.c
)

add_executable(bonk_server ${SOURCE_FILES})  # <-- TARGET CREATED HERE

# --- Include Directories ---
target_include_directories(bonk_server PRIVATE
  include
  include/network
  /usr/include
  /usr/include/GL
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIRS}
)

# --- Platform-specific Linking ---
if(APPLE)
  # macOS frameworks
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(GLUT_LIBRARY GLUT)
  
  target_link_libraries(bonk_server PRIVATE
    ${COCOA_LIBRARY}
    ${IOKIT_LIBRARY}
    ${OPENGL_LIBRARY}
    ${GLUT_LIBRARY}
  )
elseif(UNIX)
  # Linux/Unix
  target_link_libraries(bonk_server PRIVATE
    OpenGL::OpenGL
    GLUT::GLUT
    X11
    dl
  )
endif()

# --- Common Linking ---
target_link_libraries(bonk_server PRIVATE
  Boost::system
  Boost::thread
  OpenSSL::SSL
  OpenSSL::Crypto
  pthread
)

# --- Glad Handling ---
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")
  message(WARNING "glad.c not found! Run: ...")
endif()
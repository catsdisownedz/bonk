cmake_minimum_required(VERSION 3.15)
project(BonkServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific configuration
if(APPLE)
    # macOS requires OpenGL framework
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(GLUT_LIBRARY GLUT)
    target_link_libraries(bonk_server PRIVATE
        ${OPENGL_LIBRARY}
        ${GLUT_LIBRARY}
    )
else()
    # Linux/Windows use the standard OpenGL target
    target_link_libraries(bonk_server PRIVATE
        OpenGL::OpenGL
    )
endif()

# --- Dependency Management ---
# Modern Boost configuration
find_package(Boost 1.71 REQUIRED COMPONENTS system thread)

# OpenSSL configuration
find_package(OpenSSL REQUIRED)

# Modern OpenGL configuration
find_package(OpenGL REQUIRED)

# --- Glad Handling ---
# Check for glad.c existence
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")
  message(WARNING "glad.c not found! To generate it, run:
    pip install glad
    glad --profile core --api gl=4.3 --out-path src/ --generator c
  ")
else()
  message(STATUS "Found glad.c in source directory")
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

add_executable(bonk_server ${SOURCE_FILES})

# --- Modern Include Directories ---
target_include_directories(bonk_server PRIVATE
  include
  include/network
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIRS}
)

# --- Modern Linking ---
target_link_libraries(bonk_server PRIVATE
  Boost::system
  Boost::thread
  OpenSSL::SSL
  OpenSSL::Crypto
  OpenGL::OpenGL
  pthread
)

# --- Platform-specific Linking ---
if(APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  target_link_libraries(bonk_server PRIVATE
    ${COCOA_LIBRARY}
    ${IOKIT_LIBRARY}
  )
elseif(UNIX)
  target_link_libraries(bonk_server PRIVATE
    X11
    dl
  )
endif()

# --- File Validation ---
if(NOT SOURCE_FILES)
  message(FATAL_ERROR "No source files found! Check your file paths.")
endif()
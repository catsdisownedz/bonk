#!/bin/bash

# Detect OS
OS_TYPE=$(uname)
WINDOWS=false
LINUX=false
MAC=false

if [[ "$OS_TYPE" == "Linux" ]]; then
    LINUX=true
    echo "[OS Detected] Linux"
elif [[ "$OS_TYPE" == "Darwin" ]]; then
    MAC=true
    echo "[OS Detected] macOS"
elif [[ "$OS_TYPE" =~ "MINGW" || "$OS_TYPE" =~ "MSYS" || "$OS_TYPE" =~ "CYGWIN" ]]; then
    WINDOWS=true
    echo "[OS Detected] Windows (Git Bash)"
else
    echo "Unsupported OS: $OS_TYPE"
    exit 1
fi

echo "[INFO] Building for $OS_TYPE..."

# Create build output dir if not exists
mkdir -p build/output

# Common source paths
MENU_SRC="src/ui/MenuManager.cpp src/core/SoundPlayer.cpp src/core/ImageLoader.cpp"
GAME_SRC="src/ui/maps/main.cpp src/ui/maps/OneVsOne.cpp src/ui/maps/GangGrounds.cpp \
          src/physics/GameObject.cpp src/physics/Player.cpp src/physics/PhysicsEngine.cpp \
          src/physics/Platform.cpp src/physics/Bouncy.cpp \
          src/core/InputManager.cpp src/core/Renderer.cpp"

INCLUDE_FLAGS="-Isrc/include -Iexternal -Iexternal/openal-soft/include"

# Compile for the detected OS
if $WINDOWS; then
    echo "[BUILD] Compiling menu for Windows..."
    g++ $MENU_SRC $INCLUDE_FLAGS -Lexternal/openal-soft/lib -o build/output/menu.exe -lOpenAL32 -lfreeglut -lopengl32 -lglu32

    echo "[BUILD] Compiling game for Windows..."
    g++ $GAME_SRC $INCLUDE_FLAGS -Lexternal/openal-soft/lib -o build/output/opengl_glut.exe -lOpenAL32 -lfreeglut -lopengl32 -lglu32

elif $LINUX; then
    echo "[BUILD] Compiling menu for Linux..."
    g++ $MENU_SRC $INCLUDE_FLAGS -o build/output/menu -lopenal -lGL -lGLU -lglut

    echo "[BUILD] Compiling game for Linux..."
    g++ $GAME_SRC $INCLUDE_FLAGS -o build/output/opengl_glut -lopenal -lGL -lGLU -lglut

elif $MAC; then
    echo "[BUILD] Compiling menu for macOS..."
    g++ $MENU_SRC $INCLUDE_FLAGS -o build/output/menu -framework OpenAL -framework OpenGL -framework GLUT

    echo "[BUILD] Compiling game for macOS..."
    g++ $GAME_SRC $INCLUDE_FLAGS -o build/output/opengl_glut -framework OpenAL -framework OpenGL -framework GLUT
fi

echo "[SUCCESS] Build complete ✅"

# Ask to run the menu
read -p "Do you want to run the Menu Manager now? (y/n): " choice
case "$choice" in
    y|Y )
        echo "[INFO] Running Menu Manager..."
        cd "$(dirname "$0")"
        if $WINDOWS; then
            ./build/output/menu.exe
        else
            echo "[DEBUG] Checking if menu exists: $(ls -l ./build/output/menu)"
            ./build/output/menu  # This is the Linux version of the executable

    # If you need to run the game, also update to:
             ./build/output/opengl_glut  # Without .exe extension
        fi
        ;;
    * )
        echo "[INFO] Menu Manager not launched."
        ;;
esac


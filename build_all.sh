#!/usr/bin/env bash

# Detect OS
OS_TYPE=$(uname -s)
WINDOWS=false; LINUX=false; MAC=false

case "$OS_TYPE" in
  Linux*)            LINUX=true   ;;
  Darwin*)           MAC=true     ;;
  MINGW*|MSYS*|CYGWIN*) WINDOWS=true ;;
  *) echo "Unsupported OS: $OS_TYPE"; exit 1 ;;
esac

echo "[INFO] Building for $OS_TYPE…"

# Create output dir
mkdir -p build/output

# All sources for the single bonk binary
SOURCES=(
  # entrypoint + scene coordinator
  src/ui/Game.cpp

  # menu manager
  src/ui/MenuManager.cpp
  src/ui/GameOver.cpp


  # actual maps (no mains)
  src/ui/maps/OneVsOne.cpp
  src/ui/maps/GangGrounds.cpp
  src/ui/maps/GravityOff.cpp

  # core systems
  src/core/SoundPlayer.cpp
  src/core/InputManager.cpp
  src/core/Renderer.cpp
  src/core/ImageLoader.cpp   # your stb_image wrapper

  # physics
  src/physics/Platform.cpp
  src/physics/Bouncy.cpp
  src/physics/Player.cpp
  src/physics/PhysicsEngine.cpp
  src/physics/GameObject.cpp
)

# Point <…> includes at these roots:
INCLUDE_FLAGS="-Iinclude -Iexternal -Iexternal/openal-soft/include"

# Per‐OS linker flags and output name
if $WINDOWS; then
  LIBS="-Lexternal/openal-soft/lib -lOpenAL32 -lfreeglut -lopengl32 -lglu32"
  OUT=build/output/bonk.exe
elif $LINUX; then
  LIBS="-lopenal -lGL -lGLU -lglut"
  OUT=build/output/bonk
elif $MAC; then
  LIBS="-framework OpenAL -framework OpenGL -framework GLUT"
  OUT=build/output/bonk
  EXTRA_FLAGS="-Wno-deprecated-declarations"
fi

echo "[BUILD] Compiling into $OUT"
g++ -std=c++17 "${SOURCES[@]}" $INCLUDE_FLAGS $LIBS $EXTRA_FLAGS -o "$OUT"
if [ $? -ne 0 ]; then
  echo "[ERROR] Build failed."
  exit 1
fi

echo "[SUCCESS] Build complete ✅"

# Optionally run
read -p "Run Bonk now? (y/N) " run
if [[ "$run" =~ ^[Yy]$ ]]; then
  if $WINDOWS; then
    ./build/output/bonk.exe
  else
    ./build/output/bonk
  fi
fi

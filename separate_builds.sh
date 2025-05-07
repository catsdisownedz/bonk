#!/usr/bin/env bash
set -Eeuo pipefail

# ─── Platform Detection ────────────────────────────────────────────────────────
OS_TYPE=$(uname -s)
WINDOWS=false; LINUX=false; MAC=false
case "$OS_TYPE" in
  Linux*)            LINUX=true   ;;
  Darwin*)           MAC=true     ;;
  MINGW*|MSYS*|CYGWIN*) WINDOWS=true ;;
  *) echo "Unsupported OS: $OS_TYPE"; exit 1 ;;
esac
# treat WSL as Windows
if grep -qEi "(Microsoft|WSL)" /proc/version 2>/dev/null; then
  WINDOWS=true; LINUX=false; MAC=false
fi

echo "[INFO] Building for $OS_TYPE…"
mkdir -p build/output

# ─── Common Sources ───────────────────────────────────────────────────────────
MAIN_SRC="src/ui/maps/main.cpp"

CORE_SRCS=(
  src/core/SoundPlayer.cpp
  src/core/InputManager.cpp
  src/core/Renderer.cpp
  src/core/ImageLoader.cpp
)

PHYS_SRCS=(
  src/physics/Platform.cpp
  src/physics/Bouncy.cpp
  src/physics/Player.cpp
  src/physics/PhysicsEngine.cpp
  src/physics/GameObject.cpp
)

# ─── Include Paths ────────────────────────────────────────────────────────────
INCLUDE_FLAGS="-Iinclude -Iexternal -Iexternal/openal-soft/include"

# ─── Linker Flags ─────────────────────────────────────────────────────────────
if $WINDOWS; then
  LIBS="-Lexternal/openal-soft/lib \
        -lOpenAL32 -lfreeglut -lopengl32 -lglu32 \
        -lws2_32 -lgdi32 -lwinmm"
  EXTRA_LDFLAGS="-mconsole"
elif $LINUX; then
  LIBS="-lopenal -lGL -lGLU -lglut -lboost_system"
  EXTRA_LDFLAGS=""
else
  LIBS="-framework OpenAL -framework OpenGL -framework GLUT"
  EXTRA_LDFLAGS=""
fi

# ─── Build Function ──────────────────────────────────────────────────────────
build_demo(){
  DEMO="$1"                              # e.g. OneVsOne or GangGrounds
  MAP_SRC="src/ui/maps/${DEMO}.cpp"
  OUT="build/output/${DEMO}${WINDOWS:+.exe}"
  
  echo "[BUILD] $DEMO → $OUT"
  g++ -std=c++17 \
      "$MAIN_SRC" \
      "$MAP_SRC" \
      "${CORE_SRCS[@]}" \
      "${PHYS_SRCS[@]}" \
      $INCLUDE_FLAGS \
      $LIBS \
      $EXTRA_LDFLAGS \
      -o "$OUT"
  
  echo "[SUCCESS] $DEMO built at $OUT"
}

# ─── Fire Off Two Builds ─────────────────────────────────────────────────────
build_demo OneVsOne
build_demo GangGrounds

echo "[ALL DONE] Binaries in build/output/"
ls -1 build/output/

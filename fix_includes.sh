#!/usr/bin/env bash
set -Eeuo pipefail

echo "[INFO] Rewriting #include paths…"

# tweak these roots if you have other code dirs
find include src external \
  -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) \
  -print0 \
| xargs -0 sed -E -i \
    -e 's|#include[[:space:]]*"(\.\./)+include/([^"]+)"|#include <\2>|g' \
    -e 's|#include[[:space:]]*"(\.\./)+([^/"]+/[^"]+)"|#include <\2>|g'

echo "[DONE] All includes are now angle‐bracketed and point at your include/ tree."

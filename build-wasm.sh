#!/bin/bash

# Battle City C++ Build Script
# This script builds the WASM module using Emscripten

set -e

echo "🔨 Building Battle City Engine (C++ → WASM)..."

# Check if Emscripten is available
if ! command -v emcc &> /dev/null; then
  echo "❌ Emscripten not found. Please install it:"
  echo "   https://emscripten.org/docs/getting_started/downloads.html"
  exit 1
fi

# Create build directory
mkdir -p build/wasm

# Compile C++ to WASM
echo "📦 Compiling C++ to WebAssembly..."

emcc \
  -I./include \
  ./cpp/geometry.cpp \
  ./cpp/collision.cpp \
  ./cpp/bullet-collision.cpp \
  ./cpp/random.cpp \
  ./cpp/bindings.cpp \
  -o build/wasm/battle-engine.js \
  -s WASM=1 \
  -s EXPORTED_FUNCTIONS="['_malloc','_free','_engine_init','_geom_test_rect_collide','_geom_distance','_geom_distance_sq','_bullet_calculate_hit_time','_random_init','_random_next','_random_next_int']" \
  -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap','UTF8ToString','stringToUTF8']" \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s TOTAL_MEMORY=33554432 \
  -O3 \
  --bind

echo "✅ Build complete!"
echo "📁 Output:"
echo "   - build/wasm/battle-engine.js"
echo "   - build/wasm/battle-engine.wasm"
echo ""
echo "📖 Next: Import battle-engine.js in your TypeScript code"

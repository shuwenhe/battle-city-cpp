cmake_minimum_required(VERSION 3.12)
project(battle-city-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# 构建 C++ 服务器
# ============================================================================

add_subdirectory(../apps/backend ../apps/backend/build)

# ============================================================================
# Emscripten WebAssembly 构建（可选）
# ============================================================================

if(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_SDL=2")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
  
  # WASM 绑定目标
  add_executable(battle-engine-wasm
    cpp/src/wasm/bindings.cpp
  )
  
  target_link_libraries(battle-engine-wasm PRIVATE game-engine)
  
  target_include_directories(battle-engine-wasm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/../external
  )
  
  set(EMSCRIPTEN_FLAGS
    -s EXPORTED_FUNCTIONS="['_malloc','_free']"
    -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap','UTF8ToString','stringToUTF8']"
    -s WASM=1
    -s ALLOW_MEMORY_GROWTH=1
    -O3
  )
  
  set_target_properties(battle-engine-wasm PROPERTIES
    LINK_FLAGS "${EMSCRIPTEN_FLAGS}"
    SUFFIX ".js"
  )
endif()

# ============================================================================
# 单元测试（可选）
# ============================================================================

enable_testing()
# add_subdirectory(cpp/test)  # 取消注释以启用单元测试

cmake_minimum_required(VERSION 3.12)
project(battle-city-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten-specific settings
if(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_SDL=2")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
endif()

# Source files
set(SOURCES
  cpp/geometry.cpp
  cpp/collision.cpp
  cpp/bullet-collision.cpp
  cpp/random.cpp
)

set(HEADERS
  include/geometry.h
  include/collision.h
  include/bullet-collision.h
  include/random.h
  include/types.h
)

# Create library
add_library(battle-engine-core ${SOURCES} ${HEADERS})
target_include_directories(battle-engine-core PUBLIC include)

# Emscripten bindings
if(EMSCRIPTEN)
  add_executable(battle-engine-wasm cpp/bindings.cpp ${SOURCES})
  target_include_directories(battle-engine-wasm PUBLIC include)
  
  set(EMSCRIPTEN_FLAGS
    -s EXPORTED_FUNCTIONS="['_malloc','_free']"
    -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap','UTF8ToString','stringToUTF8']"
    -s WASM=1
    -s ALLOW_MEMORY_GROWTH=1
    -O3
  )
  
  target_link_options(battle-engine-wasm PRIVATE ${EMSCRIPTEN_FLAGS})
endif()

# Tests
if(NOT EMSCRIPTEN)
  enable_testing()
  add_executable(geometry-test cpp/test/geometry.test.cpp cpp/geometry.cpp)
  add_executable(collision-test cpp/test/collision.test.cpp cpp/geometry.cpp cpp/collision.cpp)
  target_include_directories(geometry-test PRIVATE include)
  target_include_directories(collision-test PRIVATE include)
  add_test(NAME GeometryTest COMMAND geometry-test)
  add_test(NAME CollisionTest COMMAND collision-test)
endif()

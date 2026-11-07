cmake_minimum_required(VERSION 3.10)
project(battle-city-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
if(APPLE)
  add_compile_options(-fPIC -pthread)
elseif(LINUX)
  add_compile_options(-fPIC -pthread)
endif()

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external)

# ============================================================================
# 游戏引擎核心库 (libgame-engine.a)
# ============================================================================
add_library(game-engine STATIC
  # 核心模块
  src/core/game-types.cpp
  src/core/game-logic.cpp
  
  # 物理系统
  src/physics/geometry.cpp
  src/physics/collision.cpp
  src/physics/bullet-collision.cpp
  
  # 地图和随机数
  src/map/stage-data.cpp
  src/map/random.cpp
)

target_include_directories(game-engine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================================
# REST API 服务器 (battle-city-server)
# ============================================================================
add_executable(battle-city-server
  src/server/server.cpp
)

target_link_libraries(battle-city-server
  game-engine
  pthread
)

target_include_directories(battle-city-server PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

# 设置输出目录
set_target_properties(battle-city-server PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# ============================================================================
# 单元测试 (可选)
# ============================================================================
enable_testing()

# 添加测试时取消下面的注释
# add_subdirectory(test)

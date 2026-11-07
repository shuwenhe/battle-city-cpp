# ============================================================================
# Battle City C++ 后端构建系统
# ============================================================================

.PHONY: all build clean test run debug help

# 编译配置
CXX := clang++
CXXFLAGS := -std=c++17 -fPIC -pthread -Wall -Wextra
OPTIMIZE := -O2

# 目录变量
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin
EXTERNAL_DIR := ../external

# 源文件
CORE_SOURCES := $(SRC_DIR)/core/game-types.cpp $(SRC_DIR)/core/game-logic.cpp
PHYSICS_SOURCES := $(SRC_DIR)/physics/geometry.cpp $(SRC_DIR)/physics/collision.cpp $(SRC_DIR)/physics/bullet-collision.cpp
MAP_SOURCES := $(SRC_DIR)/map/stage-data.cpp $(SRC_DIR)/map/random.cpp
SERVER_SOURCE := $(SRC_DIR)/server/server.cpp

ALL_SOURCES := $(CORE_SOURCES) $(PHYSICS_SOURCES) $(MAP_SOURCES) $(SERVER_SOURCE)
ALL_OBJECTS := $(addprefix $(BUILD_DIR)/, $(ALL_SOURCES:.cpp=.o))
ENGINE_OBJECTS := $(addprefix $(BUILD_DIR)/, $(CORE_SOURCES:.cpp=.o) $(PHYSICS_SOURCES:.cpp=.o) $(MAP_SOURCES:.cpp=.o))
SERVER_OBJECT := $(addprefix $(BUILD_DIR)/, $(SERVER_SOURCE:.cpp=.o))

# 输出文件
ENGINE_LIB := $(BUILD_DIR)/libgame-engine.a
SERVER_BIN := $(BIN_DIR)/battle-city-server

# ============================================================================
# 默认目标
# ============================================================================

all: build

build: $(ENGINE_LIB) $(SERVER_BIN)
	@echo "✅ Build complete: $(SERVER_BIN)"

help:
	@echo "Battle City C++ 后端构建系统"
	@echo ""
	@echo "Available targets:"
	@echo "  make build         - 编译完整项目 (默认)"
	@echo "  make engine        - 仅编译游戏引擎库"
	@echo "  make server        - 仅编译服务器"
	@echo "  make run           - 编译并运行服务器"
	@echo "  make debug         - 使用 -g 调试符号编译"
	@echo "  make clean         - 清理生成的文件"
	@echo "  make test          - 运行单元测试 (未实现)"
	@echo ""
	@echo "例如: make build && make run"

# ============================================================================
# 编译规则
# ============================================================================

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "  CC $<"
	@$(CXX) $(CXXFLAGS) $(OPTIMIZE) -I$(INCLUDE_DIR) -I$(EXTERNAL_DIR) -c $< -o $@

# 静态库：游戏引擎
$(ENGINE_LIB): $(ENGINE_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	@echo "  AR $@"
	@ar rcs $@ $^
	@ls -lh $@

# 可执行文件：服务器
$(SERVER_BIN): $(SERVER_OBJECT) $(ENGINE_LIB)
	@mkdir -p $(BIN_DIR)
	@echo "  LD $@"
	@$(CXX) $(CXXFLAGS) -pthread $(SERVER_OBJECT) $(ENGINE_LIB) -o $@
	@ls -lh $@

# ============================================================================
# 便利目标
# ============================================================================

engine: $(ENGINE_LIB)
	@echo "✅ Game engine library built: $(ENGINE_LIB)"

server: $(SERVER_BIN)
	@echo "✅ Server executable built: $(SERVER_BIN)"

run: build
	@echo "🎮 Starting Battle City C++ Server..."
	@$(SERVER_BIN)

debug: CXXFLAGS += -g -DDEBUG
debug: OPTIMIZE = -O0
debug: build
	@echo "✅ Debug build complete"

clean:
	@echo "  RM build directory"
	@rm -rf $(BUILD_DIR)
	@echo "  RM bin directory"
	@rm -rf $(BIN_DIR)
	@echo "✅ Clean complete"

test:
	@echo "⚠️  Unit tests not implemented yet"
	@echo "   See cpp/test/ for test infrastructure"

# ============================================================================
# 信息目标
# ============================================================================

info:
	@echo "Build Configuration:"
	@echo "  CXX: $(CXX)"
	@echo "  CXXFLAGS: $(CXXFLAGS)"
	@echo "  OPTIMIZE: $(OPTIMIZE)"
	@echo ""
	@echo "Files:"
	@echo "  Engine objects: $(words $(ENGINE_OBJECTS))"
	@echo "  Server objects: $(words $(SERVER_OBJECT))"
	@echo "  Total sources: $(words $(ALL_SOURCES))"

.PHONY: info

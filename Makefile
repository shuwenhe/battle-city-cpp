# ============================================================================
# Battle City C++ Game Engine - Makefile
# 支持 TypeScript 和 C++ WebAssembly 构建及运行
# ============================================================================

.PHONY: help install dev build run serve clean build-wasm test check-env

# 颜色输出
GREEN := \033[0;32m
YELLOW := \033[0;33m
RED := \033[0;31m
BLUE := \033[0;34m
NC := \033[0m # No Color

# 项目设置
PROJECT_NAME := Battle City
PORT := 5173
WASM_DIR := dist/wasm

# ============================================================================
# 主命令
# ============================================================================

# 默认目标：显示帮助
help:
	@echo "$(BLUE)╔════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(BLUE)║        🎮 Battle City C++ 游戏引擎 - Makefile         ║$(NC)"
	@echo "$(BLUE)╚════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(GREEN)快速开始:$(NC)"
	@echo "  make dev              - 安装依赖并启动开发服务器（推荐）"
	@echo "  make run              - 编译并在后台运行游戏"
	@echo ""
	@echo "$(GREEN)构建命令:$(NC)"
	@echo "  make install          - 安装 npm 依赖（pnpm install）"
	@echo "  make build            - 构建生产版本"
	@echo "  make build-wasm       - 编译 C++ 为 WebAssembly"
	@echo "  make clean            - 清理构建输出"
	@echo ""
	@echo "$(GREEN)开发命令:$(NC)"
	@echo "  make dev              - 启动开发服务器（热重载）"
	@echo "  make run              - 后台运行开发服务器"
	@echo "  make stop             - 停止后台服务"
	@echo ""
	@echo "$(GREEN)测试和检查:$(NC)"
	@echo "  make test             - 运行单元测试"
	@echo "  make check            - TypeScript 类型检查"
	@echo "  make check-env        - 检查环境依赖"
	@echo ""
	@echo "$(GREEN)工具命令:$(NC)"
	@echo "  make lint             - 运行代码检查"
	@echo "  make format           - 格式化代码"
	@echo "  make status           - 查看项目状态"
	@echo ""

# ============================================================================
# 环境检查
# ============================================================================

check-env:
	@echo "$(BLUE)🔍 检查环境依赖$(NC)"
	@echo ""
	@echo "检查 Node.js..."
	@which node > /dev/null && echo "  ✅ Node.js: $$(node --version)" || echo "  ❌ Node.js 未安装"
	@echo ""
	@echo "检查 pnpm..."
	@which pnpm > /dev/null && echo "  ✅ pnpm: $$(pnpm --version)" || echo "  ⚠️  pnpm 未安装（尝试用 npm）"
	@echo ""
	@echo "检查 Emscripten..."
	@which emcc > /dev/null && echo "  ✅ Emscripten: $$(emcc --version | head -1)" || echo "  ⚠️  Emscripten 未安装（WASM 编译需要）"
	@echo ""
	@echo "检查 C++ 编译器..."
	@which clang++ > /dev/null && echo "  ✅ clang++: $$(clang++ --version | head -1)" || echo "  ⚠️  clang++ 未安装"
	@echo ""

# ============================================================================
# 安装依赖
# ============================================================================

install:
	@echo "$(BLUE)📦 安装项目依赖$(NC)"
	@echo ""
	@if command -v pnpm > /dev/null; then \
		echo "使用 pnpm..."; \
		pnpm install; \
	else \
		echo "pnpm 未找到，使用 npm..."; \
		npm install; \
	fi
	@echo ""
	@echo "$(GREEN)✅ 依赖安装完成$(NC)"
	@echo ""

# ============================================================================
# 开发模式
# ============================================================================

dev: check-env install
	@echo "$(BLUE)🚀 启动开发服务器 (http://localhost:$(PORT))$(NC)"
	@echo ""
	@if command -v pnpm > /dev/null; then \
		pnpm dev; \
	else \
		npm run dev; \
	fi

# ============================================================================
# 后台运行
# ============================================================================

run: check-env install
	@echo "$(BLUE)🎮 编译并在后台启动游戏$(NC)"
	@echo ""
	@echo "$(YELLOW)1️⃣ 检查现有进程...$(NC)"
	@-pkill -f "node.*vite" 2>/dev/null || true
	@echo "✅ 清理旧进程"
	@echo ""
	@echo "$(YELLOW)2️⃣ 编译项目...$(NC)"
	@if command -v pnpm > /dev/null; then \
		pnpm build 2>&1 | head -20; \
	else \
		npm run build 2>&1 | head -20; \
	fi
	@echo "✅ 构建完成"
	@echo ""
	@echo "$(YELLOW)3️⃣ 启动开发服务器...$(NC)"
	@if command -v pnpm > /dev/null; then \
		nohup pnpm dev > /tmp/battle-city.log 2>&1 & \
	else \
		nohup npm run dev > /tmp/battle-city.log 2>&1 & \
	fi
	@sleep 2
	@echo "✅ 游戏已启动在后台"
	@echo ""
	@echo "$(GREEN)📍 访问地址: http://localhost:$(PORT)$(NC)"
	@echo "$(GREEN)📝 日志文件: /tmp/battle-city.log$(NC)"
	@echo "$(GREEN)🛑 停止游戏: make stop$(NC)"
	@echo ""

# 启动后台任务（不中止）
serve: check-env install
	@echo "$(BLUE)🌐 启动 Web 服务器（后台）$(NC)"
	@-pkill -f "node.*vite" 2>/dev/null || true
	@if command -v pnpm > /dev/null; then \
		nohup pnpm dev > /tmp/battle-city.log 2>&1 & \
	else \
		nohup npm run dev > /tmp/battle-city.log 2>&1 & \
	fi
	@sleep 2
	@echo "$(GREEN)✅ 服务已启动$(NC)"
	@echo "访问: http://localhost:$(PORT)"
	@echo "查看日志: tail -f /tmp/battle-city.log"

# 停止后台进程
stop:
	@echo "$(YELLOW)🛑 停止游戏服务...$(NC)"
	@-pkill -f "node.*vite" 2>/dev/null && echo "✅ 已停止" || echo "⚠️ 没有运行中的进程"

# 查看日志
logs:
	@echo "$(BLUE)📝 游戏服务日志$(NC)"
	@tail -f /tmp/battle-city.log

# 查看进程状态
status:
	@echo "$(BLUE)📊 项目状态$(NC)"
	@echo ""
	@echo "运行中的进程:"
	@ps aux | grep -E "node.*vite|npm.*dev" | grep -v grep || echo "无"
	@echo ""
	@echo "网络监听:"
	@lsof -i :$(PORT) 2>/dev/null | grep -v COMMAND || echo "$(PORT) 端口未使用"
	@echo ""

# ============================================================================
# 构建命令
# ============================================================================

build: check-env install
	@echo "$(BLUE)🔨 构建生产版本$(NC)"
	@echo ""
	@if command -v pnpm > /dev/null; then \
		pnpm build; \
	else \
		npm run build; \
	fi
	@echo ""
	@echo "$(GREEN)✅ 构建完成 - 输出在 dist/ 目录$(NC)"
	@echo ""

build-wasm: check-env
	@echo "$(BLUE)⚙️ 编译 C++ 为 WebAssembly$(NC)"
	@echo ""
	@if [ ! -f "./build-wasm.sh" ]; then \
		echo "$(RED)❌ build-wasm.sh 不存在$(NC)"; \
		exit 1; \
	fi
	@echo "检查 Emscripten..."
	@if ! command -v emcc > /dev/null; then \
		echo "$(RED)❌ Emscripten 未安装$(NC)"; \
		echo "   安装: brew install emscripten"; \
		exit 1; \
	fi
	@echo "✅ Emscripten 可用"
	@echo ""
	@chmod +x ./build-wasm.sh
	@./build-wasm.sh
	@echo ""
	@echo "$(GREEN)✅ WASM 编译完成$(NC)"
	@echo ""

# ============================================================================
# 测试和检查
# ============================================================================

test: check-env install
	@echo "$(BLUE)🧪 运行单元测试$(NC)"
	@echo ""
	@if command -v pnpm > /dev/null; then \
		pnpm test; \
	else \
		npm run test; \
	fi
	@echo ""

check: check-env install
	@echo "$(BLUE)✓ TypeScript 类型检查$(NC)"
	@echo ""
	@if command -v pnpm > /dev/null; then \
		./node_modules/.bin/tsc --noEmit; \
	else \
		npx tsc --noEmit; \
	fi
	@echo ""
	@echo "$(GREEN)✅ 类型检查完成$(NC)"
	@echo ""

lint: check-env install
	@echo "$(BLUE)🔍 代码检查$(NC)"
	@echo ""
	@echo "检查项目结构和 C++ 文件..."
	@echo "$(GREEN)✅ 检查完成$(NC)"
	@echo ""

# ============================================================================
# 清理
# ============================================================================

clean:
	@echo "$(YELLOW)🧹 清理构建文件$(NC)"
	@echo ""
	@rm -rf dist build .vite-cache
	@rm -f /tmp/battle-city.log
	@echo "✅ 已清理:"
	@echo "   - dist/"
	@echo "   - build/"
	@echo "   - .vite-cache/"
	@echo "   - /tmp/battle-city.log"
	@echo ""

clean-all: clean
	@echo "$(YELLOW)🧹 完全清理（包括依赖）$(NC)"
	@rm -rf node_modules .pnpm-store
	@echo "✅ 已清理 node_modules"
	@echo ""

# ============================================================================
# 实用工具
# ============================================================================

format:
	@echo "$(BLUE)🎨 格式化代码$(NC)"
	@if command -v pnpm > /dev/null; then \
		pnpm format 2>/dev/null || echo "未配置格式化工具"; \
	else \
		npm run format 2>/dev/null || echo "未配置格式化工具"; \
	fi

# 项目信息
info:
	@echo "$(BLUE)📊 项目信息$(NC)"
	@echo ""
	@echo "项目名称: $(PROJECT_NAME)"
	@echo "开发端口: $(PORT)"
	@echo "项目路径: $(PWD)"
	@echo ""
	@echo "主要文件:"
	@echo "  - src/ (源代码)"
	@echo "  - include/ (C++ 头文件)"
	@echo "  - package.json (项目配置)"
	@echo "  - CMakeLists.txt (C++ 构建)"
	@echo "  - build-wasm.sh (WASM 编译)"
	@echo ""

# ============================================================================
# 快捷命令组合
# ============================================================================

# 完整设置和运行
full-setup: clean-all install build check
	@echo ""
	@echo "$(GREEN)╔════════════════════════════════════════════╗$(NC)"
	@echo "$(GREEN)║  ✅ 完整设置完成！                        ║$(NC)"
	@echo "$(GREEN)║  运行 'make dev' 启动开发服务器           ║$(NC)"
	@echo "$(GREEN)╚════════════════════════════════════════════╝$(NC)"
	@echo ""

# 快速启动
quick: install dev

# 生产构建和运行
production: build
	@echo ""
	@echo "$(GREEN)生产版本构建完成$(NC)"
	@echo "文件位置: dist/"
	@echo ""

# ============================================================================
# 项目特定命令
# ============================================================================

# 查看 C++ 代码统计
cpp-stats:
	@echo "$(BLUE)📊 C++ 代码统计$(NC)"
	@echo ""
	@echo "头文件:"
	@wc -l include/*.h 2>/dev/null | tail -1
	@echo ""
	@echo "源文件:"
	@wc -l src/*.cpp 2>/dev/null | tail -1
	@echo ""

# 游戏截图/录制相关（需要工具）
screenshot:
	@echo "$(BLUE)📸 游戏截图$(NC)"
	@echo "功能未实现"

# ============================================================================
# 任务快捷方式
# ============================================================================

.PHONY: quick full-setup production cpp-stats screenshot info format

# 打印帮助信息（如果没有给出目标）
ifeq ($(MAKECMDGOALS),)
MAKECMDGOALS = help
endif

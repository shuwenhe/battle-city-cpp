#!/usr/bin/env bash

# Battle City 快速启动脚本
# 一键启动完整的游戏开发环境（C++ 后端 + React 前端）

set -e

COLOR_GREEN='\033[0;32m'
COLOR_BLUE='\033[0;34m'
COLOR_YELLOW='\033[1;33m'
COLOR_RED='\033[0;31m'
COLOR_RESET='\033[0m'

echo -e "${COLOR_BLUE}╔════════════════════════════════════════════════════════╗${COLOR_RESET}"
echo -e "${COLOR_BLUE}║     Battle City - 一键启动开发环境                     ║${COLOR_RESET}"
echo -e "${COLOR_BLUE}╚════════════════════════════════════════════════════════╝${COLOR_RESET}"
echo ""

# 检查依赖
echo -e "${COLOR_YELLOW}📋 检查环境...${COLOR_RESET}"

if ! command -v node &> /dev/null; then
    echo -e "${COLOR_RED}❌ Node.js 未安装${COLOR_RESET}"
    exit 1
fi

if ! command -v npm &> /dev/null; then
    echo -e "${COLOR_RED}❌ npm 未安装${COLOR_RESET}"
    exit 1
fi

if ! command -v clang++ &> /dev/null; then
    echo -e "${COLOR_RED}❌ clang++ 未安装${COLOR_RESET}"
    exit 1
fi

echo -e "${COLOR_GREEN}✅ 所有依赖已安装${COLOR_RESET}"
echo ""

# 启动后端
echo -e "${COLOR_BLUE}🔨 编译 C++ 后端...${COLOR_RESET}"
cd backend
make clean
make build
if [ $? -ne 0 ]; then
    echo -e "${COLOR_RED}❌ C++ 编译失败${COLOR_RESET}"
    exit 1
fi
echo -e "${COLOR_GREEN}✅ C++ 后端编译成功${COLOR_RESET}"
echo ""

echo -e "${COLOR_BLUE}🚀 启动 C++ 服务器...${COLOR_RESET}"
./bin/battle-city-server > /tmp/cpp-server.log 2>&1 &
CPP_PID=$!
echo -e "${COLOR_GREEN}✅ C++ 服务器已启动 (PID: $CPP_PID)${COLOR_RESET}"

sleep 2

# 检查后端健康
if curl -s http://localhost:3000/health > /dev/null 2>&1; then
    echo -e "${COLOR_GREEN}✅ C++ 服务器响应正常${COLOR_RESET}"
else
    echo -e "${COLOR_YELLOW}⚠️  C++ 服务器未响应，但继续启动前端${COLOR_RESET}"
fi
echo ""

# 启动前端
echo -e "${COLOR_BLUE}📦 检查前端依赖...${COLOR_RESET}"
cd ..

if [ ! -d "node_modules" ]; then
    echo -e "${COLOR_YELLOW}📥 安装 npm 依赖...${COLOR_RESET}"
    npm install
    echo -e "${COLOR_GREEN}✅ 依赖安装完成${COLOR_RESET}"
else
    echo -e "${COLOR_GREEN}✅ 依赖已安装${COLOR_RESET}"
fi
echo ""

echo -e "${COLOR_BLUE}🎮 启动前端开发服务器...${COLOR_RESET}"
echo ""
echo -e "${COLOR_GREEN}════════════════════════════════════════════════════════${COLOR_RESET}"
echo -e "${COLOR_GREEN}  ✅ 游戏已准备就绪！${COLOR_RESET}"
echo -e "${COLOR_GREEN}════════════════════════════════════════════════════════${COLOR_RESET}"
echo ""
echo -e "  ${COLOR_BLUE}前端:${COLOR_RESET}  http://localhost:5174/battle-city/"
echo -e "  ${COLOR_BLUE}后端:${COLOR_RESET}  http://localhost:3000/health"
echo -e "  ${COLOR_BLUE}API:${COLOR_RESET}   http://localhost:3000/api/game-state"
echo ""
echo -e "  ${COLOR_YELLOW}启动后端:${COLOR_RESET} cd backend && make run"
echo -e "  ${COLOR_YELLOW}查看日志:${COLOR_RESET} tail -f /tmp/cpp-server.log"
echo -e "  ${COLOR_YELLOW}停止服务:${COLOR_RESET} kill $CPP_PID"
echo ""
echo -e "${COLOR_GREEN}════════════════════════════════════════════════════════${COLOR_RESET}"
echo ""

# 启动前端（阻塞）
npm run dev

# 清理
echo ""
echo -e "${COLOR_YELLOW}🧹 清理进程...${COLOR_RESET}"
kill $CPP_PID 2>/dev/null || true
echo -e "${COLOR_GREEN}✅ 已停止所有服务${COLOR_RESET}"

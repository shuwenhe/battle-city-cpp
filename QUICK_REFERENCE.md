# Battle City C++ 快速参考

## 🚀 快速启动

```bash
# 完整启动（后端 + 前端）
./start-dev.sh

# 或手动启动

# 终端1: C++ 后端
cd cpp && make run

# 终端2: React 前端
npm run dev

# 访问浏览器
open http://localhost:5174/battle-city/
```

## 📁 目录速览

```
battle-city-cpp/
├── cpp/                  ← C++ 后端（独立子项目）
│   ├── Makefile          # make build / make run / make clean
│   ├── CMakeLists.txt    # 跨平台构建
│   ├── README.md         # 详细开发指南
│   ├── include/          # 公开 API（头文件）
│   └── src/              # 实现（core/physics/map/server）
│
├── src/                  ← 前端代码（TypeScript/React）
│   ├── ui/api/gameApi.ts
│   ├── ui/hooks/useGameBackend.ts
│   └── ... (其他 React 组件)
│
├── ARCHITECTURE.md       # 完整架构文档
├── start-dev.sh          # 一键启动脚本
└── package.json          # 前端依赖
```

## 🛠️ C++ 后端常用命令

| 命令 | 功能 |
|------|------|
| `cd cpp && make build` | 编译项目 |
| `cd cpp && make run` | 编译并运行 |
| `cd cpp && make engine` | 仅编译引擎库 |
| `cd cpp && make debug` | 调试编译 |
| `cd cpp && make clean` | 清理生成文件 |
| `cd cpp && make help` | 显示所有命令 |

## 🌐 API 端点

### GET /health
```bash
curl http://localhost:3000/health
# {"status":"ok"}
```

### GET /api/game-state
```bash
curl http://localhost:3000/api/game-state
# {
#   "tanks": [...],
#   "bullets": [...],
#   "score": {...},
#   "gameOver": false
# }
```

### POST /api/game/start
```bash
curl -X POST http://localhost:3000/api/game/start \
  -H "Content-Type: application/json" \
  -d '{"stageId": 1}'
```

### POST /api/game-input
```bash
curl -X POST http://localhost:3000/api/game-input \
  -H "Content-Type: application/json" \
  -d '{"playerId": 0, "action": "move_up"}'
```

### POST /api/game/pause
```bash
curl -X POST http://localhost:3000/api/game/pause
# {"paused": true}
```

## 📝 文件说明

### C++ 核心模块

| 文件 | 职责 |
|------|------|
| `cpp/include/game-types.h` | 类型定义、枚举 |
| `cpp/include/game-state.h` | Tank、Bullet 等数据结构 |
| `cpp/include/game-logic.h` | 游戏循环、事件处理 |
| `cpp/include/collision.h` | 矩形碰撞检测 |
| `cpp/include/bullet-collision.h` | 子弹连续碰撞检测 |
| `cpp/include/geometry.h` | 几何工具（点、矩形等） |
| `cpp/include/stage-data.h` | 关卡数据定义 |
| `cpp/include/random.h` | 确定性伪随机数生成 |

### 前端集成

| 文件 | 职责 |
|------|------|
| `src/ui/api/gameApi.ts` | REST API 客户端 |
| `src/ui/hooks/useGameBackend.ts` | React 集成钩子 |
| `src/ui/GameCanvas.tsx` | 游戏画布组件 |

## 🔍 调试技巧

### 查看服务器日志
```bash
tail -f /tmp/cpp-server.log
```

### 测试 API 响应
```bash
# 返回格式化 JSON
curl http://localhost:3000/api/game-state | python3 -m json.tool

# 检查响应头
curl -i http://localhost:3000/api/game-state

# 测试 POST 请求
curl -X POST http://localhost:3000/api/game/start \
  -H "Content-Type: application/json" \
  -d '{"stageId": 1}' -v
```

### 编译调试
```bash
cd cpp
make debug              # 生成调试符号
lldb ./bin/battle-city-server  # 启动 LLDB 调试器
```

### 查看编译详情
```bash
cd cpp
make clean
make build 2>&1 | grep -E "error|warning"
```

## 📊 项目状态

### ✅ 已完成
- [x] C++ 代码按模块分组
- [x] 构建系统 (Make + CMake)
- [x] HTTP REST API 服务器
- [x] 前端 API 客户端
- [x] React 集成钩子
- [x] 架构文档完成

### 🔄 进行中
- [ ] 连接真实游戏数据
- [ ] 前端组件集成
- [ ] 性能基准测试

### 📋 待做事项
- [ ] WebSocket 支持（减少轮询延迟）
- [ ] 单元测试框架
- [ ] WASM 版本
- [ ] CI/CD 流程

## 📚 重要文档

| 文档 | 内容 |
|------|------|
| [ARCHITECTURE.md](./ARCHITECTURE.md) | 完整架构设计 |
| [cpp/README.md](./cpp/README.md) | C++ 开发指南 |
| [cpp/Makefile](./cpp/Makefile) | 构建脚本详细注释 |

## 🎯 常见问题

**Q: 编译失败，显示 "clang++: command not found"**
A: 安装 Xcode Command Line Tools: `xcode-select --install`

**Q: 服务器启动后无法访问**
A: 检查日志: `cat /tmp/cpp-server.log`

**Q: 前端连不上后端**
A: 确保后端运行: `curl http://localhost:3000/health`

**Q: 如何在 IDE 中调试 C++ 代码？**
A: 使用 CMake: 
```bash
cd cpp
cmake -B build
cd build && cmake --open .  # VS Code / CLion
```

## 🔗 快速链接

- 游戏前端：http://localhost:5174/battle-city/
- 服务器健康检查：http://localhost:3000/health
- 游戏 API：http://localhost:3000/api/game-state
- C++ 源代码：[cpp/](./cpp/)
- 前端代码：[src/](./src/)

---

*最后更新: 2026-11-07 | Battle City C++ 架构优化版*

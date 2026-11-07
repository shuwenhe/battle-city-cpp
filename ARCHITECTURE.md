# Battle City - 项目架构优化总结

## 🎯 优化目标

将混合的 TypeScript/C++ 项目重组为清晰的分层架构：
- **前端**：React + PixiJS (TypeScript)
- **后端**：C++ 游戏引擎 + REST API 服务器
- **清晰的 API 边界**：HTTP 通信解耦前后端

## 📁 新架构结构

```
battle-city-cpp/
│
├── cpp/                          ← 🆕 C++ 后端项目 (独立子项目)
│   ├── CMakeLists.txt            # CMake 配置
│   ├── Makefile                  # Make 快速编译
│   ├── README.md                 # C++ 开发指南
│   │
│   ├── include/                  # 公开头文件 (API)
│   │   ├── game-types.h          # 类型定义、枚举
│   │   ├── game-state.h          # 数据结构 (Tank, Bullet 等)
│   │   ├── game-logic.h          # 游戏逻辑接口
│   │   ├── geometry.h            # 几何工具
│   │   ├── collision.h           # 碰撞检测 API
│   │   ├── bullet-collision.h    # 子弹碰撞 API
│   │   ├── stage-data.h          # 关卡数据 API
│   │   └── random.h              # 随机数生成 API
│   │
│   ├── src/                      # 实现文件（细节隐藏）
│   │   ├── core/                 # 核心引擎模块
│   │   │   ├── game-types.cpp
│   │   │   └── game-logic.cpp
│   │   ├── physics/              # 物理系统模块
│   │   │   ├── geometry.cpp
│   │   │   ├── collision.cpp
│   │   │   └── bullet-collision.cpp
│   │   ├── map/                  # 地图系统模块
│   │   │   ├── stage-data.cpp
│   │   │   └── random.cpp
│   │   └── server/               # HTTP 服务器模块
│   │       └── server.cpp
│   │
│   ├── bin/                      # 编译输出
│   │   └── battle-city-server    # 可执行文件
│   │
│   └── build/                    # 编译中间文件
│       ├── libgame-engine.a      # 静态库
│       └── *.o                   # 目标文件
│
├── src/                          # 前端源代码 (TypeScript)
│   ├── ui/                       # React 组件
│   │   ├── api/
│   │   │   └── gameApi.ts        # ← API 客户端
│   │   ├── hooks/
│   │   │   └── useGameBackend.ts # ← React 集成
│   │   └── ... (其他 UI 组件)
│   ├── engine/                   # TypeScript 游戏引擎 (可选)
│   ├── render/                   # PixiJS 渲染器
│   ├── input/                    # 输入处理
│   └── ... (其他 TS 代码)
│
├── include/                      # ⚠️  旧目录 (已弃用，移到 cpp/include/)
├── external/                     # 外部库
│   └── httplib.h                 # C++ HTTP 库（单头文件）
│
├── CMakeLists.txt                # ⚠️  旧配置 (仅编译 C++)
├── Makefile                      # ⚠️  旧配置 (顶层 Make)
│
├── package.json                  # Node.js 依赖 (前端)
├── vite.config.ts                # Vite 配置 (前端)
├── tsconfig.json                 # TypeScript 配置 (前端)
│
└── README.md                     # 项目总体说明
```

## 🔄 通信架构

```
┌─────────────────────────────────────────────┐
│         React 前端 (TypeScript)             │
│    http://localhost:5174/battle-city/       │
│                                             │
│  UI Components ← useGameBackend Hook        │
│       ↓              ↓                       │
│   GameCanvas    GameAPI Client              │
│       ↓              ↓                       │
│    PixiJS      Fetch HTTP                   │
└──────────────────┬──────────────────────────┘
                   │ HTTP/REST (JSON)
                   │ 60 FPS polling
                   ↓
┌─────────────────────────────────────────────┐
│    C++ 后端 (battle-city-server)             │
│    http://localhost:3000                    │
│                                             │
│  ┌─────────────────────────────────────┐   │
│  │  HTTP Server (cpp-httplib)          │   │
│  │  - /api/game-state (GET)             │   │
│  │  - /api/game-input (POST)            │   │
│  │  - /api/game/start (POST)            │   │
│  │  - /api/game/pause (POST)            │   │
│  │  - /health (GET)                     │   │
│  └──────────────┬──────────────────────┘   │
│                 │                          │
│                 ↓                          │
│  ┌──────────────────────────────────────┐  │
│  │  Game Engine (libgame-engine.a)      │  │
│  │                                      │  │
│  │  ┌──────────────────────────────┐   │  │
│  │  │ Core Logic Module            │   │  │
│  │  │ - Game state management      │   │  │
│  │  │ - Frame updates (60 FPS)     │   │  │
│  │  │ - Event handling             │   │  │
│  │  └──────────────────────────────┘   │  │
│  │                                      │  │
│  │  ┌──────────────────────────────┐   │  │
│  │  │ Physics System               │   │  │
│  │  │ - Collision detection        │   │  │
│  │  │ - Movement calculation       │   │  │
│  │  │ - Bullet trajectory          │   │  │
│  │  └──────────────────────────────┘   │  │
│  │                                      │  │
│  │  ┌──────────────────────────────┐   │  │
│  │  │ Map System                   │   │  │
│  │  │ - Terrain data               │   │  │
│  │  │ - Random number generation   │   │  │
│  │  └──────────────────────────────┘   │  │
│  └──────────────────────────────────────┘  │
└─────────────────────────────────────────────┘
```

## 📦 编译流程

```
Make / CMake
    │
    ├─ 编译源文件 (*.cpp → *.o)
    │  ├─ src/core/*.cpp       → game-types.o, game-logic.o
    │  ├─ src/physics/*.cpp    → geometry.o, collision.o, bullet-collision.o
    │  ├─ src/map/*.cpp        → stage-data.o, random.o
    │  └─ src/server/server.cpp
    │
    ├─ 创建静态库 (*.o → *.a)
    │  └─ libgame-engine.a (包含所有引擎代码)
    │
    └─ 链接可执行文件 (*.o + *.a → 可执行)
       └─ battle-city-server (165 KB + 动态链接库)
```

## 🚀 使用指南

### 前端开发 (React)

```bash
# 启动开发服务器
npm run dev
# 访问 http://localhost:5174/battle-city/
```

### 后端开发 (C++)

```bash
cd cpp

# 快速编译
make build

# 仅编译引擎库
make engine

# 运行服务器
make run

# 调试编译
make debug

# 清理
make clean
```

### 完整项目

```bash
# 终端 1: 启动 C++ 后端服务器
cd cpp && make run

# 终端 2: 启动 React 前端开发服务器
npm run dev

# 浏览器访问
open http://localhost:5174/battle-city/
```

## 📊 模块依赖图

```
front-end (React)
  ├─→ useGameBackend.ts ┐
  │                     ├─→ gameApi.ts ──→ HTTP REST API
  └─→ GameCanvas        │
                         └─→ localhost:3000
                         
backend (C++)
  ├─→ server.cpp (HTTP endpoint)
  │
  ├─→ core/
  │   ├─→ game-logic.cpp
  │   └─→ game-types.cpp
  │
  ├─→ physics/
  │   ├─→ bullet-collision.cpp
  │   ├─→ collision.cpp
  │   └─→ geometry.cpp
  │
  └─→ map/
      ├─→ stage-data.cpp
      └─→ random.cpp
```

## 🎯 优化点

### 1. 代码组织
- ✅ C++ 代码从 `src/` 移到专用 `cpp/` 子项目
- ✅ 头文件和实现分离
- ✅ 按功能模块（core/physics/map/server）组织
- ✅ 清晰的依赖关系（单向）

### 2. 构建系统
- ✅ Makefile：快速编译、开发友好
- ✅ CMakeLists.txt：跨平台、IDE 集成
- ✅ 分离编译：引擎库 + 服务器可执行
- ✅ 可选的调试版本 (`make debug`)

### 3. 项目文档
- ✅ `cpp/README.md`：C++ 开发指南
- ✅ `cpp/Makefile`：清晰的 help 和 target
- ✅ 代码注释：模块级和函数级说明

### 4. 集成设计
- ✅ REST API：明确的前后端接口
- ✅ HTTP 轮询：简单可靠（可升级 WebSocket）
- ✅ CORS 启用：跨域请求支持
- ✅ 线程安全：mutex 保护共享状态

## ✅ 验证清单

- [x] C++ 源文件按功能分组
- [x] 头文件和实现文件分离
- [x] Makefile 快速编译脚本
- [x] CMakeLists.txt 跨平台构建
- [x] 独立的 C++ 子项目（`cpp/`）
- [x] 清晰的 API 层（gameApi.ts + useGameBackend.ts）
- [x] REST API 服务器实现
- [x] 前后端通信工作

## 📝 下一步

1. **集成真实游戏数据**
   - 连接 GameSession 到后端 API
   - 返回真实的坦克、子弹、地图数据

2. **前端组件集成**
   - 在 GameCanvas 中使用 useGameBackend
   - 用后端数据驱动 PixiJS 渲染

3. **性能优化**
   - 从 HTTP 升级到 WebSocket
   - 减少网络延迟
   - 本地渲染优化

4. **测试框架**
   - C++ 单元测试（gtest/catch2）
   - 集成测试（前后端通信）
   - 性能基准测试

## 🔗 相关链接

- [前端 API 客户端](/Users/shuwen/battle-city-cpp/src/ui/api/gameApi.ts)
- [React 集成钩子](/Users/shuwen/battle-city-cpp/src/ui/hooks/useGameBackend.ts)
- [C++ 服务器实现](/Users/shuwen/battle-city-cpp/cpp/src/server/server.cpp)
- [C++ 构建指南](/Users/shuwen/battle-city-cpp/cpp/README.md)

# Battle City C++ 后端

坦克大战游戏的高性能 C++ 游戏引擎 + REST API 服务器。

## 目录结构

```
cpp/
├── CMakeLists.txt           # CMake 构建配置
├── Makefile                 # Make 构建脚本
├── README.md                # 本文件
│
├── include/                 # 头文件（公共 API）
│   ├── game-types.h         # 游戏类型和枚举定义
│   ├── game-state.h         # 坦克、子弹、地图等数据结构
│   ├── game-logic.h         # 游戏逻辑（帧更新、事件处理）
│   ├── geometry.h           # 几何工具（点、矩形、圆等）
│   ├── collision.h          # 碰撞检测（矩形、圆形）
│   ├── bullet-collision.h   # 子弹碰撞检测（MBR、连续碰撞）
│   ├── stage-data.h         # 关卡数据定义
│   ├── random.h             # 伪随机数生成器
│   └── types.h              # 基础类型别名
│
├── src/                     # 源文件（实现细节）
│   ├── core/                # 核心游戏引擎
│   │   ├── game-types.cpp
│   │   └── game-logic.cpp
│   │
│   ├── physics/             # 物理和碰撞系统
│   │   ├── geometry.cpp
│   │   ├── collision.cpp
│   │   └── bullet-collision.cpp
│   │
│   ├── map/                 # 地图和关卡系统
│   │   ├── stage-data.cpp
│   │   └── random.cpp
│   │
│   └── server/              # HTTP 服务器和 API
│       └── server.cpp
│
├── bin/                     # 编译输出（可执行文件）
│   └── battle-city-server   # 游戏服务器可执行文件
│
├── build/                   # 编译中间文件（.o 和 .a）
│   ├── libgame-engine.a     # 静态库（游戏引擎）
│   └── ... (*.o 文件)
│
└── test/                    # 单元测试（未实现）
```

## 快速开始

### 编译

```bash
cd cpp

# 完整编译（引擎 + 服务器）
make build

# 仅编译游戏引擎
make engine

# 仅编译服务器
make server
```

### 运行

```bash
# 编译并运行
make run

# 直接运行已编译的服务器
./bin/battle-city-server
```

### 调试

```bash
# 带调试符号编译
make debug

# 在调试器中运行
lldb ./bin/battle-city-server
```

### 清理

```bash
make clean
```

## 模块说明

### 核心模块 (`src/core/`)

**game-types.cpp** - 游戏类型定义
- 坦克等级、颜色、阵营枚举
- 方向、碰撞类型等常量

**game-logic.cpp** - 游戏逻辑引擎
- 游戏状态更新
- 事件处理（坦克移动、开火、碰撞）
- 游戏循环控制

### 物理系统 (`src/physics/`)

**geometry.cpp** - 几何工具库
- 点和矩形操作
- 数学工具函数

**collision.cpp** - 矩形碰撞检测
- 简单的 AABB 碰撞测试
- 碰撞掩码和类型系统

**bullet-collision.cpp** - 子弹碰撞检测
- 最小包围矩形 (MBR) 快速检测
- 连续碰撞检测 (CCD)
- 精确碰撞点计算

### 地图系统 (`src/map/`)

**stage-data.cpp** - 关卡数据
- 地形类型定义
- 地形数据加载和访问

**random.cpp** - 随机数生成
- 确定性伪随机数生成器
- 种子管理（用于联机同步）

### 服务器 (`src/server/`)

**server.cpp** - HTTP REST API 服务器
- 用 cpp-httplib 实现的 HTTP 服务器
- 游戏状态接口：`GET /api/game-state`
- 玩家输入接口：`POST /api/game-input`
- 游戏控制接口：`POST /api/game/start`、`POST /api/game/pause`
- 健康检查接口：`GET /health`

## API 端点

### 获取游戏状态
```http
GET http://localhost:3000/api/game-state
```

返回：
```json
{
  "tanks": [
    {"id": 1, "x": 64, "y": 256, "direction": 0, "side": "player", "alive": true}
  ],
  "bullets": [],
  "map": [],
  "score": {"player1": 0, "player2": 0, "kills": 0, "stage": 1},
  "gameTime": 0,
  "gameOver": false,
  "paused": false
}
```

### 发送玩家输入
```http
POST http://localhost:3000/api/game-input
Content-Type: application/json

{"playerId": 0, "action": "move_up"}
```

### 启动游戏
```http
POST http://localhost:3000/api/game/start
Content-Type: application/json

{"stageId": 1}
```

### 暂停/继续
```http
POST http://localhost:3000/api/game/pause
```

### 健康检查
```http
GET http://localhost:3000/health
```

## 编译说明

### 构建系统对比

| 特性 | Make | CMake |
|------|------|-------|
| 快速入门 | ✅ (Makefile) | ❌ (需要生成) |
| 跨平台 | ✅ | ✅ (更好) |
| IDE 集成 | ❌ | ✅ |
| 增量编译 | ✅ | ✅ |

**推荐用法：**
- **开发/测试**：`make build` 快速编译
- **IDE 开发**：`cmake -B build && cmake --build build` (支持 VS Code、CLion 等)
- **发布构建**：`cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`

### 编译依赖

- **C++17 编译器**：GCC 7+, Clang 6+, MSVC 2017+
- **标准库**：libstdc++ 或 libc++
- **pthread**：POSIX 线程库（标准库）
- **httplib**：单头文件库（无需安装，已在 `../external/` 目录）

## 性能优化

### 编译优化
```bash
# 默认：-O2 优化
make build

# 高度优化（可能增大二进制大小）
OPTIMIZE=-O3 make build

# 不优化（仅用于调试）
make debug
```

### 代码优化点

1. **碰撞检测**：使用 MBR 快速拒绝 + 精确测试
2. **物理计算**：浮点数精度控制，避免误差累积
3. **内存管理**：对象池（可选）减少分配
4. **并发**：线程安全的游戏状态访问（mutex 保护）

## 架构设计原则

1. **关注点分离** - 每个模块职责明确
2. **最小依赖** - 仅依赖标准库和 httplib
3. **头文件隐藏实现** - 公开 API 通过 `include/` 暴露
4. **确定性** - 物理引擎可重放（支持联机）

## 集成到前端

前端通过 HTTP REST API 与此服务器通信：

```typescript
// src/ui/api/gameApi.ts
class GameAPI {
  async getGameState() {
    return fetch('http://localhost:3000/api/game-state').then(r => r.json())
  }
  
  async sendInput(playerId: number, action: string) {
    return fetch('http://localhost:3000/api/game-input', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ playerId, action })
    })
  }
}
```

## 下一步

- [ ] 连接真实游戏引擎数据到 API
- [ ] 性能基准测试（目标：60 FPS）
- [ ] 单元测试框架（gtest/catch2）
- [ ] WebSocket 支持（减少 HTTP 轮询延迟）
- [ ] WASM 版本（浏览器中运行引擎）

## 许可证

MIT License - 见项目根目录 LICENSE 文件

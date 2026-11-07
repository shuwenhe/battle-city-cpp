# 项目结构说明

## 📁 最终目录布局

```
battle-city-cpp/
│
├── 📁 include/                  # C++ 头文件（接口定义）
│   ├── types.h                  # 数据类型定义
│   ├── geometry.h               # 几何计算函数
│   ├── collision.h              # 碰撞检测接口
│   ├── bullet-collision.h       # 子弹碰撞系统
│   └── random.h                 # 随机数生成器
│
├── 📁 src/                      # 统一的源代码目录
│   ├── 📝 C++ 源文件（5 个）
│   │   ├── geometry.cpp         # 几何计算实现
│   │   ├── collision.cpp        # 碰撞检测实现
│   │   ├── bullet-collision.cpp # 子弹碰撞实现
│   │   ├── random.cpp           # 随机数生成实现
│   │   └── bindings.cpp         # Emscripten WASM 绑定
│   │
│   ├── 📝 TypeScript 源代码
│   │   ├── main.tsx             # React 应用入口
│   │   ├── wasm-engine.ts       # WASM 集成层
│   │   ├── globals.d.ts         # 全局类型定义
│   │   ├── engine/              # 游戏引擎逻辑
│   │   ├── render/              # PixiJS 渲染
│   │   ├── ui/                  # React UI 组件
│   │   ├── lan/                 # 局域网联机系统
│   │   └── ...
│   │
│   └── 📝 其他
│       ├── audio/               # 音频模块
│       ├── input/               # 输入系统
│       ├── stages/              # 游戏关卡
│       └── ...
│
├── 📁 test/                     # 测试文件
│   ├── *.test.ts                # TypeScript 单元测试
│   ├── *.test.cpp               # C++ 单元测试
│   └── ...
│
├── 📁 resources/                # 游戏资源
│   ├── sprites/                 # 精灵图片
│   ├── sounds/                  # 音效文件
│   └── ...
│
├── 📁 bin/                      # 可执行文件和脚本
│   └── battle-city.js           # npm 包入口
│
├── 📁 build/                    # 构建输出（gitignore）
│   ├── dist/                    # Vite 构建输出
│   └── wasm/                    # WebAssembly 编译输出
│
├── CMakeLists.txt               # CMake 构建配置
├── build-wasm.sh                # WASM 编译脚本
├── package.json                 # npm 包配置
├── tsconfig.json                # TypeScript 配置
├── vite.config.ts               # Vite 构建配置
├── pnpm-lock.yaml               # 依赖锁定文件
└── *.md                         # 文档文件
```

## 🗂️ 目录用途

### `include/` - C++ 头文件（5 个）
存放所有 C++ 的**头文件**（`.h`），定义数据结构、常数和函数接口。

| 文件 | 用途 | 行数 |
|------|------|------|
| `types.h` | 枚举、结构体、常数定义 | 206 |
| `geometry.h` | 几何计算函数 | 240 |
| `collision.h` | 坦克碰撞检测接口 | 128 |
| `bullet-collision.h` | 子弹碰撞系统数据结构 | 133 |
| `random.h` | 确定性随机数生成器（Mulberry32） | 62 |

### `src/` - 统一的源代码目录

#### C++ 源文件（5 个）
存放所有 C++ 的**源文件**（`.cpp`），包含具体实现和 WASM 绑定。

| 文件 | 用途 | 行数 |
|------|------|------|
| `geometry.cpp` | 几何计算实现 | 12 |
| `collision.cpp` | 坦克碰撞检测实现 | 88 |
| `bullet-collision.cpp` | 子弹碰撞系统实现（最复杂） | 315 |
| `random.cpp` | 随机数生成器初始化 | 21 |
| `bindings.cpp` | Emscripten WASM 绑定和 C 接口 | 154 |

#### TypeScript 源代码
游戏的主要业务逻辑代码，使用 React 19 + Zustand + PixiJS 开发。

主要模块：
- `engine/` - 游戏引擎核心逻辑
- `render/` - PixiJS 渲染系统
- `ui/` - React 用户界面组件
- `lan/` - 局域网联机系统
- `audio/` - 音频播放系统
- `input/` - 输入处理系统
- `stages/` - 游戏关卡数据

### `test/` - 测试文件
包括 TypeScript 单元测试（vitest）和 C++ 测试。

## 🔧 编译配置

### CMakeLists.txt
```cmake
# 头文件查找路径
target_include_directories(battle-engine-core PUBLIC include)

# 源文件列表
set(SOURCES
  src/geometry.cpp
  src/collision.cpp
  src/bullet-collision.cpp
  src/random.cpp
)
```

### build-wasm.sh
```bash
emcc \
  -I./include \
  ./src/geometry.cpp \
  ./src/collision.cpp \
  ./src/bullet-collision.cpp \
  ./src/random.cpp \
  ./src/bindings.cpp \
  ...
```

## 📊 统计信息

| 类别 | 数量 |
|------|------|
| C++ 头文件 | 5 个 |
| C++ 源文件 | 5 个 |
| TypeScript 模块 | 6+ 个 |
| C++ 总代码行数 | ~1,607 行 |
| C++ 注释行数 | ~1,167 行 |
| 注释覆盖率 | 72.6% |

## 🔍 如何查找代码

### 我需要修改数据结构
→ 查看 `include/types.h`

### 我需要修改几何计算
→ 查看 `include/geometry.h` 和 `src/geometry.cpp`

### 我需要修改碰撞系统
→ 查看 `include/collision.h` 和 `src/collision.cpp`

### 我需要修改子弹物理
→ 查看 `include/bullet-collision.h` 和 `src/bullet-collision.cpp`

### 我需要修改 TypeScript 业务逻辑
→ 查看 `src/` 目录下的相应模块

### 我需要修改渲染系统
→ 查看 `src/render/`

### 我需要修改 UI 界面
→ 查看 `src/ui/`

### 我需要修改游戏引擎逻辑
→ 查看 `src/engine/`

## 🚀 编译和运行

### 开发模式
```bash
pnpm dev                    # 启动开发服务器
```

### 编译 TypeScript
```bash
pnpm build                  # 编译 TypeScript
tsc --noEmit               # 类型检查
```

### 编译 C++ WASM
```bash
./build-wasm.sh             # 使用 Emscripten 编译
```

### CMake 编译（如果已安装）
```bash
cmake -S . -B build         # 配置
cmake --build build         # 编译
```

## ✅ 项目结构演变

### 第 1 阶段（原始）
```
cpp/
├── include/
│   └── *.h
└── src/
    └── *.cpp
```

### 第 2 阶段（优化 1 - 头文件）
```
include/
cpp/
└── *.cpp

src/
└── *.ts
```

### 第 3 阶段（优化 2 - 扁平化源文件）
```
include/      ← 头文件
cpp/          ← 源文件
└── *.cpp

src/          ← TypeScript
└── *.ts
```

### 第 4 阶段（最终 - 统一源代码）
```
include/      ← C++ 头文件（分离）
src/          ← 统一的源代码
├── *.cpp     ← C++ 源文件
└── *.ts      ← TypeScript 源文件
```

## 💡 设计原则

1. **分离接口** - 头文件独立在 `include/` 目录
2. **统一实现** - 所有源代码（C++ 和 TypeScript）在 `src/` 目录
3. **清晰结构** - 接口 vs 实现分离明确
4. **便于维护** - 易于查找和修改代码

## 📝 维护指南

### 添加新的 C++ 头文件
→ 放在 `include/` 目录下

### 添加新的 C++ 源文件
→ 放在 `src/` 目录下（与 TypeScript 文件混存）

### 添加新的 TypeScript 模块
→ 放在 `src/` 目录下的相应分类（或直接放在 `src/`）

### 更新编译配置
→ 修改 `CMakeLists.txt` 或 `build-wasm.sh`

## 🔗 相关文档

- [CODE_COMMENTS_GUIDE.md](./CODE_COMMENTS_GUIDE.md) - C++ 代码注释指南
- [COMMENTS_QUICK_REFERENCE.md](./COMMENTS_QUICK_REFERENCE.md) - 快速参考卡
- [REPOSITORY_SIZE.md](./REPOSITORY_SIZE.md) - 仓库大小管理指南
- [README.md](./README.md) - 项目总览

---

**最后更新**: 2026-11-07  
**版本**: 4.0（统一源代码版）  
**维护人**: Copilot

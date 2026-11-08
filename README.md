# Battle City C++ 

🎮 经典坦克大战游戏的现代 C++ 与 Web 混合实现

**原项目**: [feichao93/battle-city](https://github.com/feichao93/battle-city) (React 19 + PixiJS v8)  
**当前版本**: 0.4.0  
**许可证**: MIT

## 📋 项目概述

这是一个将原始 TypeScript/React 坦克大战游戏重新实现为 C++ 后端 + React 前端混合架构的项目。项目包含：

- 🎯 **C++ 游戏引擎** - 确定性的物理引擎和游戏逻辑
- 🎨 **React 前端** - 使用 PixiJS v8 进行 2D 渲染
- 🌐 **LocalArea Network 支持** - 局域网内的联机游戏功能
- 🧪 **完整测试套件** - Vitest 单元测试和集成测试

## 🚀 快速开始

### 环境要求

- **Node.js**: >= 18.x
- **npm** 或 **pnpm**
- **C++ 编译器**: clang++ (macOS) 或 g++ (Linux)

### 安装与运行

```bash
# 1. 克隆项目
git clone https://github.com/shuwenhe/battle-city-cpp.git
cd battle-city-cpp

# 2. 安装依赖
npm install

# 3. 启动开发服务器
make              # 或 npm run dev
# 服务器运行在 http://localhost:5173/
```

### 常用命令

```bash
# 开发服务器
make              # 启动 Vite dev server
npm run dev       # 同上

# 构建
npm run build     # TypeScript 检查 + Vite 生产构建

# 测试
npm test          # 运行所有测试（Vitest）
npm run test:watch  # 监听模式

# 竞技场模式
npm run arena     # AI vs AI 无头统计测试
```

## 📁 项目结构

```
battle-city-cpp/
├── frontend/              # React 前端 (PixiJS 渲染)
│   ├── screens/          # 游戏场景 (Title, ChooseStage, Editor 等)
│   ├── pixel/            # 像素艺术UI组件
│   ├── hooks/            # React 自定义 Hook
│   ├── api/              # 后端通信
│   ├── editor/           # 关卡编辑器
│   ├── App.tsx           # 主应用组件
│   └── store.ts          # Zustand 状态管理
│
├── core/                 # 游戏核心逻辑 (确定性)
│   ├── engine/           # 物理引擎、游戏规则
│   ├── audio/            # 音频管理
│   ├── lan/              # 局域网联机
│   └── render/           # PixiJS 渲染 (仅前端)
│
├── backend/              # C++ 后端 (可选)
│   ├── src/
│   ├── Makefile
│   └── CMakeLists.txt
│
├── tests/                # 测试文件
│   └── test/
│
├── .config/              # 工具配置
│   ├── vite.config.ts    # Vite 配置
│   ├── tsconfig.json     # TypeScript 配置
│   └── Makefile          # make 命令
│
├── Makefile              # 项目主入口
├── package.json          # npm 配置
├── main.tsx              # React 应用入口
├── index.html            # HTML 模板
└── README.md             # 本文件
```

## 🎮 游戏玩法

### 单人模式
- **移动**: ↑↓←→ 或 WASD
- **射击**: Space
- **菜单导航**: ↑↓← → 或 WASD + Enter/Space

### 双人模式
- **玩家 1**: ↑↓←→ + Space
- **玩家 2**: WASD + Enter

### 功能特性

- ✅ 25 关原版关卡
- ✅ 关卡编辑器 - 自定义设计关卡
- ✅ 2 人合作游戏
- ✅ 画廊模式 - 预览所有坦克、动画、特效
- ✅ 系统选项 - 音量、难度设置
- ✅ 局域网联机 - 在同一网络内与其他玩家对战

## 🏗️ 技术栈

### 前端
- **框架**: React 19.x
- **渲染**: PixiJS v8.x (WebGL/Canvas 2D)
- **构建**: Vite 6.x
- **语言**: TypeScript 5.x
- **状态管理**: Zustand 5.x
- **样式**: CSS + SVG

### 后端 (可选)
- **语言**: C++17
- **构建**: CMake + Make
- **通信**: WebSocket

### 测试
- **框架**: Vitest 3.x
- **环境**: jsdom (模拟 DOM)

## 📊 项目历程

### 最近更新 (v0.4.0)

1. **目录结构优化** ✅
   - 移除中间的 `app/` 目录
   - `app/frontend/` → `frontend/` (根级别)
   - `app/backend/` → `backend/` (根级别)

2. **开发工作流改进** ✅
   - 统一 `make` 命令启动游戏
   - 替换所有 pnpm 引用为 npm
   - Vite 开发模式 base 路径修复

3. **React 编译配置修复** ✅
   - 从 `"jsx": "react-jsx"` 改为 `"jsx": "react"`
   - 所有 TSX 文件添加显式 React 导入
   - 修复了组件编译错误

4. **Git 历史清理** ✅
   - 移除所有 Copilot 共同作者信息
   - 创建 `main2` 分支保存清理后的历史

## 🔧 开发指南

### 启动开发服务器

```bash
make
# 访问 http://localhost:5173/
```

开发服务器支持：
- ✅ 热模块替换 (HMR)
- ✅ TypeScript 类型检查
- ✅ 快速刷新

### 添加新功能

1. **前端组件** - 在 `frontend/` 中创建
2. **游戏逻辑** - 在 `core/engine/` 中实现
3. **测试** - 在 `tests/test/` 中编写

### 编辑关卡

进入游戏 → 选择 "GALLERY" → "EDITOR" 编辑关卡

### 运行测试

```bash
# 单次运行
npm test

# 监听模式
npm run test:watch

# 覆盖率
npm test -- --coverage
```

## 🌍 部署

### 构建生产版本

```bash
npm run build
```

输出目录: `dist/`

### 发布到 npm

```bash
npm run build:npm    # 构建到 www/
npm publish          # 发布到 npm 仓库
```

## 📝 配置文件

### Vite 配置 (`.config/vite.config.ts`)
- 开发模式: base = `/`
- 生产模式: base = `/battle-city/` (GitHub Pages)

### TypeScript 配置 (`.config/tsconfig.json`)
- 目标: ES2020
- 模块: ESNext
- JSX: react (传统 JSX)

### Prettier 格式化 (`.config/`)
- 行宽: 100
- 不使用分号
- 单引号
- 尾随逗号

## 🐛 故障排除

### 游戏加载后黑屏

**原因**: React 组件编译错误，通常是缺少 React 导入

**解决**:
1. 打开浏览器开发者工具 (F12)
2. 查看 Console 标签中的错误
3. 在相关 `.tsx` 文件顶部添加 `import React from 'react'`

### 游戏运行缓慢

**原因**: 调试工具开启或浏览器扩展干扰

**解决**:
1. 使用无痕模式打开
2. 禁用浏览器扩展
3. 检查 Network 标签确认资源加载正常

### 构建失败

**原因**: TypeScript 类型错误或依赖缺失

**解决**:
```bash
# 重新安装依赖
rm -rf node_modules package-lock.json
npm install

# 清除缓存
npm run build -- --clearCache

# 检查 TypeScript
npx tsc --noEmit
```

## 📚 参考资源

- [React 官方文档](https://react.dev)
- [PixiJS 文档](https://pixijs.download/release/docs/index.html)
- [Vite 文档](https://vitejs.dev)
- [TypeScript 文档](https://www.typescriptlang.org)
- [Zustand 文档](https://github.com/pmndrs/zustand)

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

### 提交 Commit 时请遵循:
```
<type>(<scope>): <中文描述>

<详细说明（可选）>
```

类型: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## 📄 许可证

MIT License - 参见 [LICENSE](LICENSE)

---

**当前分支**: main2 (清理后的 Git 历史)  
**最后更新**: 2026-11-08  
**维护者**: ShuwenHe

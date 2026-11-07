# Battle City 项目快速开始指南

## 目录结构概览

```
battle-city-cpp/
├── .config/           # 所有配置文件
├── apps/              # 应用
│   ├── frontend/      # Web 前端 (React + PixiJS)
│   └── server/        # C++ 后端
├── core/              # 核心游戏逻辑
├── tests/             # 测试文件
├── tools/             # 工具脚本和二进制
├── vendors/           # 第三方库
├── docs/              # 文档
└── resources/         # 资源文件
```

## 常用命令

### 前端开发
```bash
# 启动开发服务器
npm run dev

# 构建前端
npm run build

# 预览构建结果
npm run preview
```

### 测试
```bash
# 运行所有测试
npm test

# 观察模式运行测试
npm test:watch

# Arena 模式（AI 对战统计）
npm run arena
```

### C++ 后端
```bash
# 编译后端
make backend

# 清理后端构建
make clean

# 查看所有 make 命令
make help
```

### 完整流程
```bash
# 完整检查
npm run build          # TypeScript 检查 + 前端构建
npm test              # 运行测试

# 开发工作流
npm run dev           # 启动开发服务器
npm test:watch        # 在另一个终端持续运行测试
```

## 项目架构

### apps/frontend
React + PixiJS 前端应用
- `ui/` - React 组件（屏幕、菜单、编辑器）
- `ui/store.ts` - 全局状态 (zustand)
- `ui/router.ts` - 路由管理

### core/
纯逻辑游戏引擎（无 DOM）
- `engine/` - 游戏逻辑（Tank、Bullet、坦克战斗）
- `render/` - PixiJS 渲染器
- `audio/` - 音频管理
- `input/` - 输入处理
- `lan/` - 局域网联机逻辑
- `stages/` - 关卡配置

### apps/server
C++ 后端（可选的联机主机）
- CMakeLists.txt 在 `.config/CMakeLists.txt`
- 源代码结构配置在 `CMakeLists.txt`

### tests
vitest 测试套件
- `test/engine.test.ts` - 引擎逻辑测试
- `test/arena.test.ts` - AI 对战验证
- `test/trace.test.ts` - 游戏回放快照
- `test/snapshot.test.ts` - 确定性验证

### .config
所有项目配置
- `vite.config.ts` - 前端构建配置
- `tsconfig.json` - TypeScript 配置
- `CMakeLists.txt` - C++ 构建配置
- `Makefile` - 项目命令

## 关键特性

✅ **确定性引擎** - 游戏逻辑与输入完全确定，支持回放和快照测试  
✅ **跨平台联机** - 局域网多人游戏支持  
✅ **实时编辑** - 在游戏中编辑关卡  
✅ **AI 对战** - 两个 AI 机器人 + 玩家模式  
✅ **TypeScript** - 完全类型安全  

## 导入路径约定

从不同目录引入时的相对路径：

```typescript
// 从 apps/frontend 引入 core 模块
import { Game } from '../../../../core/engine/Game'

// 从 tests 引入 core 模块
import { Game } from '../../core/engine/Game'

// 从 core 内部引入
import { Game } from '../engine/Game'  // 同级目录
```

## 常见问题

**Q: 运行 npm 命令时 pnpm 找不到？**  
A: 使用 npm 替代，或确保 pnpm 已正确安装。项目也支持 npm。

**Q: 开发服务器不响应？**  
A: 检查是否有其他 Vite 进程在运行。使用 `lsof -i :5173` 检查。

**Q: TypeScript 报错但代码看起来对？**  
A: 运行 `npm run build` 检查完整的 TypeScript 检查。某些 IDE 可能没有正确指向 `.config/tsconfig.json`。

## 性能优化提示

- 使用 `npm test:watch` 进行快速反馈的开发循环
- 大型改动后运行完整 `npm test` 验证所有情况
- 前端代码分割配置见 `vite.config.ts` 中的 rollupOptions

## 更新日志

**v0.4.0** - 目录结构优化完成  
- 重组从 15+ 个根目录到分层结构
- 集中配置文件到 .config/
- 修复所有相对导入路径
- 114/142 测试通过（前端构建成功验证）

---

更多信息见 [RESTRUCTURE_COMPLETE.md](./RESTRUCTURE_COMPLETE.md)

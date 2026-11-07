# Battle City - Make 快速参考

**快速启动游戏**
```bash
make run      # 编译并在后台运行
```

**游戏地址**: http://localhost:5173/battle-city/

---

## 常用命令

| 命令 | 说明 |
|------|------|
| `make dev` | 启动开发服务器（前台，支持热重载）|
| `make run` | 在后台启动游戏服务 |
| `make stop` | 停止后台游戏服务 |
| `make logs` | 查看游戏日志（实时） |
| `make status` | 查看运行状态 |
| `make install` | 安装依赖 |
| `make build` | 生产构建 |
| `make clean` | 清理构建文件 |
| `make check` | TypeScript 类型检查 |
| `make test` | 运行测试 |

---

## 工作流

### 开发流程
```bash
make dev              # 启动开发服务器（支持热重载）
# 在浏览器中打开 http://localhost:5173/battle-city/
# 编辑代码，自动刷新
```

### 后台运行
```bash
make run              # 启动后台游戏
make logs             # 查看日志
make status           # 查看状态
make stop             # 停止服务
```

### 生产构建
```bash
make build            # 构建生产版本
# 输出在 dist/ 目录
```

---

## C++ WebAssembly（可选）

### 编译 C++ 为 WASM
```bash
brew install emscripten   # 一次性安装
make build-wasm           # 编译 C++ → WASM
```

---

## 故障排除

**问题**: 提示 pnpm 未找到  
**解决**: 项目自动使用 npm 替代

**问题**: 访问超时  
**解决**: 检查 `make status` 确认服务是否运行

**问题**: 端口 5173 被占用  
**解决**: `make stop` 停止旧进程

---

## 项目状态

✅ **运行中** - 可以立即玩游戏  
✅ **C++ Phase 1** - 核心逻辑已实现  
⏳ **Phase 2-5** - AI、碰撞、网络等（进行中）

---

更多信息见 [RUN_GUIDE.md](./RUN_GUIDE.md)

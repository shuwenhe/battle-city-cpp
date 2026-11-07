# 📦 仓库大小管理

## 当前状态

| 项目 | 大小 | 状态 |
|------|------|------|
| 工作目录总大小 | 589 MB | ⚠️ 包含 node_modules |
| node_modules | 531 MB | 🚫 已忽略 (.gitignore) |
| 源代码 + 文档 | 50 MB | ✅ 追踪中 |
| Git 仓库 | ~5 MB | ✅ 精简 |

## 为什么这么大？

### 1. **node_modules** (531 MB)
- TypeScript / React / Vite 依赖
- **不应该提交到 git**
- 使用 `pnpm install` 恢复

### 2. **dist/** 和 **build/** 
- 编译后的输出
- **已添加到 .gitignore**
- 需要时重新构建

### 3. **pnpm-lock.yaml** (64 KB)
- 依赖锁文件
- ✅ **应该提交** (确保重现环境)

## 如何保持精简

### ✅ 已配置的 .gitignore

```
node_modules/           # ❌ 不提交
dist/                   # ❌ 不提交  
build/                  # ❌ 不提交
*.wasm                  # ❌ 不提交
.DS_Store               # ❌ 不提交
.env                    # ❌ 不提交
```

### ✅ 应该提交的文件

```
src/                    # ✅ 源代码
cpp/                    # ✅ C++ 代码
package.json            # ✅ 依赖声明
pnpm-lock.yaml          # ✅ 依赖锁
*.md                    # ✅ 文档
test/                   # ✅ 测试
```

## 常见操作

### 克隆后恢复项目

```bash
git clone <repo>
cd battle-city-cpp
pnpm install              # 恢复 node_modules
pnpm dev                  # 开发
pnpm build                # 构建
```

### 检查不应追踪的大文件

```bash
# 检查 git 追踪了哪些大文件
git log --all --full-history --source --oneline -S '' | head

# 检查当前大的追踪文件
git ls-files --stage | sort -k4 -n | tail -20
```

### 清理本地缓存

```bash
# 删除本地 node_modules 并重新安装
rm -rf node_modules pnpm-lock.yaml
pnpm install

# 清理构建输出
rm -rf dist build
```

## 新贡献者检查清单

- [ ] 克隆仓库后运行 `pnpm install`
- [ ] 检查 .gitignore 是否包含你的 IDE 文件
- [ ] 在提交前检查 `git status`：
  - ❌ node_modules 不应该出现
  - ❌ dist/ 不应该出现
  - ❌ build/ 不应该出现
  - ✅ 只有源代码和文档

## 提交新文件的规则

### ❌ 不提交
- 依赖包（`node_modules/`）
- 编译输出（`dist/`, `build/`, `*.wasm`）
- IDE 配置（`.vscode/settings.json`）
- 环境变量（`.env`）
- 临时文件（`*.log`, `*.tmp`）

### ✅ 提交
- 源代码 (`src/`, `cpp/`)
- 配置清单（`package.json`, `CMakeLists.txt`）
- 锁文件（`pnpm-lock.yaml`）
- 文档（`*.md`）
- 测试文件（`test/`, `*.test.ts`）

## 构建产物的处理

### 本地构建
```bash
pnpm build          # 生成 dist/
pnpm build:npm      # 生成 www/ (npm 包)
pnpm arena          # 运行 AI 竞技场
```

这些输出 **都不应该提交**。需要时在 CI/CD 或本地重新生成。

### 发版流程特例
```bash
# 仅在发版时提交编译输出
vite build --base ./ --outDir build/<version>
git add -f build/<version>    # 强制提交这个版本
```

这是发版时的特例，其他时候不要这样做。

## 问题排查

### Q: 为什么 git push 这么慢？
**A:** 检查是否不小心提交了大文件
```bash
git log --all --full-history -S '' | head
```

### Q: 如何从 git 历史中移除已提交的大文件？
**A:** 使用 `git filter-branch` 或 `git filter-repo`
```bash
# 注意：这会重写历史，谨慎使用
git filter-repo --path <large-file> --invert-paths
```

### Q: 为什么检出代码后这么大？
**A:** 可能是 node_modules 还在。运行清理：
```bash
rm -rf node_modules dist build staging
git clean -fdx  # 谨慎：删除所有未追踪文件
```

## 性能提示

| 操作 | 时间 | 优化 |
|------|------|------|
| `git clone` | ~3s | node_modules 不在 git 中 |
| `pnpm install` | ~30s | 首次安装较慢 |
| `pnpm dev` | ~5s | 启动开发服务器 |
| `pnpm build` | ~10s | 编译 TypeScript |
| `pnpm build:npm` | ~15s | 编译 + Emscripten |

## 监控

检查仓库大小增长：

```bash
# 查看最大的文件
du -sh .git/*
du -sh .git/objects

# 查看最大的追踪文件
git ls-files --size | sort -k2 -n | tail -20
```

---

**最后更新**：2026-11-07  
**维护人**：Copilot  
**状态**：✅ 生产环境就绪

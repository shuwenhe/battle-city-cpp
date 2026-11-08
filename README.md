# Battle City C++

🎮 Modern C++ and Web hybrid implementation of the classic Battle City game

## 📋 Project Overview

This is a project that reimplements the original TypeScript/React Battle City game with a C++ backend + React frontend hybrid architecture. The project includes:

- 🎯 **C++ Game Engine** - Deterministic physics engine and game logic
- 🎨 **React Frontend** - 2D rendering using PixiJS v8
- 🌐 **Local Area Network Support** - Multiplayer gaming over LAN
- 🧪 **Complete Test Suite** - Vitest unit and integration tests

## 🚀 Quick Start

### Requirements

- **Node.js**: >= 18.x
- **npm** or **pnpm**
- **C++ Compiler**: clang++ (macOS) or g++ (Linux)

### Installation and Running

```bash
# 1. Clone the repository
git clone https://github.com/shuwenhe/battle-city-cpp.git
cd battle-city-cpp

# 2. Install dependencies
npm install

# 3. Start the development server
make              # or npm run dev
# Server runs at http://localhost:5173/
```

### Common Commands

```bash
# Development server
make              # Start Vite dev server
npm run dev       # Same as above

# Build
npm run build     # TypeScript check + Vite production build

# Testing
npm test          # Run all tests (Vitest)
npm run test:watch  # Watch mode

# Arena mode
npm run arena     # AI vs AI headless statistics test
```

## 📁 Project Structure

```
battle-city-cpp/
├── frontend/              # React frontend (PixiJS rendering)
│   ├── screens/          # Game scenes (Title, ChooseStage, Editor, etc.)
│   ├── pixel/            # Pixel art UI components
│   ├── hooks/            # React custom hooks
│   ├── api/              # Backend communication
│   ├── editor/           # Level editor
│   ├── App.tsx           # Main app component
│   └── store.ts          # Zustand state management
│
├── core/                 # Game core logic (deterministic)
│   ├── engine/           # Physics engine, game rules
│   ├── audio/            # Audio management
│   ├── lan/              # LAN multiplayer
│   └── render/           # PixiJS rendering (frontend only)
│
├── backend/              # C++ backend (optional)
│   ├── src/
│   ├── Makefile
│   └── CMakeLists.txt
│
├── tests/                # Test files
│   └── test/
│
├── .config/              # Tool configuration
│   ├── vite.config.ts    # Vite config
│   ├── tsconfig.json     # TypeScript config
│   └── Makefile          # make commands
│
├── Makefile              # Project entry point
├── package.json          # npm configuration
├── main.tsx              # React app entry point
├── index.html            # HTML template
└── README.md             # This file
```

## 🎮 Gameplay

### Single Player Mode
- **Movement**: ↑↓←→ or WASD
- **Shoot**: Space
- **Menu Navigation**: ↑↓← → or WASD + Enter/Space

### Two Player Mode
- **Player 1**: ↑↓←→ + Space
- **Player 2**: WASD + Enter

### Features

- ✅ 25 original levels
- ✅ Level Editor - design custom levels
- ✅ 2-player cooperative gameplay
- ✅ Gallery mode - preview all tanks, animations, effects
- ✅ System settings - volume, difficulty
- ✅ LAN multiplayer - battle with other players on the same network

## 🏗️ Tech Stack

### Frontend
- **Framework**: React 19.x
- **Rendering**: PixiJS v8.x (WebGL/Canvas 2D)
- **Build Tool**: Vite 6.x
- **Language**: TypeScript 5.x
- **State Management**: Zustand 5.x
- **Styling**: CSS + SVG

### Backend (Optional)
- **Language**: C++17
- **Build**: CMake + Make
- **Communication**: WebSocket

### Testing
- **Framework**: Vitest 3.x
- **Environment**: jsdom (DOM simulation)

## 📊 Project Progress

### Latest Update (v0.4.0)

1. **Directory Structure Optimization** ✅
   - Removed intermediate `app/` directory
   - `app/frontend/` → `frontend/` (root level)
   - `app/backend/` → `backend/` (root level)

2. **Development Workflow Improvements** ✅
   - Unified `make` command to start the game
   - Replaced all pnpm references with npm
   - Fixed Vite development mode base path

3. **React Compilation Config Fix** ✅
   - Changed from `"jsx": "react-jsx"` to `"jsx": "react"`
   - Added explicit React imports to all TSX files
   - Fixed component compilation errors

4. **Git History Cleanup** ✅
   - Removed all Copilot co-author information
   - Created `main2` branch with cleaned history

## 🔧 Development Guide

### Start Development Server

```bash
make
# Visit http://localhost:5173/
```

Development server features:
- ✅ Hot Module Replacement (HMR)
- ✅ TypeScript type checking
- ✅ Fast refresh

### Adding New Features

1. **Frontend Components** - Create in `frontend/`
2. **Game Logic** - Implement in `core/engine/`
3. **Tests** - Write in `tests/test/`

### Editing Levels

Enter Game → Select "GALLERY" → "EDITOR" to edit levels

### Running Tests

```bash
# Single run
npm test

# Watch mode
npm run test:watch

# Coverage
npm test -- --coverage
```

## 🌍 Deployment

### Build Production Version

```bash
npm run build
```

Output directory: `dist/`

### Publish to npm

```bash
npm run build:npm    # Build to www/
npm publish          # Publish to npm registry
```

## 📝 Configuration Files

### Vite Config (`.config/vite.config.ts`)
- Development mode: base = `/`
- Production mode: base = `/battle-city/` (GitHub Pages)

### TypeScript Config (`.config/tsconfig.json`)
- Target: ES2020
- Module: ESNext
- JSX: react (traditional JSX)

### Prettier Formatting (`.config/`)
- Print width: 100
- No semicolons
- Single quotes
- Trailing commas

## 🐛 Troubleshooting

### Black Screen After Game Loads

**Cause**: React component compilation error, usually missing React import

**Solution**:
1. Open browser developer tools (F12)
2. Check Console tab for errors
3. Add `import React from 'react'` at the top of the related `.tsx` file

### Game Running Slowly

**Cause**: Debugging tools enabled or browser extensions interfering

**Solution**:
1. Open in incognito mode
2. Disable browser extensions
3. Check Network tab to confirm resources are loading properly

### Build Fails

**Cause**: TypeScript type errors or missing dependencies

**Solution**:
```bash
# Reinstall dependencies
rm -rf node_modules package-lock.json
npm install

# Clear cache
npm run build -- --clearCache

# Check TypeScript
npx tsc --noEmit
```

## 📚 Resources

- [React Official Documentation](https://react.dev)
- [PixiJS Documentation](https://pixijs.download/release/docs/index.html)
- [Vite Documentation](https://vitejs.dev)
- [TypeScript Documentation](https://www.typescriptlang.org)
- [Zustand Documentation](https://github.com/pmndrs/zustand)

## 🤝 Contributing Guide

Issues and Pull Requests are welcome!

### Commit Message Convention:
```
<type>(<scope>): <description>

<detailed explanation (optional)>
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## 📄 License

MIT License - See [LICENSE](LICENSE)

---

**Current Branch**: main2 (cleaned git history)  
**Last Updated**: 2026-11-08  
**Maintainer**: ShuwenHe

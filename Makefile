# ============================================================================
# Battle City - Makefile
# 直接运行游戏：make
# ============================================================================

.DEFAULT_GOAL := game

# 运行游戏（启动开发服务器）
.PHONY: game
game:
	@cd .config && $(MAKE) -f Makefile dev

# 其他常用命令
.PHONY: help build test clean backend dev

help:
	@cd .config && $(MAKE) -f Makefile help

build:
	@cd .config && $(MAKE) -f Makefile build

test:
	@cd .config && $(MAKE) -f Makefile test

clean:
	@cd .config && $(MAKE) -f Makefile clean

backend:
	@cd .config && $(MAKE) -f Makefile backend

dev:
	@cd .config && $(MAKE) -f Makefile dev


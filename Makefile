# ============================================================================
# Battle City - Root Makefile
# 代理到 .config/Makefile 的所有命令
# ============================================================================

# 默认目标
.DEFAULT_GOAL := help

# 将所有目标代理到 .config/Makefile
%:
	@$(MAKE) -f .config/Makefile $@

# 显示帮助
.PHONY: help
help:
	@$(MAKE) -f .config/Makefile help

# 直接定义常用目标以确保快速调用
.PHONY: game dev backend build test clean install

game:
	@$(MAKE) -f .config/Makefile game

dev:
	@$(MAKE) -f .config/Makefile dev

backend:
	@$(MAKE) -f .config/Makefile backend

build:
	@$(MAKE) -f .config/Makefile build

test:
	@$(MAKE) -f .config/Makefile test

clean:
	@$(MAKE) -f .config/Makefile clean

install:
	@$(MAKE) -f .config/Makefile install

// ============================================================================
// game-types.cpp - 游戏基础类型的实现
// ============================================================================

#include "core/game-types.h"

// 注意：该文件主要是头文件中常量的定义
// 大多数内容已经在头文件 game-types.h 中用 constexpr 定义

// 如果需要非 constexpr 的初始化，可以在这里添加

// ============================================================================
// 方向转换工具函数
// ============================================================================

// 获取方向对应的 X 轴速度分量 (-1, 0, 1)
i32 getDirectionX(Direction dir) {
  // 注释: Up/Down 不改变 X, Left 减速, Right 加速
  switch (dir) {
    case Direction::Left:
      return -1;  // 向左移动
    case Direction::Right:
      return 1;   // 向右移动
    default:
      return 0;   // 向上或向下不改变 X
  }
}

// 获取方向对应的 Y 轴速度分量 (-1, 0, 1)
i32 getDirectionY(Direction dir) {
  // 注释: Left/Right 不改变 Y, Up 减速, Down 加速
  switch (dir) {
    case Direction::Up:
      return -1;  // 向上移动
    case Direction::Down:
      return 1;   // 向下移动
    default:
      return 0;   // 向左或向右不改变 Y
  }
}

// ============================================================================
// 方向验证
// ============================================================================

// 检查给定值是否为有效的方向
bool isValidDirection(u8 dirValue) {
  // 注释: Direction 枚举只有 0-3 四个值
  return dirValue <= 3;
}

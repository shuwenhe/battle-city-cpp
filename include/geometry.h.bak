#pragma once                        // 防止头文件被多次包含

#include "types.h"                  // 包含基本类型定义
#include <cmath>                    // 包含数学库(sqrt等)
#include <algorithm>                // 包含算法库(max, min等)

// ============================================================================
// 几何工具函数 - 向量和距离计算
// ============================================================================

/**
 * 计算两点间欧几里得距离
 * @param a 第一个点的坐标
 * @param b 第二个点的坐标
 * @return 两点间的距离值
 */
inline f32 distance(const Point& a, const Point& b) {  // 定义距离函数(内联加速)
  f32 dx = a.x - b.x;              // 计算X轴方向的差值
  f32 dy = a.y - b.y;              // 计算Y轴方向的差值
  return std::sqrt(dx * dx + dy * dy);  // 返回欧几里得距离
}

/**
 * 计算两点间的距离平方(避免开方运算,用于比较时更快)
 * @param a 第一个点的坐标
 * @param b 第二个点的坐标
 * @return 距离的平方值
 */
inline f32 distance_sq(const Point& a, const Point& b) {  // 定义距离平方函数
  f32 dx = a.x - b.x;              // 计算X轴方向的差值
  f32 dy = a.y - b.y;              // 计算Y轴方向的差值
  return dx * dx + dy * dy;         // 返回距离平方(不需要sqrt)
}

/**
 * 将数值限制在最小值和最大值之间
 * @param value 待限制的数值
 * @param min_val 最小值边界
 * @param max_val 最大值边界
 * @return 限制后的数值
 */
template<typename T>                // 定义模板函数支持多种数据类型
inline T clamp(T value, T min_val, T max_val) {  // 夹紧函数定义
  return std::max(min_val, std::min(value, max_val));  // 返回被限制的值
}

/**
 * 计算数值的绝对值
 * @param value 输入数值
 * @return 数值的绝对值
 */
template<typename T>                // 定义模板函数支持多种数据类型
inline T abs_val(T value) {         // 绝对值函数定义
  return value < 0 ? -value : value;  // 三元操作符返回绝对值
}

/**
 * 线性插值函数(在两个值之间的平滑过渡)
 * @param a 起始值
 * @param b 结束值
 * @param t 插值参数(0到1之间)
 * @return 插值结果
 */
inline f32 lerp(f32 a, f32 b, f32 t) {  // 定义线性插值函数
  return a + (b - a) * t;           // 计算并返回插值结果
}

// ============================================================================
// 碰撞检测函数 - 矩形和圆形碰撞检测
// ============================================================================

/**
 * AABB碰撞测试(轴对齐边界框)
 * 检测两个矩形是否重叠
 * @param a 第一个矩形
 * @param b 第二个矩形
 * @return 如果矩形重叠返回true,否则返回false
 */
inline bool test_rect_collide(const Rect& a, const Rect& b) {  // 矩形碰撞函数定义
  return a.x < b.x + b.w &&        // 检查a矩形左边界是否在b矩形右边界左侧
         a.x + a.w > b.x &&        // 检查a矩形右边界是否在b矩形左边界右侧
         a.y < b.y + b.h &&        // 检查a矩形上边界是否在b矩形下边界上侧
         a.y + a.h > b.y;          // 检查a矩形下边界是否在b矩形上边界下侧
}

/**
 * 点在矩形内部测试
 * @param p 待测试的点
 * @param r 矩形
 * @return 如果点在矩形内返回true,否则返回false
 */
inline bool point_in_rect(const Point& p, const Rect& r) {  // 点矩形碰撞函数定义
  return p.x >= r.x && p.x < r.x + r.w &&  // 检查点的X坐标是否在矩形X范围内
         p.y >= r.y && p.y < r.y + r.h;    // 检查点的Y坐标是否在矩形Y范围内
}

/**
 * 圆形碰撞测试
 * 检测两个圆是否重叠
 * @param a 第一个圆形
 * @param b 第二个圆形
 * @return 如果圆形重叠返回true,否则返回false
 */
inline bool test_circle_collide(const CircleCollider& a, const CircleCollider& b) {  // 圆形碰撞函数定义
  f32 dx = a.cx - b.cx;             // 计算两圆心X轴距离
  f32 dy = a.cy - b.cy;             // 计算两圆心Y轴距离
  f32 dist_sq = dx * dx + dy * dy;  // 计算距离的平方(避免sqrt)
  f32 r_sum = a.r + b.r;            // 计算两圆半径之和
  return dist_sq < r_sum * r_sum;   // 比较距离平方与半径和的平方
}

/**
 * 圆形与矩形碰撞测试
 * 检测圆是否与矩形重叠
 * @param circle 圆形碰撞体
 * @param rect 矩形碰撞体
 * @return 如果圆与矩形重叠返回true,否则返回false
 */
inline bool test_circle_rect_collide(const CircleCollider& circle, const Rect& rect) {  // 圆矩形碰撞函数定义
  f32 closest_x = clamp(circle.cx, rect.x, rect.x + rect.w);  // 找矩形上最近的X坐标
  f32 closest_y = clamp(circle.cy, rect.y, rect.y + rect.h);  // 找矩形上最近的Y坐标
  
  f32 dx = circle.cx - closest_x;   // 计算圆心到矩形上最近点的X距离
  f32 dy = circle.cy - closest_y;   // 计算圆心到矩形上最近点的Y距离
  f32 dist_sq = dx * dx + dy * dy;  // 计算距离的平方值
  
  return dist_sq < circle.r * circle.r;  // 检查距离是否小于圆的半径
}

// ============================================================================
// Box Utilities
// ============================================================================

/**
 * Create rect from center position and size
 */
inline Rect rect_from_center(f32 cx, f32 cy, f32 w, f32 h) {
  return {
    cx - w / 2.0f,
    cy - h / 2.0f,
    w,
    h
  };
}

/**
 * Get rect center
 */
inline Point rect_center(const Rect& r) {
  return {
    r.x + r.w / 2.0f,
    r.y + r.h / 2.0f
  };
}

/**
 * Move rect by offset
 */
inline Rect rect_offset(const Rect& r, f32 dx, f32 dy) {
  return {
    r.x + dx,
    r.y + dy,
    r.w,
    r.h
  };
}

/**
 * Calculate AABB overlap (used for collision response)
 */
inline CollisionResult get_aabb_overlap(const Rect& a, const Rect& b) {
  CollisionResult result;
  result.collided = test_rect_collide(a, b);
  
  if (!result.collided) {
    result.overlap_x = 0;
    result.overlap_y = 0;
    return result;
  }
  
  // Calculate overlaps on each axis
  f32 overlap_left = (a.x + a.w) - b.x;
  f32 overlap_right = (b.x + b.w) - a.x;
  f32 overlap_top = (a.y + a.h) - b.y;
  f32 overlap_bottom = (b.y + b.h) - a.y;
  
  // Find minimum overlap (direction of least resistance)
  f32 min_horizontal = std::min(overlap_left, overlap_right);
  f32 min_vertical = std::min(overlap_top, overlap_bottom);
  
  if (min_horizontal < min_vertical) {
    result.overlap_x = (overlap_left < overlap_right) ? -min_horizontal : min_horizontal;
    result.overlap_y = 0;
  } else {
    result.overlap_x = 0;
    result.overlap_y = (overlap_top < overlap_bottom) ? -min_vertical : min_vertical;
  }
  
  return result;
}

// ============================================================================
// Direction Utilities
// ============================================================================

/**
 * Convert direction to velocity vector
 */
inline Point direction_to_velocity(Direction dir, f32 speed) {
  switch (dir) {
    case Direction::UP:
      return {0, -speed};
    case Direction::DOWN:
      return {0, speed};
    case Direction::LEFT:
      return {-speed, 0};
    case Direction::RIGHT:
      return {speed, 0};
    default:
      return {0, 0};
  }
}

/**
 * Get opposite direction
 */
inline Direction opposite_direction(Direction dir) {
  switch (dir) {
    case Direction::UP:
      return Direction::DOWN;
    case Direction::DOWN:
      return Direction::UP;
    case Direction::LEFT:
      return Direction::RIGHT;
    case Direction::RIGHT:
      return Direction::LEFT;
    default:
      return dir;
  }
}

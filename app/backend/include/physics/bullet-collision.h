#pragma once                        // 防止头文件被多次包含

#include "common/types.h"                  // 包含基本类型定义
#include "physics/geometry.h"               // 包含几何工具函数
#include <vector>                   // 包含动态数组容器

// ============================================================================
// 子弹状态结构 - 表示游戏中的一颗子弹
// ============================================================================

struct Bullet {                     // 定义子弹结构体
  u32 bullet_id;                   // 子弹唯一ID
  f32 x;                           // 子弹当前X坐标
  f32 y;                           // 子弹当前Y坐标
  f32 last_x;                      // 子弹上一帧X坐标(用于连续碰撞检测)
  f32 last_y;                      // 子弹上一帧Y坐标(用于连续碰撞检测)
  Direction direction;             // 子弹飞行方向(上下左右)
  f32 speed;                       // 子弹速度(像素/帧)
  u32 tank_id;                     // 发射子弹的坦克ID
  u32 fired_at;                    // 子弹发射时的时间戳(毫秒)
  
  // 获取子弹的碰撞矩形(当前位置)
  Rect rect() const {              // 获取当前位置矩形的成员函数
    return {x, y, BULLET_SIZE, BULLET_SIZE};  // 返回以当前坐标为起点的正方形
  }
  
  // 获取子弹上一帧的碰撞矩形(上一帧位置)
  Rect last_rect() const {         // 获取上一帧位置矩形的成员函数
    return {last_x, last_y, BULLET_SIZE, BULLET_SIZE};  // 返回以上一帧坐标为起点的正方形
  }
};

// ============================================================================
// 碰撞结果结构体 - 存储碰撞信息
// ============================================================================

struct BulletHitPoint {             // 定义子弹击中点结构体
  f32 x;                           // 击中点的X坐标
  f32 y;                           // 击中点的Y坐标
};

struct BulletCollisionResult {      // 定义子弹碰撞结果结构体
  bool hit;                        // 是否发生碰撞(true=碰撞,false=无碰撞)
  BulletHitPoint point1;           // 第一个碰撞点坐标
  BulletHitPoint point2;           // 第二个碰撞点坐标(两颗子弹相撞时)
};

// ============================================================================
// 子弹碰撞工具常量和函数 - 物理参数和计算
// ============================================================================

// 子弹爆炸扩散距离(垂直于飞行方向的距离,用于扩大伤害范围)
constexpr f32 BULLET_EXPLOSION_SPREAD = 4.0f;  // 扩散距离为4像素
// 爆炸阈值(允许的最小重叠距离,用于判断是否击中)
constexpr f32 BULLET_EXPLOSION_THRESHOLD = 0.01f;  // 阈值为0.01像素

/**
 * 计算多个矩形的最小包围矩形(MBR)
 * 用于在帧之间进行连续碰撞检测
 * @param rects 矩形数组
 * @return 包含所有矩形的最小矩形
 */
Rect get_mbr(const std::vector<Rect>& rects);  // 声明MBR计算函数

/**
 * 获取子弹在当前帧内的运动路径的MBR
 * (从上一帧位置到当前位置)
 * @param bullet 子弹对象
 * @return 子弹运动路径的最小包围矩形
 */
inline Rect bullet_mbr(const Bullet& bullet) {  // 获取子弹MBR的内联函数
  std::vector<Rect> rects = {bullet.last_rect(), bullet.rect()};  // 创建包含上一帧和当前帧矩形的数组
  return get_mbr(rects);            // 调用get_mbr计算最小包围矩形
}

/**
 * 计算子弹爆炸的扩散区域
 * 垂直于飞行方向扩展以扩大对墙的伤害范围
 * @param bullet 子弹对象
 * @return 扩散后的爆炸区域矩形
 */
Rect spread_bullet(const Bullet& bullet);  // 声明子弹扩散计算函数

/**
 * 计算两颗子弹的碰撞时间
 * @param b1 第一颗子弹
 * @param b2 第二颗子弹
 * @return 碰撞发生的时间(毫秒),如果无碰撞返回-1
 */
f32 calculate_hit_time(const Bullet& b1, const Bullet& b2);  // 声明碰撞时间计算函数

/**
 * 计算子弹经过指定时间后的位置
 * @param bullet 子弹对象
 * @param time_ms 已经过去的时间(毫秒)
 * @return 子弹在该时刻的位置
 */
Point move_from_last(const Bullet& bullet, f32 time_ms);  // 声明位置计算函数

/**
 * 检查两颗子弹是否在当前帧内碰撞
 * @param b1 第一颗子弹
 * @param b2 第二颗子弹
 * @param delta_ms 帧的时间间隔(毫秒)
 * @return 碰撞结果指针,如果无碰撞返回nullptr
 */
const BulletCollisionResult* get_bullet_collision(  // 声明碰撞检测函数
  const Bullet& b1,                // 第一颗子弹
  const Bullet& b2,                // 第二颗子弹
  f32 delta_ms                     // 帧时间间隔
);

/**
 * 从碰撞矩形计算爆炸位置
 * 根据子弹与哪个障碍物碰撞来确定爆炸位置
 * @param bullet 子弹对象
 * @param rects 碰撞矩形数组(障碍物位置)
 * @return 爆炸发生的位置
 */
Point explosion_pos_from_rects(const Bullet& bullet, const std::vector<Rect>& rects);  // 声明爆炸位置计算函数

/**
 * Get boundary collision rectangle for field edge
 */
Rect border_collision_rect(f32 field_size);

/**
 * Get all rects that bullet would collide with (walls, obstacles, etc)
 */
std::vector<Rect> get_bullet_collision_rects(
  const Bullet& bullet,
  const std::vector<Rect>& static_rects
);

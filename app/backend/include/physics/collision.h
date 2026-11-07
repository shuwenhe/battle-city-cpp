#pragma once                        // 防止头文件被多次包含

#include "common/types.h"                  // 包含基本类型定义
#include "physics/geometry.h"               // 包含几何工具函数
#include <vector>                   // 包含动态数组容器
#include <optional>                 // 包含可选值容器

// ============================================================================
// 碰撞系统数据结构 - 世界中的地形和物体
// ============================================================================

/**
 * 地形地图接口(最小化设计用于碰撞检测)
 */
class TerrainMap {               // 定义地形地图基类
public:                          // 公开部分
  virtual ~TerrainMap() = default;  // 虚析构函数(支持多态删除)
  
  // 碰撞测试方法(纯虚函数由子类实现)
  virtual bool collide_brick(const Rect& rect, f32 threshold = -0.01f) const = 0;  // 砖墙碰撞检测
  virtual bool collide_steel(const Rect& rect, f32 threshold = -0.01f) const = 0;  // 钢墙碰撞检测
  virtual bool collide_river(const Rect& rect, f32 threshold = -0.01f) const = 0;  // 河流碰撞检测
  
  // 老鹰位置查询
  virtual std::optional<Point> get_eagle() const = 0;  // 获取老鹰位置(可能不存在)
};

/**
 * 坦克接口(最小化设计用于碰撞检测)
 */
class Tank {                     // 定义坦克基类
public:                          // 公开部分
  virtual ~Tank() = default;     // 虚析构函数(支持多态删除)
  
  u32 tank_id;                   // 坦克唯一ID
  bool alive;                    // 坦克是否活着
  Direction direction;           // 坦克朝向(上下左右)
  f32 x;                         // 坦克中心X坐标
  f32 y;                         // 坦克中心Y坐标
  f32 rx;                        // 保留位置X坐标(上一帧位置)
  f32 ry;                        // 保留位置Y坐标(上一帧位置)
  
  Rect rect() const {            // 获取坦克的碰撞矩形
    return {x, y, TANK_SIZE, TANK_SIZE};  // 返回以中心为起点的正方形矩形
  }
};

/**
 * 碰撞查询的世界视图结构体
 */
struct CollisionWorld {          // 定义碰撞世界结构体
  TerrainMap* map;               // 指向地形地图的指针
  std::vector<Tank*> tanks;      // 游戏中所有坦克的动态数组
  std::vector<Rect> restricted_areas;  // 受限制区域数组(正在生成的坦克位置)
};

// ============================================================================
// 碰撞检测函数 - 坦克移动和碰撞验证
// ============================================================================

/**
 * 检查矩形是否在游戏场地边界内
 * @param rect 待检查的矩形
 * @return 如果矩形完全在边界内返回true
 */
inline bool is_in_field(const Rect& rect) {  // 定义边界检查函数
  const f32 FIELD_SIZE = 416.0f;  // 常量:游戏场地大小(13个方格*32像素)
  return rect.x >= 0 &&           // 检查矩形左边界是否>=0
         rect.x + rect.w <= FIELD_SIZE &&  // 检查矩形右边界是否<=416
         rect.y >= 0 &&           // 检查矩形上边界是否>=0
         rect.y + rect.h <= FIELD_SIZE;    // 检查矩形下边界是否<=416
}

/**
 * 检查目标矩形是否在坦克的前方(基于坦克朝向)
 * @param other 目标矩形
 * @param tank 坦克对象(包含朝向信息)
 * @return 如果目标在前方返回true
 */
inline bool is_in_front(const Rect& other, const Tank& tank) {  // 定义前方检查函数
  switch (tank.direction) {      // 根据坦克朝向分类处理
    case Direction::Left:        // 坦克向左时
      return other.x < tank.x;   // 检查目标是否在坦克左侧
    case Direction::Right:       // 坦克向右时
      return other.x > tank.x;   // 检查目标是否在坦克右侧
    case Direction::Up:          // 坦克向上时
      return other.y < tank.y;   // 检查目标是否在坦克上侧
    case Direction::Down:        // 坦克向下时
      return other.y > tank.y;   // 检查目标是否在坦克下侧
    default:                     // 未知方向
      return false;              // 返回false表示未知状态
  }
}

/**
 * 检查坦克是否能够移动到当前位置
 * 
 * 检查项目包括:
 * - 场地边界检查
 * - 老鹰位置碰撞
 * - 砖墙碰撞检测
 * - 钢墙碰撞检测
 * - 河流碰撞检测
 * - 受限制区域碰撞(正在生成的坦克)
 * - 其他活跃坦克碰撞(仅检查前方)
 * 
 * @param world 碰撞世界数据结构
 * @param tank 待检查的坦克
 * @param threshold 碰撞阈值(默认-0.01允许微小重叠以获得自然感)
 * @return 如果坦克能够移动返回true,被阻挡返回false
 */
bool can_tank_move(const CollisionWorld& world, const Tank& tank, f32 threshold = -0.01f);  // 声明坦克移动检查函数

/**
 * 验证坦克位置是否在世界边界内有效
 * @param tank 待验证的坦克
 * @return 如果位置有效返回true
 */
bool is_valid_tank_position(const Tank& tank);  // 声明位置验证函数

/**
 * 获取与矩形碰撞的所有坦克列表
 * @param world 碰撞世界数据
 * @param rect 碰撞矩形
 * @return 与矩形碰撞的坦克指针动态数组
 */
std::vector<Tank*> get_colliding_tanks(const CollisionWorld& world, const Rect& rect);  // 声明坦克碰撞查询函数


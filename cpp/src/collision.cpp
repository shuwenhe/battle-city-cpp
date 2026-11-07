#include "collision.h"  // 包含碰撞检测头文件

/**
 * 检查坦克是否能够移动到当前位置
 * 检查所有可能的碰撞障碍物
 */
bool can_tank_move(const CollisionWorld& world, const Tank& tank, f32 threshold) {  // 坦克移动检查函数
  Rect tank_rect = tank.rect();  // 获取坦克的碰撞矩形
  
  // 检查场地边界
  if (!is_in_field(tank_rect)) {  // 如果坦克不在场地内
    return false;                 // 返回false表示不能移动
  }
  
  // 检查与地形地图的碰撞
  if (world.map != nullptr) {     // 如果地图指针不为空
    auto eagle = world.map->get_eagle();  // 获取老鹰位置
    if (eagle.has_value()) {      // 如果老鹰存在
      Rect eagle_rect = {         // 构建老鹰矩形
        eagle->x,                 // 老鹰X坐标
        eagle->y,                 // 老鹰Y坐标
        BLOCK_SIZE,               // 老鹰宽度(一个方格大小)
        BLOCK_SIZE                // 老鹰高度(一个方格大小)
      };
      if (test_rect_collide(eagle_rect, tank_rect)) {  // 如果坦克与老鹰碰撞
        return false;             // 返回false表示不能移动
      }
    }
    
    // 检查砖墙、钢墙、河流碰撞
    if (world.map->collide_brick(tank_rect, threshold) ||  // 如果与砖墙碰撞
        world.map->collide_steel(tank_rect, threshold) ||  // 或与钢墙碰撞
        world.map->collide_river(tank_rect, threshold)) {  // 或与河流碰撞
      return false;               // 返回false表示不能移动
    }
  }
  
  // 检查受限制区域(正在生成的坦克位置)
  for (const auto& area : world.restricted_areas) {  // 遍历所有受限制区域
    if (test_rect_collide(area, tank_rect)) {  // 如果坦克与受限制区域碰撞
      return false;               // 返回false表示不能移动
    }
  }
  
  // 检查其他坦克碰撞(仅检查前方的坦克)
  for (auto other : world.tanks) {  // 遍历世界中的所有坦克
    if (other == &tank || !other->alive) {  // 如果是自己或其他坦克已死亡
      continue;                   // 跳过此坦克
    }
    
    Rect other_rect = {           // 构建其他坦克的碰撞矩形
      other->rx,                  // 其他坦克上一帧X坐标
      other->ry,                  // 其他坦克上一帧Y坐标
      TANK_SIZE,                  // 坦克宽度
      TANK_SIZE                   // 坦克高度
    };
    
    if (is_in_front(other_rect, tank) && test_rect_collide(other_rect, tank_rect)) {  // 如果其他坦克在前方且碰撞
      return false;               // 返回false表示不能移动
    }
  }
  
  return true;                    // 返回true表示坦克可以移动
}

/**
 * 验证坦克位置是否在世界边界内有效
 */
bool is_valid_tank_position(const Tank& tank) {  // 位置验证函数
  Rect tank_rect = tank.rect();  // 获取坦克的碰撞矩形
  return is_in_field(tank_rect);  // 检查坦克是否在场地内并返回结果
}

/**
 * 获取与给定矩形碰撞的所有活跃坦克列表
 */
std::vector<Tank*> get_colliding_tanks(const CollisionWorld& world, const Rect& rect) {  // 碰撞坦克查询函数
  std::vector<Tank*> result;      // 创建结果数组
  
  for (auto tank : world.tanks) {  // 遍历世界中的所有坦克
    if (tank->alive && test_rect_collide(tank->rect(), rect)) {  // 如果坦克活着且与给定矩形碰撞
      result.push_back(tank);     // 将坦克指针添加到结果数组
    }
  }
  
  return result;                  // 返回所有碰撞的坦克列表
}


#include "bullet-collision.h"  // 包含子弹碰撞头文件
#include <cmath>                    // 包含数学库(最小值最大值等)
#include <algorithm>                // 包含算法库(std::min, std::max等)

// ============================================================================
// 方向旋转映射 - 用于将坐标系转换为以UP方向为基准
// ============================================================================
const Direction ROTATE_DIR[] = {    // 创建方向旋转常量数组
  Direction::Right,                 // UP方向旋转得到RIGHT(顺时针90°)
  Direction::Down,                  // DOWN方向旋转(未使用)
  Direction::Left,                  // LEFT方向旋转(未使用)
  Direction::Up,                    // RIGHT方向旋转(未使用)
};

/**
 * 将方向顺时针旋转90度
 * @param dir 原始方向
 * @return 旋转后的方向
 */
Direction rotate_direction(Direction dir) {  // 定义方向旋转函数
  switch (dir) {                    // 根据方向进行分支处理
    case Direction::Up: return Direction::Right;    // UP旋转90°变成RIGHT
    case Direction::Right: return Direction::Down;  // RIGHT旋转90°变成DOWN
    case Direction::Down: return Direction::Left;   // DOWN旋转90°变成LEFT
    case Direction::Left: return Direction::Up;     // LEFT旋转90°变成UP
    default: return dir;            // 未知方向返回原值
  }
}

// ============================================================================
// 最小包围矩形(MBR)计算 - 用于快速碰撞检测优化
// ============================================================================

/**
 * 计算包含所有给定矩形的最小矩形
 * 用于连续碰撞检测中的广泛阶段优化
 * @param rects 矩形数组
 * @return 包含所有矩形的最小矩形
 */
Rect get_mbr(const std::vector<Rect>& rects) {  // 定义MBR计算函数
  f32 left = 1e6f;                 // 初始化左边界为极大值
  f32 top = 1e6f;                  // 初始化上边界为极大值
  f32 right = -1e6f;               // 初始化右边界为极小值
  f32 bottom = -1e6f;              // 初始化下边界为极小值
  
  for (const auto& r : rects) {    // 遍历所有矩形
    left = std::min(left, r.x);    // 更新左边界为最小的x值
    top = std::min(top, r.y);      // 更新上边界为最小的y值
    right = std::max(right, r.x + r.w);  // 更新右边界为最大的x+w值
    bottom = std::max(bottom, r.y + r.h);  // 更新下边界为最大的y+h值
  }
  
  return {left, top, right - left, bottom - top};  // 返回计算出的最小包围矩形
}

// ============================================================================
// 子弹爆炸扩散 - 计算爆炸范围
// ============================================================================

/**
 * 计算子弹爆炸的扩散区域
 * 垂直于飞行方向扩展以扩大对墙的伤害范围
 * @param bullet 子弹对象
 * @return 扩散后的爆炸范围矩形
 */
Rect spread_bullet(const Bullet& bullet) {  // 定义子弹扩散计算函数
  Rect r = bullet.rect();          // 获取子弹的碰撞矩形
  f32 v = BULLET_EXPLOSION_SPREAD + BULLET_EXPLOSION_THRESHOLD;  // 计算扩散距离
  
  if (bullet.direction == Direction::Up || bullet.direction == Direction::Down) {  // 如果子弹向上或向下
    r.x -= v;                      // 从左侧扩展v单位
    r.w += 2.0f * v;               // 宽度增加2v(左右各扩展v)
    r.y -= BULLET_EXPLOSION_THRESHOLD;  // 从上侧稍微缩小
    r.h += BULLET_EXPLOSION_THRESHOLD;  // 高度稍微增加(垂直方向最小扩展)
  } else {                          // 子弹向左或向右
    r.x -= BULLET_EXPLOSION_THRESHOLD;  // 从左侧稍微缩小
    r.w += 2.0f * BULLET_EXPLOSION_THRESHOLD;  // 宽度稍微增加(水平方向最小扩展)
    r.y -= v;                      // 从上侧扩展v单位
    r.h += 2.0f * v;               // 高度增加2v(上下各扩展v)
  }
  
  return r;                         // 返回扩散后的爆炸范围
}

// ============================================================================
// 碰撞时间计算 - 计算两颗子弹何时碰撞
// ============================================================================

/**
 * 计算两颗子弹的精确碰撞时间
 * 基于它们的位置、速度和方向进行数学计算
 * 所有计算都假设b1的方向为UP(向上)
 * @param b1 第一颗子弹
 * @param b2 第二颗子弹
 * @return 碰撞时间(毫秒),如果不会碰撞返回-1
 */
f32 calculate_hit_time(const Bullet& b1, const Bullet& b2) {  // 定义碰撞时间计算函数
  // 所有计算都假设b1的方向为UP(向上)
  if (b1.direction != Direction::Up) {  // 如果b1不向上
    // 需要将坐标系旋转使b1方向变为UP
    // 为简化实现,这里只在主情况下实现完整算法
    return -1.0f;                  // TODO: 实现坐标旋转
  }
  
  if (b2.direction == Direction::Down) {  // 如果b2向下(与b1迎面)
    // 迎面碰撞情况
    f32 relative_speed = b1.speed + b2.speed;  // 计算相对速度(两个速度相加)
    if (relative_speed < 0.01f) return -1.0f;  // 如果相对速度过小则不碰撞
    return (b1.last_y - b2.last_y - BULLET_SIZE) / relative_speed;  // 计算碰撞时间
  } else if (b2.direction == Direction::Up) {  // 如果b2向上(与b1同向)
    // 同向运动情况
    if (abs_val(b1.last_y - b2.last_y) < 0.01f) {  // 如果两颗子弹Y坐标相同
      return -1.0f;               // 平行运动,不会碰撞
    }
    
    if (b1.last_y < b2.last_y) {   // 如果b1在b2上方
      f32 relative_speed = b2.speed - b1.speed;  // b2比b1快的速度
      if (relative_speed < 0.01f) return -1.0f;  // 如果b2不比b1快则不会追上
      return (b2.last_y - b1.last_y - BULLET_SIZE) / relative_speed;  // 计算追上时间
    } else {                       // b1在b2下方
      return calculate_hit_time(b2, b1);  // 交换参数重新计算
    }
  } else if (b2.direction == Direction::Left) {  // 如果b2向左(垂直碰撞)
    // 垂直碰撞情况(b1向上,b2向左)
    Rect hit_area = {b1.x, b2.y, BULLET_SIZE, BULLET_SIZE};  // 定义碰撞区域
    
    // 计算碰撞时间1(b1运动到b2的Y坐标)
    f32 time1 = (b1.last_y - b2.last_y - BULLET_SIZE) / b1.speed;  // b1到达b2高度的时间
    if (time1 >= 0) {              // 如果时间有效(不是负数)
      f32 b2_x_at_time1 = b2.last_x - b2.speed * time1;  // 计算此时b2的X坐标
      Rect b2_rect = {b2_x_at_time1, b2.y, BULLET_SIZE, BULLET_SIZE};  // b2在该时刻的矩形
      if (test_rect_collide(hit_area, b2_rect)) {  // 检查是否碰撞
        return time1;              // 返回碰撞时间
      }
    }
    
    // 计算碰撞时间2(b2运动到b1的X坐标)
    f32 time2 = (b2.last_x - b1.last_x - BULLET_SIZE) / b2.speed;  // b2到达b1位置的时间
    if (time2 >= 0) {              // 如果时间有效(不是负数)
      f32 b1_y_at_time2 = b1.last_y - b1.speed * time2;  // 计算此时b1的Y坐标
      Rect b1_rect = {b1.x, b1_y_at_time2, BULLET_SIZE, BULLET_SIZE};  // b1在该时刻的矩形
      if (test_rect_collide(hit_area, b1_rect)) {  // 检查是否碰撞
        return time2;              // 返回碰撞时间
      }
    }
    
    return -1.0f;                  // 没有碰撞发生
  } else {                          // b2.direction == RIGHT (垂直碰撞,b2向右)
    // 垂直碰撞情况(b1向上,b2向右)
    Rect hit_area = {b1.x, b2.y, BULLET_SIZE, BULLET_SIZE};  // 定义碰撞区域
    
    // 计算碰撞时间1(b1运动到b2的Y坐标)
    f32 time1 = (b1.last_y - b2.last_y - BULLET_SIZE) / b1.speed;  // b1到达b2高度的时间
    if (time1 >= 0) {              // 如果时间有效(不是负数)
      f32 b2_x_at_time1 = b2.last_x + b2.speed * time1;  // 计算此时b2的X坐标(向右移动)
      Rect b2_rect = {b2_x_at_time1, b2.y, BULLET_SIZE, BULLET_SIZE};  // b2在该时刻的矩形
      if (test_rect_collide(hit_area, b2_rect)) {  // 检查是否碰撞
        return time1;              // 返回碰撞时间
      }
    }
    
    // 计算碰撞时间2(b2运动到b1的X坐标)
    f32 time2 = (b1.last_x - b2.last_x - BULLET_SIZE) / b2.speed;  // b2到达b1位置的时间
    if (time2 >= 0) {              // 如果时间有效(不是负数)
      f32 b1_y_at_time2 = b1.last_y - b1.speed * time2;  // 计算此时b1的Y坐标
      Rect b1_rect = {b1.x, b1_y_at_time2, BULLET_SIZE, BULLET_SIZE};  // b1在该时刻的矩形
      if (test_rect_collide(hit_area, b1_rect)) {  // 检查是否碰撞
        return time2;              // 返回碰撞时间
      }
    }
    
    return -1.0f;                  // 没有碰撞发生
  }
}

// ============================================================================
// 运动计算 - 根据经过的时间计算子弹位置
// ============================================================================

/**
 * 计算子弹经过指定时间后从上一帧位置移动到的新位置
 * @param bullet 子弹对象
 * @param time_ms 经过的时间(毫秒)
 * @return 子弹在新时刻的位置
 */
Point move_from_last(const Bullet& bullet, f32 time_ms) {  // 定义位置计算函数
  f32 distance = bullet.speed * time_ms;  // 计算子弹移动的距离
  
  switch (bullet.direction) {      // 根据子弹方向分别计算新位置
    case Direction::Up:            // 向上移动
      return {bullet.last_x, bullet.last_y - distance};  // Y坐标减少(向上)
    case Direction::Down:          // 向下移动
      return {bullet.last_x, bullet.last_y + distance};  // Y坐标增加(向下)
    case Direction::Left:          // 向左移动
      return {bullet.last_x - distance, bullet.last_y};  // X坐标减少(向左)
    case Direction::Right:         // 向右移动
      return {bullet.last_x + distance, bullet.last_y};  // X坐标增加(向右)
    default:                       // 未知方向
      return {bullet.last_x, bullet.last_y};  // 返回上一帧位置
  }
}

// ============================================================================
// 碰撞检测 - 检查两颗子弹是否在一帧内碰撞
// ============================================================================

/**
 * 检查两颗子弹在当前帧内是否碰撞
 * 使用两阶段方法: 先做MBR快速检测,再计算精确碰撞时间
 * @param b1 第一颗子弹
 * @param b2 第二颗子弹
 * @param delta_ms 帧时间间隔(毫秒)
 * @return 碰撞结果指针,如果无碰撞返回nullptr
 */
const BulletCollisionResult* get_bullet_collision(  // 定义碰撞检测函数
  const Bullet& b1,                // 第一颗子弹参数
  const Bullet& b2,                // 第二颗子弹参数
  f32 delta_ms                     // 帧时间间隔(毫秒)
) {
  // 第一步: MBR快速检查(初级阶段)
  if (!test_rect_collide(bullet_mbr(b1), bullet_mbr(b2))) {  // 如果运动路径MBR不重叠
    return nullptr;                // 返回nullptr表示肯定不碰撞
  }
  
  // 第二步: 计算精确碰撞时间(精细阶段)
  f32 hit_time = calculate_hit_time(b1, b2);  // 调用碰撞时间计算函数
  
  if (hit_time >= 0 && hit_time <= delta_ms) {  // 如果碰撞时间有效且在当前帧内
    static BulletCollisionResult result;  // 创建静态结果对象(避免重复分配)
    result.hit = true;              // 标记发生碰撞
    result.point1 = move_from_last(b1, hit_time);  // 计算b1的碰撞位置
    result.point2 = move_from_last(b2, hit_time);  // 计算b2的碰撞位置
    return &result;                 // 返回结果指针
  }
  
  return nullptr;                  // 没有碰撞发生
}

// ============================================================================
// 爆炸位置计算 - 根据碰撞矩形确定爆炸发生的位置
// ============================================================================

/**
 * 从碰撞矩形计算爆炸位置
 * 根据子弹飞行方向和与什么障碍物碰撞来确定爆炸的确切位置
 * @param bullet 子弹对象
 * @param rects 与子弹碰撞的矩形数组(障碍物位置)
 * @return 爆炸发生的位置坐标
 */
Point explosion_pos_from_rects(const Bullet& bullet, const std::vector<Rect>& rects) {  // 定义爆炸位置计算函数
  switch (bullet.direction) {      // 根据子弹方向分别处理
    case Direction::Right: {        // 子弹向右飞行
      f32 left = 1e6f;              // 初始化左边界为极大值
      for (const auto& r : rects) {  // 遍历所有碰撞矩形
        left = std::min(left, r.x);  // 找到最左的矩形边界
      }
      return {left - BULLET_SIZE, bullet.y};  // 爆炸位置在矩形左侧
    }
    case Direction::Left: {         // 子弹向左飞行
      f32 right = -1e6f;            // 初始化右边界为极小值
      for (const auto& r : rects) {  // 遍历所有碰撞矩形
        right = std::max(right, r.x + r.w);  // 找到最右的矩形边界
      }
      return {right, bullet.y};     // 爆炸位置在矩形右侧
    }
    case Direction::Up: {           // 子弹向上飞行
      f32 bottom = -1e6f;           // 初始化下边界为极小值
      for (const auto& r : rects) {  // 遍历所有碰撞矩形
        bottom = std::max(bottom, r.y + r.h);  // 找到最下的矩形边界
      }
      return {bullet.x, bottom};    // 爆炸位置在矩形下侧
    }
    case Direction::Down: {         // 子弹向下飞行
      f32 top = 1e6f;               // 初始化上边界为极大值
      for (const auto& r : rects) {  // 遍历所有碰撞矩形
        top = std::min(top, r.y);   // 找到最上的矩形边界
      }
      return {bullet.x, top - BULLET_SIZE};  // 爆炸位置在矩形上侧
    }
    default:                        // 未知方向
      return {bullet.x, bullet.y};  // 返回子弹当前位置
  }
}

/**
 * 计算与游戏场地边界碰撞的矩形
 * @param field_size 游戏场地大小
 * @return 边界碰撞矩形
 */
Rect border_collision_rect(f32 field_size) {  // 定义边界碰撞矩形计算函数
  return {field_size, field_size, -field_size, -field_size};  // 返回表示边界的特殊矩形
}

/**
 * 获取与子弹运动路径碰撞的所有静态矩形
 * 用于优化碰撞检测,只检查相关的障碍物
 * @param bullet 子弹对象
 * @param static_rects 场景中所有静态矩形(障碍物)
 * @return 与子弹运动路径碰撞的矩形数组
 */
std::vector<Rect> get_bullet_collision_rects(  // 定义碰撞矩形查询函数
  const Bullet& bullet,             // 子弹参数
  const std::vector<Rect>& static_rects  // 静态矩形数组参数
) {
  std::vector<Rect> result;         // 创建结果数组
  Rect bullet_path = bullet_mbr(bullet);  // 计算子弹的运动路径MBR
  
  for (const auto& r : static_rects) {  // 遍历所有静态矩形
    if (test_rect_collide(bullet_path, r)) {  // 如果该矩形与子弹路径MBR重叠
      result.push_back(r);          // 将此矩形添加到结果数组
    }
  }
  
  return result;                    // 返回所有碰撞的矩形
}

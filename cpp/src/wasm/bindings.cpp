#include <emscripten/emscripten.h>  // 包含Emscripten C++绑定库
#include <emscripten/bind.h>       // 包含Emscripten绑定宏和函数

#include "../include/common/types.h"                 // 包含基本类型定义
#include "../include/physics/geometry.h"              // 包含几何工具函数
#include "../include/physics/collision.h"             // 包含碰撞检测函数
#include "../include/physics/bullet-collision.h"      // 包含子弹碰撞系统
#include "../include/map/random.h"                // 包含随机数生成器

using namespace emscripten;        // 使用emscripten命名空间

// ============================================================================
// JavaScript交互的绑定 - 导出C++对象和函数到JavaScript
// ============================================================================

// 导出Rect结构体和其他类型到JavaScript
EMSCRIPTEN_BINDINGS(battle_engine) {  // 开始定义"battle_engine"绑定模块
  // 枚举类型导出
  enum_<Direction>("Direction")    // 导出Direction枚举到JavaScript
    .value("UP", Direction::UP)    // 导出UP值为0
    .value("DOWN", Direction::DOWN)  // 导出DOWN值为1
    .value("LEFT", Direction::LEFT)  // 导出LEFT值为2
    .value("RIGHT", Direction::RIGHT);  // 导出RIGHT值为3
  
  // 结构体导出 - Point(二维点)
  value_object<Point>("Point")     // 将Point结构体导出为JavaScript对象
    .field("x", &Point::x)         // 导出x字段
    .field("y", &Point::y);        // 导出y字段
  
  // 结构体导出 - Rect(矩形)
  value_object<Rect>("Rect")       // 将Rect结构体导出为JavaScript对象
    .field("x", &Rect::x)          // 导出x字段(矩形左上角X坐标)
    .field("y", &Rect::y)          // 导出y字段(矩形左上角Y坐标)
    .field("w", &Rect::w)          // 导出w字段(矩形宽度)
    .field("h", &Rect::h);         // 导出h字段(矩形高度)
  
  // 结构体导出 - CollisionResult(碰撞检测结果)
  value_object<CollisionResult>("CollisionResult")  // 将CollisionResult导出为JavaScript对象
    .field("collided", &CollisionResult::collided)  // 导出碰撞标志
    .field("overlap_x", &CollisionResult::overlap_x)  // 导出X轴重叠距离
    .field("overlap_y", &CollisionResult::overlap_y);  // 导出Y轴重叠距离
  
  // 结构体导出 - BulletHitPoint(子弹击中点)
  value_object<BulletHitPoint>("BulletHitPoint")  // 将BulletHitPoint导出为JavaScript对象
    .field("x", &BulletHitPoint::x)  // 导出击中点X坐标
    .field("y", &BulletHitPoint::y);  // 导出击中点Y坐标
  
  // 结构体导出 - Bullet(子弹状态)
  value_object<Bullet>("Bullet")   // 将Bullet结构体导出为JavaScript对象
    .field("bullet_id", &Bullet::bullet_id)  // 导出子弹ID
    .field("x", &Bullet::x)         // 导出当前X坐标
    .field("y", &Bullet::y)         // 导出当前Y坐标
    .field("last_x", &Bullet::last_x)  // 导出上一帧X坐标
    .field("last_y", &Bullet::last_y)  // 导出上一帧Y坐标
    .field("direction", &Bullet::direction)  // 导出子弹方向
    .field("speed", &Bullet::speed)  // 导出子弹速度
    .field("tank_id", &Bullet::tank_id)  // 导出发射坦克ID
    .field("fired_at", &Bullet::fired_at);  // 导出发射时刻时间戳
  
  // 几何工具函数导出
  function("distance", &distance);  // 导出距离计算函数
  function("distance_sq", &distance_sq);  // 导出距离平方计算函数
  function("clamp", select_overload<f32(f32, f32, f32)>(&clamp<f32>));  // 导出数值限制函数
  function("lerp", &lerp);         // 导出线性插值函数
  function("test_rect_collide", &test_rect_collide);  // 导出矩形碰撞检测
  function("point_in_rect", &point_in_rect);  // 导出点在矩形内检测
  function("test_circle_collide", &test_circle_collide);  // 导出圆形碰撞检测
  function("test_circle_rect_collide", &test_circle_rect_collide);  // 导出圆矩形碰撞检测
  function("rect_from_center", &rect_from_center);  // 导出从中心创建矩形
  function("rect_center", &rect_center);  // 导出获取矩形中心
  function("rect_offset", &rect_offset);  // 导出矩形偏移
  function("get_aabb_overlap", &get_aabb_overlap);  // 导出AABB重叠计算
  function("direction_to_velocity", &direction_to_velocity);  // 导出方向转速度向量
  function("opposite_direction", &opposite_direction);  // 导出相反方向计算
  
  // 子弹碰撞系统函数导出
  function("bullet_mbr", &bullet_mbr);  // 导出子弹最小包围矩形计算
  function("spread_bullet", &spread_bullet);  // 导出子弹扩散计算
  function("calculate_hit_time", &calculate_hit_time);  // 导出碰撞时间计算
  function("move_from_last", &move_from_last);  // 导出从上一帧移动计算
  function("explosion_pos_from_rects", select_overload<  // 导出爆炸位置计算(重载版本1)
    Point(const Bullet&, const std::vector<Rect>&)  // 选择接收子弹和矩形数组的版本
  >(&explosion_pos_from_rects));
  function("border_collision_rect", &border_collision_rect);  // 导出边界碰撞矩形
  
  // 随机数生成器类导出
  class_<SeededRandom>("SeededRandom")  // 将SeededRandom类导出为JavaScript类
    .constructor<u32>()            // 导出构造函数(接收u32种子)
    .function("next", &SeededRandom::next)  // 导出next方法
    .function("next_int", &SeededRandom::next_int)  // 导出next_int方法
    .function("next_float", &SeededRandom::next_float)  // 导出next_float方法
    .function("get_state", &SeededRandom::get_state);  // 导出get_state方法
  
  // 随机数初始化函数导出
  function("init_random", &init_random);  // 导出随机数全局初始化函数
}

// ============================================================================
// C接口 - 用于WASM的原始C风格函数(可选,用于高性能路径)
// ============================================================================

extern "C" {                       // 声明C链接(避免C++名称修饰)
  // 引擎初始化函数
  void engine_init(u32 random_seed) {  // C函数:初始化游戏引擎
    init_random(random_seed);      // 调用随机数初始化函数
  }
  
  // 几何计算 - 矩形碰撞测试
  bool geom_test_rect_collide(f32 ax, f32 ay, f32 aw, f32 ah,  // C函数:测试矩形碰撞
                              f32 bx, f32 by, f32 bw, f32 bh) {  // 参数:两个矩形的位置和大小
    Rect a = {ax, ay, aw, ah};    // 构建第一个矩形对象
    Rect b = {bx, by, bw, bh};    // 构建第二个矩形对象
    return test_rect_collide(a, b);  // 调用C++碰撞检测并返回结果
  }
  
  // 距离计算 - 两点间的欧几里得距离
  f32 geom_distance(f32 x1, f32 y1, f32 x2, f32 y2) {  // C函数:计算两点距离
    return distance({x1, y1}, {x2, y2});  // 构建Point对象并调用distance函数
  }
  
  // 距离平方 - 用于比较时避免开方计算
  f32 geom_distance_sq(f32 x1, f32 y1, f32 x2, f32 y2) {  // C函数:计算两点距离平方
    return distance_sq({x1, y1}, {x2, y2});  // 构建Point对象并调用distance_sq函数
  }
  
  // 子弹碰撞计算 - 计算两颗子弹的碰撞时间
  f32 bullet_calculate_hit_time(  // C函数:计算子弹碰撞时间
    f32 b1_x, f32 b1_y, f32 b1_last_x, f32 b1_last_y,  // 第一颗子弹:当前和上一帧坐标
    i32 b1_dir, f32 b1_speed,     // 第一颗子弹:方向和速度
    f32 b2_x, f32 b2_y, f32 b2_last_x, f32 b2_last_y,  // 第二颗子弹:当前和上一帧坐标
    i32 b2_dir, f32 b2_speed      // 第二颗子弹:方向和速度
  ) {
    Bullet b1 = {0, b1_x, b1_y, b1_last_x, b1_last_y,  // 构建第一颗子弹对象
                 static_cast<Direction>(b1_dir), b1_speed, 0, 0};  // 方向强制转换为枚举
    Bullet b2 = {0, b2_x, b2_y, b2_last_x, b2_last_y,  // 构建第二颗子弹对象
                 static_cast<Direction>(b2_dir), b2_speed, 0, 0};  // 方向强制转换为枚举
    return calculate_hit_time(b1, b2);  // 调用碰撞时间计算函数
  }
  
  // 随机数生成 - 初始化全局随机数生成器
  void random_init(u32 seed) {    // C函数:初始化随机数生成器
    init_random(seed);            // 调用随机数初始化函数
  }
  
  // 获取下一个随机浮点数(0-1之间)
  f32 random_next() {             // C函数:获取随机浮点数
    return g_random.next();       // 返回全局随机数生成器的下一个值
  }
  
  // 获取指定范围内的随机整数
  i32 random_next_int(i32 min, i32 max) {  // C函数:获取范围内随机整数
    return g_random.next_int(min, max);  // 返回指定范围内的随机整数
  }
}

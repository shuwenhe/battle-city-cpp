// ============================================================================
// game-types.h - 游戏核心数据类型和常量
// 
// 本文件定义了 Battle City 游戏引擎的基础类型、枚举和数据结构。
// 包括坦克、子弹、地形、道具等游戏对象的属性定义。
// ============================================================================

#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <cstdint>  // 用于 int32_t、uint32_t 等标准整数类型
#include <array>    // 用于 std::array
#include <string>   // 用于 std::string

// ============================================================================
// 基础类型别名
// ============================================================================

// 浮点数别名 (用于性能关键的物理计算，64 位精度)
using f32 = float;
using f64 = double;

// 整数别名 (标准化整数类型以保证跨平台一致性)
using i32 = int32_t;
using u32 = uint32_t;
using i16 = int16_t;
using u16 = uint16_t;
using u8 = uint8_t;

// ============================================================================
// 方向和运动
// ============================================================================

// 坦克四个运动方向的枚举 (原版 NES 支持 4 向)
enum class Direction : u8 {
  Up = 0,    // 向上移动 (小写值)
  Down = 1,  // 向下移动
  Left = 2,  // 向左移动
  Right = 3  // 向右移动
};

// 为了与现有代码兼容，额外定义大写别名
// （这些是枚举值而非别名，用于兼容）

// ============================================================================
// 坦克属性
// ============================================================================

// 坦克等级 (决定速度、子弹性能)
enum class TankLevel : u8 {
  Basic = 0,   // 基础坦克: 移速 0.03, 弹速 0.12, 威力 1, 弹数 1
  Fast = 1,    // 快速坦克: 移速 0.06, 弹速 0.12, 威力 1, 弹数 1
  Power = 2,   // 强力坦克: 移速 0.03, 弹速 0.24, 威力 2, 弹数 1
  Armor = 3    // 装甲坦克: 移速 0.045, 弹速 0.18, 威力 3, 弹数 2
};

// 坦克颜色方案 (4 种配色)
enum class TankColor : u8 {
  Yellow = 0,  // 黄色: 玩家 1 默认
  Green = 1,   // 绿色: 玩家 2 默认
  Silver = 2,  // 银色: bot 默认
  Red = 3      // 红色: 备用配色
};

// 坦克所属阵营 (玩家或敌人 AI)
enum class TankSide : u8 {
  Player = 0,  // 玩家坦克 (可被键盘或 AI 控制)
  Bot = 1      // 敌人坦克 (由 AI 控制)
};

// ============================================================================
// 地图和地形
// ============================================================================

// 地图方格类型 (13×13 block 的地形编码)
enum class MapItemType : u8 {
  Empty = 0,   // X: 空地
  Eagle = 1,   // E: 老鹰 (保护目标)
  Brick = 2,   // B: 砖块 (可摧毁，但 1 次子弹击中就毁)
  Steel = 3,   // T: 钢墙 (不可摧毁，除非子弹威力 >= 3)
  River = 4,   // R: 河流 (可通行，玩家会滑行)
  Snow = 5,    // S: 雪地 (可通行，玩家会滑行)
  Forest = 6   // F: 森林 (可通行，上面有树木不阻挡)
};

// ============================================================================
// 道具系统
// ============================================================================

// 6 种道具类型及其效果
enum class PowerUpName : u8 {
  Tank = 0,      // tank: 额外坦克 (+1 命)
  Star = 1,      // star: 升级坦克等级
  Grenade = 2,   // grenade: 炸掉所有 bot
  Timer = 3,     // timer: 冻结所有 bot
  Helmet = 4,    // helmet: 无敌时间内不受伤害
  Shovel = 5     // shovel: 老鹰周围变成钢墙
};

// ============================================================================
// 爆炸和动画
// ============================================================================

// 爆炸效果形状 (影响视觉反馈和音效)
enum class ExplosionShape : u8 {
  Small0 = 0,   // s0: 小爆炸形状 0 (子弹命中地形)
  Small1 = 1,   // s1: 小爆炸形状 1 (子弹命中坦克)
  Small2 = 2,   // s2: 小爆炸形状 2 (坦克碰撞)
  Big0 = 3,     // b0: 大爆炸形状 0 (坦克/道具销毁)
  Big1 = 4      // b1: 大爆炸形状 1 (特殊爆炸)
};

// 音效名称 (与 src/audio/sounds/*.ogg 对应)
enum class SoundName : u8 {
  StageStart = 0,     // stage_start: 关卡开始
  GameOver = 1,       // game_over: 游戏结束
  BulletShot = 2,     // bullet_shot: 开火音效
  BulletHit1 = 3,     // bullet_hit_1: 子弹命中 1
  BulletHit2 = 4,     // bullet_hit_2: 子弹命中 2
  Explosion1 = 5,     // explosion_1: 爆炸音 1
  Explosion2 = 6,     // explosion_2: 爆炸音 2
  Pause = 7,          // pause: 暂停音效
  PowerupAppear = 8,  // powerup_appear: 道具出现
  PowerupPick = 9,    // powerup_pick: 拾取道具
  Statistics1 = 10,   // statistics_1: 结算音效
  SnowSlide = 11      // snow_slide: 冰面滑行 (可选)
};

// ============================================================================
// 坦克配置数据
// ============================================================================

// bot 编队配置 (关卡中的敌人编队信息)
struct BotGroupConfig {
  TankLevel tankLevel;  // 该编队中 bot 的坦克等级
  u32 count;            // 该编队包含的 bot 数量
};

// ============================================================================
// 几何数据结构
// ============================================================================

// 二维点坐标 (通常为像素)
struct Point {
  f32 x;  // X 轴坐标
  f32 y;  // Y 轴坐标
};

// 矩形碰撞箱 (用于碰撞检测)
struct Rect {
  f32 x;       // 左上角 X 坐标
  f32 y;       // 左上角 Y 坐标
  f32 w;       // 矩形宽度
  f32 h;       // 矩形高度
};

// ============================================================================
// 游戏常量
// ============================================================================

namespace Constants {
  // 时间相关常量 (基于 60 Hz 原版 NES)
  constexpr f32 FRAME_MS = 1000.0f / 60.0f;  // 每帧毫秒数 (~16.667ms)
  constexpr f32 STEP_MS = 1000.0f / 60.0f;   // 逻辑步长 (固定步长游戏循环)
  
  // 地图相关常量 (像素尺寸和方格)
  constexpr i32 BLOCK_SIZE = 16;              // 一个方格的像素大小
  constexpr i32 FIELD_BLOCK_SIZE = 13;        // 战场的方格数 (13×13)
  constexpr i32 FIELD_SIZE = BLOCK_SIZE * FIELD_BLOCK_SIZE;  // 战场像素大小 (208×208)
  
  // 实体尺寸
  constexpr i32 TANK_SIZE = BLOCK_SIZE;  // 坦克碰撞箱大小 (16×16)
  constexpr i32 BULLET_SIZE = 3;         // 子弹碰撞箱大小 (3×3)
  
  // 砖块精度 (4 个 4px 小格组成一个 16px 方格)
  constexpr i32 BRICK_ITEM_SIZE = 4;     // 砖块子格大小
  constexpr i32 BRICK_GRID_SIZE = FIELD_SIZE / BRICK_ITEM_SIZE;  // 砖块网格大小 (52×52)
  
  // 钢墙精度 (4 个 8px 小格组成一个 16px 方格)
  constexpr i32 STEEL_ITEM_SIZE = 8;     // 钢墙子格大小
  constexpr i32 STEEL_GRID_SIZE = FIELD_SIZE / STEEL_ITEM_SIZE;  // 钢墙网格大小 (26×26)
  
  // 地形宽度 (河、雪、森林按方格精度)
  constexpr i32 TERRAIN_GRID_SIZE = FIELD_BLOCK_SIZE;  // 13×13
  
  // 子弹相关
  constexpr i32 STEEL_POWER = 3;  // 摧毁钢墙所需最低威力
  
  // 坦克数值 (速度、间隔等)
  constexpr f32 PLAYER_MOVE_SPEED = 0.045f;         // 玩家坦克移速 (px/ms)
  constexpr f32 BOT_BASIC_MOVE_SPEED = 0.03f;       // bot 基础移速
  constexpr f32 BOT_FAST_MOVE_SPEED = 0.06f;        // bot 快速移速
  
  constexpr f32 PLAYER_BASIC_BULLET_SPEED = 0.12f;  // 玩家基础子弹速度
  constexpr f32 PLAYER_POWER_BULLET_SPEED = 0.18f;  // 玩家强力子弹速度
  constexpr f32 BOT_POWER_BULLET_SPEED = 0.24f;     // bot 强力子弹速度
  
  constexpr i32 PLAYER_BASIC_BULLET_INTERVAL = 300;  // 玩家基础开火间隔 (ms)
  constexpr i32 PLAYER_POWER_BULLET_INTERVAL = 200;  // 玩家强力开火间隔
  constexpr i32 BOT_BULLET_INTERVAL = 200;           // bot 开火间隔
  
  // 玩家生命相关
  constexpr i32 INITIAL_LIVES = 3;                  // 初始命数
  constexpr i32 LIFE_BONUS_SCORE = 10000;           // 每累计 10000 分 +1 命
  constexpr i32 SPAWN_HELMET_DURATION_MS = 2250;   // 出生无敌时间 (135 帧 = 2250ms)
  
  // bot 出生相关
  constexpr i32 MAX_BOT_ON_FIELD_SINGLE = 4;   // 单人最多 bot 数
  constexpr i32 MAX_BOT_ON_FIELD_MULTI = 6;    // 双人最多 bot 数
  constexpr i32 BOT_FLICKER_DURATION_MS = 933; // 出生闪烁时间 (56 帧)
  
  // 道具得分
  constexpr i32 TANK_KILL_SCORE_BASIC = 100;   // 击杀基础坦克得分
  constexpr i32 TANK_KILL_SCORE_FAST = 200;    // 击杀快速坦克得分
  constexpr i32 TANK_KILL_SCORE_POWER = 300;   // 击杀强力坦克得分
  constexpr i32 TANK_KILL_SCORE_ARMOR = 400;   // 击杀装甲坦克得分
  constexpr i32 POWER_UP_SCORE = 500;          // 拾取道具得分
  constexpr i32 STAR_ARMOR_SCORE = 5000;       // armor 拾取 star 额外得分
  
  // 托管和 AI 相关
  constexpr i32 AUTOPILOT_START_MS = 2000;  // 没按过键时, 2s 后托管
  constexpr i32 AUTOPILOT_IDLE_MS = 5000;   // 按过键后, 5s 空闲托管
  constexpr i32 BLOCK_TIMEOUT_MS = 200;     // AI 卡住判定超时
  constexpr f32 BLOCK_DISTANCE_THRESHOLD = 0.01f;  // AI 卡住距离阈值
  
  // 特殊效果时长 (毫秒)
  constexpr i32 SCORE_POPUP_DURATION_MS = 800;       // 击杀弹出文字时长 (48 帧)
  constexpr i32 HELMET_POWERUP_DURATION_MS = 10500;  // helmet 道具时长 (630 帧)
  constexpr i32 SHOVEL_STEEL_DURATION_MS = 17933;    // shovel 变钢时长 (1076 帧)
  constexpr i32 SHOVEL_BLINK_INTERVAL_MS = 267;      // shovel 闪烁间隔 (16 帧)
  constexpr i32 SHOVEL_BLINK_TIMES = 6;              // shovel 闪烁次数
  
  // 冰面滑行 (玩家特定)
  constexpr i32 SNOW_SLIDE_START = 16;  // 初始滑行量 (px)
  constexpr i32 SNOW_SLIDE_LOCK = 8;    // 锁定阈值
  
  // timer 道具 (冻结 bot)
  constexpr i32 TIMER_FREEZE_TICKS = 10;         // 冻结计数次数
  constexpr i32 TIMER_FREEZE_TICK_MS = 1067;    // 每次递减间隔 (64 帧)
}

#endif  // GAME_TYPES_H

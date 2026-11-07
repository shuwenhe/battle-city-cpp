// ============================================================================
// game-state.h - 游戏对象的数据结构
// 
// 定义坦克、子弹、地形等游戏对象的状态数据结构。
// 这些是纯数据容器，不包含游戏逻辑，只用于存储和传递状态。
// ============================================================================

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "core/game-types.h"
#include <vector>
#include <optional>

// ============================================================================
// 坦克实体
// ============================================================================

// 坦克的完整状态 (可变对象，逐帧原地更新)
struct Tank {
  // 基本信息
  i32 tankId;           // 坦克唯一编号 (自增)
  TankSide side;        // 阵营: 玩家 or bot
  TankLevel level;      // 坦克等级: basic/fast/power/armor
  TankColor color;      // 配色方案
  
  // 位置和方向
  f32 x;                // 坦克左上角 X 坐标 (像素)
  f32 y;                // 坦克左上角 Y 坐标 (像素)
  Direction direction;  // 朝向: up/down/left/right
  
  // 物理状态
  f32 vx;               // X 轴速度分量 (px/ms)
  f32 vy;               // Y 轴速度分量 (px/ms)
  bool moving;          // 本帧是否发生平移 (驱动履带动画)
  
  // 转向对齐 (为垂直转向而预留的坐标)
  f32 rx;               // 预留 X 坐标 (沿当前轴对齐到 8 的倍数)
  f32 ry;               // 预留 Y 坐标
  
  // 冰面滑行 (仅玩家坦克)
  f32 slide;            // 剩余滑行量 (px), 离开冰面保留, 回到冰面继续
  
  // 状态效果时长 (毫秒, >0 表示该效果激活)
  f32 frozenTimeout;    // 冻结剩余时间 (队友命中/timer 道具)
  f32 cooldown;         // 开火冷却剩余时间
  f32 helmetDuration;   // 无敌头盔剩余时间
  
  // 生命值和状态
  i32 hp;               // 生命值 (通常为 1, armor 可能更高)
  bool alive;           // 是否活着 (false 时待移除)
  bool visible;         // 是否可见 (冻结时闪烁)
  
  // 道具相关
  bool withPowerUp;     // 该 bot 被击中时是否掉落道具
  
  // 时间戳
  f32 bornAt;           // 出生时的场景时间 (用于 bot 变色动画)
  
  // 构造函数 (初始化所有字段)
  Tank(i32 id, TankSide side_, TankLevel level_, TankColor color_,
       f32 x_, f32 y_, Direction dir = Direction::Up)
      : tankId(id), side(side_), level(level_), color(color_),
        x(x_), y(y_), direction(dir),
        vx(0.0f), vy(0.0f), moving(false),
        rx(x_), ry(y_),
        slide(0.0f),
        frozenTimeout(0.0f), cooldown(0.0f), helmetDuration(0.0f),
        hp(1), alive(true), visible(true),
        withPowerUp(false),
        bornAt(0.0f) {}
  
  // 获取坦克的碰撞矩形
  Rect getRect() const {
    return {x, y, static_cast<f32>(Constants::TANK_SIZE), 
            static_cast<f32>(Constants::TANK_SIZE)};
  }
};

// ============================================================================
// 子弹实体
// ============================================================================

// 子弹的完整状态 (可变对象)
struct Bullet {
  // 基本信息
  i32 bulletId;      // 子弹唯一编号
  TankSide side;     // 发射者阵营
  i32 tankId;        // 发射者坦克编号
  Direction direction;  // 飞行方向
  
  // 位置和速度
  f32 x;             // 子弹中心 X 坐标 (像素)
  f32 y;             // 子弹中心 Y 坐标 (像素)
  f32 lastX;         // 上一帧 X 坐标 (用于 MBR 连续碰撞检测)
  f32 lastY;         // 上一帧 Y 坐标
  f32 speed;         // 移动速度 (px/ms)
  
  // 属性
  i32 power;         // 子弹威力 (1/2/3, 3 可破钢)
  bool dead;         // 标记为待移除 (命中/对撞/出界)
  
  // 构造函数
  Bullet(i32 id, TankSide side_, i32 tankId_, Direction dir,
         f32 x_, f32 y_, f32 speed_, i32 power_)
      : bulletId(id), side(side_), tankId(tankId_), direction(dir),
        x(x_), y(y_), lastX(x_), lastY(y_),
        speed(speed_), power(power_), dead(false) {}
  
  // 获取子弹的碰撞矩形
  Rect getRect() const {
    return {x, y, static_cast<f32>(Constants::BULLET_SIZE), 
            static_cast<f32>(Constants::BULLET_SIZE)};
  }
  
  // 获取上一帧的碰撞矩形 (连续碰撞检测)
  Rect getLastRect() const {
    return {lastX, lastY, static_cast<f32>(Constants::BULLET_SIZE), 
            static_cast<f32>(Constants::BULLET_SIZE)};
  }
};

// ============================================================================
// 道具实体
// ============================================================================

// 道具的完整状态
struct PowerUp {
  // 基本信息
  PowerUpName name;  // 道具类型
  
  // 位置
  f32 x;             // 道具左上角 X 坐标
  f32 y;             // 道具左上角 Y 坐标
  
  // 状态
  bool visible;      // 是否可见 (闪烁动画用)
  f32 lifespan;      // 存活时间 (ms)
  
  // 构造函数
  PowerUp(PowerUpName name_, f32 x_, f32 y_)
      : name(name_), x(x_), y(y_), visible(true), lifespan(0.0f) {}
};

// ============================================================================
// 爆炸特效
// ============================================================================

// 爆炸的完整状态 (用于视觉反馈和音效)
struct Explosion {
  // 基本信息
  ExplosionShape shape;  // 爆炸形状
  
  // 位置
  f32 x;                 // 爆炸中心 X 坐标
  f32 y;                 // 爆炸中心 Y 坐标
  
  // 时间
  f32 duration;          // 爆炸总时长 (ms)
  f32 elapsed;           // 已经历时间
  
  // 构造函数
  Explosion(ExplosionShape shape_, f32 x_, f32 y_, f32 duration_)
      : shape(shape_), x(x_), y(y_), duration(duration_), elapsed(0.0f) {}
};

// ============================================================================
// 得分浮动文字
// ============================================================================

// 得分弹出的显示信息 (击杀后的分数提示)
struct ScorePopup {
  // 分数和位置
  i32 score;         // 显示的分数值
  f32 x;             // 弹出位置 X
  f32 y;             // 弹出位置 Y
  
  // 时间
  f32 duration;      // 总显示时长 (ms)
  f32 elapsed;       // 已经历时间
  
  // 构造函数
  ScorePopup(i32 score_, f32 x_, f32 y_, f32 duration_)
      : score(score_), x(x_), y(y_), duration(duration_), elapsed(0.0f) {}
};

// ============================================================================
// 闪烁效果 (出生时的闪烁)
// ============================================================================

// 闪烁的完整状态 (出生坦克的可见性动画)
struct Flicker {
  // 位置 (可能不同于实际坦克位置, 因为坦克还没被添加到列表)
  f32 x;                // 闪烁中心 X 坐标
  f32 y;                // 闪烁中心 Y 坐标
  
  // 时间
  f32 duration;         // 总闪烁时长 (ms)
  f32 elapsed;          // 已经历时间
  
  // 构造函数
  Flicker(f32 x_, f32 y_, f32 duration_)
      : x(x_), y(y_), duration(duration_), elapsed(0.0f) {}
};

// ============================================================================
// 地形地图
// ============================================================================

// 完整的地形数据 (包括砖块、钢墙、河、雪、森林和老鹰)
struct TerrainMap {
  // 砖块网格 (52×52, 每个 4px 单元)
  std::vector<bool> bricks;
  i32 brickGridSize;    // 52
  
  // 钢墙网格 (26×26, 每个 8px 单元)
  std::vector<bool> steels;
  i32 steelGridSize;    // 26
  
  // 河流、雪地、森林 (13×13, 按方格精度)
  std::vector<bool> rivers;
  std::vector<bool> snows;
  std::vector<bool> forests;
  i32 terrainGridSize;  // 13
  
  // 老鹰 (保护目标)
  std::optional<Point> eagle;
  bool eagleDestroyed;  // 老鹰是否已被击中
  
  // 构造函数 (初始化所有网格)
  TerrainMap()
      : brickGridSize(Constants::BRICK_GRID_SIZE),
        steelGridSize(Constants::STEEL_GRID_SIZE),
        terrainGridSize(Constants::TERRAIN_GRID_SIZE),
        eagleDestroyed(false) {
    // 初始化网格数组 (全 false, 表示空地)
    bricks.resize(brickGridSize * brickGridSize, false);
    steels.resize(steelGridSize * steelGridSize, false);
    rivers.resize(terrainGridSize * terrainGridSize, false);
    snows.resize(terrainGridSize * terrainGridSize, false);
    forests.resize(terrainGridSize * terrainGridSize, false);
  }
};

// ============================================================================
// 玩家状态 (跨关保留)
// ============================================================================

// 单个玩家在一局游戏中的跨关状态
struct PlayerState {
  // 基本信息
  i32 playerIndex;           // 玩家编号 (0 或 1)
  TankColor color;           // 坦克配色
  
  // 生命和分数
  i32 lives;                 // 剩余命数
  i32 score;                 // 累积得分
  
  // 坦克升级状态
  TankLevel reservedTankLevel;  // 上一关保留的坦克等级 (null 表示无)
  
  // AI 状态
  bool isAutopilot;          // 当前是否由 AI 驾驶
  f32 idleTimeMs;            // 连续没有按键的时间 (ms)
  bool touchedThisStage;     // 本关是否按过键
  
  // 出生点
  Point spawnPos;            // 该玩家的出生位置
  
  // 构造函数
  PlayerState(i32 index, TankColor color_, Point spawn)
      : playerIndex(index), color(color_), 
        lives(Constants::INITIAL_LIVES), score(0),
        reservedTankLevel(TankLevel::Basic),
        isAutopilot(false), idleTimeMs(0.0f), touchedThisStage(false),
        spawnPos(spawn) {}
};

// ============================================================================
// 场景快照 (诊断和测试用)
// ============================================================================

// 游戏场景的纯数据快照 (可直接序列化)
struct SceneSnapshot {
  // 时间
  f32 sceneTime;        // 场景已运行的逻辑时间 (ms)
  
  // 内容计数
  i32 tankCount;        // 当前坦克总数
  i32 bulletCount;      // 当前子弹总数
  i32 powerUpCount;     // 当前道具总数
  i32 explosionCount;   // 当前爆炸总数
  i32 remainingBots;    // 还没出生的 bot 数
  
  // 冻结状态
  bool botsFrozen;      // timer 道具是否正在冻结所有 bot
};

#endif  // GAME_STATE_H

// ============================================================================
// game-logic.h - 游戏逻辑系统接口
// 
// 定义游戏循环、坦克运动、子弹发射、碰撞检测、道具系统等核心逻辑接口。
// 这些函数是确定性的：相同输入必定产生相同输出。
// ============================================================================

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game-types.h"
#include "game-state.h"
#include <vector>
#include <optional>

// ============================================================================
// 坦克数值计算函数
// ============================================================================

// 获取坦克的移动速度 (px/ms)
f32 getTankMoveSpeed(const Tank& tank);

// 获取坦克的子弹速度 (px/ms)
f32 getTankBulletSpeed(const Tank& tank);

// 获取坦克的子弹威力 (1/2/3)
i32 getTankBulletPower(const Tank& tank);

// 获取坦克的开火间隔 (ms)
i32 getTankBulletInterval(const Tank& tank);

// 获取坦克的最大并发子弹数
i32 getTankBulletLimit(const Tank& tank);

// 获取击杀指定等级坦克的得分
i32 getKillScore(TankLevel level);

// ============================================================================
// 坦克运动系统
// ============================================================================

// 表示一帧内坦克的输入意图
struct TankIntent {
  // 方向改变 (如果为非空则转向，但不立即移动)
  std::optional<Direction> turnDirection;
  
  // 是否按下前进键 (true 则沿当前方向移动)
  bool moveForward;
  
  // 最大移动距离限制 (用于路径跟随) (-1 表示无限制)
  f32 maxDistance;
  
  // 默认构造函数 (无输入)
  TankIntent() : turnDirection(std::nullopt), moveForward(false), maxDistance(-1.0f) {}
};

// 检查坦克是否能在给定位置移动
// world: 当前世界状态 (其他坦克、地形等)
// tank: 要移动的坦克
// newX, newY: 新位置
// threshold: 碰撞容限 (允许的轻微穿透量)
bool canTankMove(const TerrainMap& terrain, const std::vector<Tank>& otherTanks,
                 Tank& tank, f32 newX, f32 newY, f32 threshold = 0.01f);

// 应用坦克移动输入，更新坦克位置
// dt: 时间增量 (ms)
// intent: 该帧的移动意图
void updateTankMovement(Tank& tank, const TankIntent& intent, f32 dt);

// 应用坦克转向，更新方向但不改变位置
void rotateTank(Tank& tank, Direction newDirection);

// ============================================================================
// 坦克发射系统
// ============================================================================

// 子弹初始化参数
struct BulletSpawnParams {
  i32 tankId;              // 发射者 ID
  TankSide side;           // 发射者阵营
  Direction direction;     // 发射方向
  f32 speed;               // 子弹速度
  i32 power;               // 子弹威力
  Point spawnPos;          // 子弹生成位置 (相对于坦克的偏移)
};

// 计算子弹的生成位置 (坦克前方)
Point calculateBulletSpawnPos(const Tank& tank);

// 尝试从坦克发射一颗子弹
// tank: 发射者
// currentBulletCount: 当前已发射的子弹数 (用于检查限制)
// returns: 如果发射成功，返回子弹初始化参数；否则返回空
std::optional<BulletSpawnParams> tryFireBullet(Tank& tank, i32 currentBulletCount);

// ============================================================================
// 子弹移动系统
// ============================================================================

// 更新子弹位置 (每帧调用)
// dt: 时间增量 (ms)
void updateBulletPosition(Bullet& bullet, f32 dt);

// ============================================================================
// 碰撞检测和结算
// ============================================================================

// 子弹命中坦克的结果
struct BulletTankHit {
  i32 tankId;              // 被命中的坦克 ID
  i32 bulletId;            // 发射的子弹 ID
  i32 damage;              // 造成的伤害
};

// 检测所有子弹与坦克的碰撞
std::vector<BulletTankHit> detectBulletTankCollisions(
    const std::vector<Bullet>& bullets,
    const std::vector<Tank>& tanks);

// 处理子弹与地形的碰撞 (返回被摧毁的砖块/钢块位置)
struct TileHit {
  i32 x;                   // 受损方格 X
  i32 y;                   // 受损方格 Y
  bool isBrick;            // true=砖块, false=钢块
};

std::vector<TileHit> detectBulletTerrainCollisions(
    const std::vector<Bullet>& bullets,
    TerrainMap& terrain);

// 处理坦克与坦克的碰撞 (停止双方运动)
struct TankTankCollision {
  i32 tank1Id;
  i32 tank2Id;
};

std::vector<TankTankCollision> detectTankTankCollisions(
    const std::vector<Tank>& tanks);

// ============================================================================
// 伤害和死亡处理
// ============================================================================

// 对坦克造成伤害
// tank: 被攻击的坦克
// damage: 伤害值
// returns: 坦克是否在此伤害后死亡
bool damageTank(Tank& tank, i32 damage);

// 销毁坦克并创建爆炸
void killTank(Tank& tank, Explosion& explosion);

// ============================================================================
// 道具系统
// ============================================================================

// 可以放置道具的有效位置列表
std::vector<Point> getValidPowerUpPositions(const TerrainMap& terrain,
                                            const std::vector<Tank>& tanks,
                                            const std::vector<PowerUp>& powerUps);

// 根据随机数确定道具类型 (按出生序号和难度)
PowerUpName determinePowerUpType(i32 botIndex, i32 difficulty);

// 坦克拾取道具的处理
struct PowerUpEffect {
  std::optional<TankLevel> levelUp;      // null 表示不升级
  bool spawnExtraTank;                   // 是否额外 +1 命
  bool freezeAllBots;                    // 是否冻结所有 bot
  bool destroyAllBots;                   // 是否炸掉所有 bot
  bool grantHelmet;                      // 是否获得头盔
  bool makeEagleSteel;                   // 是否让老鹰变钢
};

PowerUpEffect getPowerUpEffect(const PowerUp& powerUp, const Tank& tank);

// 应用道具效果到坦克
void applyPowerUpToTank(Tank& tank, const PowerUpEffect& effect);

// ============================================================================
// 道具超时处理
// ============================================================================

// 更新所有道具的生命周期 (检查是否过期)
// duration: 道具在场存活时长 (ms)
void updatePowerUps(std::vector<PowerUp>& powerUps, f32 dt, i32 maxLifeMs = 10000);

// ============================================================================
// 特殊效果系统
// ============================================================================

// 更新爆炸动画时间
void updateExplosion(Explosion& explosion, f32 dt);

// 更新得分弹出文字的生命周期
void updateScorePopup(ScorePopup& popup, f32 dt);

// 更新闪烁效果
void updateFlicker(Flicker& flicker, f32 dt);

// ============================================================================
// bot 出生系统
// ============================================================================

// bot 出生配置参数
struct BotSpawnConfig {
  i32 maxOnField;        // 同时在场的最大 bot 数
  f32 spawnIntervalMs;   // 出生间隔 (ms)
  f32 flickerDurationMs; // 出生闪烁时长 (ms)
};

// 计算 bot 出生间隔 (与关卡号和游戏模式相关)
f32 calculateBotSpawnInterval(i32 stageNumber, bool isMultiplayer);

// 尝试生成下一个 bot
struct BotSpawnResult {
  std::optional<Tank> newBot;        // 新生成的坦克 (如果成功)
  std::optional<Flicker> flicker;    // 伴随的闪烁效果
};

BotSpawnResult trySpawnBot(const std::vector<BotGroupConfig>& botGroups,
                           i32& nextBotIndex,
                           f32& spawnTimerMs,
                           const BotSpawnConfig& config,
                           const TerrainMap& terrain,
                           f32 dt);

// ============================================================================
// 游戏状态流转
// ============================================================================

// 游戏阶段
enum class GamePhase {
  Enter,      // 入场幕布动画
  Playing,    // 对战中
  Won,        // 阶段胜利
  Lost,       // 阶段失败
  Statistics, // 结算页面
  GameOver    // 游戏结束
};

// 检查当前关卡是否已赢
bool isStageWon(const std::vector<Tank>& tanks, const TerrainMap& terrain);

// 检查当前关卡是否已输 (老鹰被击中或玩家全部死亡)
struct LoseCondition {
  bool eagleDestroyed;   // 老鹰是否被击中
  bool allPlayersDeadOrOut;  // 所有玩家是否都已死亡
};

LoseCondition checkLoseCondition(const std::vector<Tank>& tanks,
                                  const TerrainMap& terrain,
                                  const std::vector<PlayerState>& players);

// ============================================================================
// 时间和固定步长游戏循环
// ============================================================================

// 游戏循环参数
struct GameLoopState {
  f32 accumulator;       // 累积的实际时间 (ms)
  f32 lastFrameTime;     // 上一帧的时间戳 (ms)
  f32 deltaTime;         // 当前帧的时间增量 (ms)
  i32 stepCount;         // 本帧推进的逻辑步数
};

// 确定本帧需要推进多少个逻辑步
i32 calculateLogicSteps(f32 deltaTime, f32& accumulator, f32 stepMs = Constants::STEP_MS);

// ============================================================================
// 随机数生成 (确定性)
// ============================================================================

// 随机数发生器接口 (便于替换为确定性实现)
struct RandomEngine {
  virtual f32 next() = 0;      // 返回 [0, 1) 的随机数
  virtual i32 nextInt(i32 max) = 0;  // 返回 [0, max) 的随机整数
  virtual ~RandomEngine() = default;
};

// 确定性随机数生成器 (种子确定则序列确定)
class SeededRandom : public RandomEngine {
public:
  // 初始化种子
  explicit SeededRandom(u32 seed);
  
  f32 next() override;
  i32 nextInt(i32 max) override;
  
private:
  u32 state;  // 线性同余发生器的状态
};

#endif  // GAME_LOGIC_H

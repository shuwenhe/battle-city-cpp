// ============================================================================
// game-logic.cpp - 游戏逻辑系统的实现
// 
// 包含坦克运动、子弹发射、碰撞检测等核心逻辑。
// ============================================================================

#include "core/game-logic.h"
#include "physics/geometry.h"
#include <cmath>

// ============================================================================
// 坦克数值计算
// ============================================================================

f32 getTankMoveSpeed(const Tank& tank) {
  // 注释: 玩家和 bot 的移速数值不同
  if (tank.side == TankSide::Player) {
    // 玩家坦克统一 0.045 px/ms，不受等级影响
    return Constants::PLAYER_MOVE_SPEED;
  } else {
    // bot 的移速随等级变化
    if (tank.level == TankLevel::Fast) {
      // fast 坦克是最快的
      return Constants::BOT_FAST_MOVE_SPEED;
    } else {
      // basic/power/armor 都用基础速度
      return Constants::BOT_BASIC_MOVE_SPEED;
    }
  }
}

f32 getTankBulletSpeed(const Tank& tank) {
  // 注释: 子弹速度由等级决定
  if (tank.side == TankSide::Player) {
    // 玩家: basic/fast -> 0.12, power/armor -> 0.18
    if (tank.level == TankLevel::Basic || tank.level == TankLevel::Fast) {
      return Constants::PLAYER_BASIC_BULLET_SPEED;
    } else {
      return Constants::PLAYER_POWER_BULLET_SPEED;
    }
  } else {
    // bot: basic -> 0.12, fast -> 0.18, power/armor -> 0.24
    if (tank.level == TankLevel::Basic) {
      return Constants::PLAYER_BASIC_BULLET_SPEED;  // 0.12
    } else if (tank.level == TankLevel::Fast) {
      return Constants::PLAYER_POWER_BULLET_SPEED;  // 0.18
    } else {
      return Constants::BOT_POWER_BULLET_SPEED;  // 0.24
    }
  }
}

i32 getTankBulletPower(const Tank& tank) {
  // 注释: 威力决定了是否能摧毁钢墙（需要 >= 3）
  if (tank.side == TankSide::Player) {
    // 玩家 armor 等级才能摧毁钢墙
    if (tank.level == TankLevel::Armor) {
      return 3;  // armor 威力 3, 可以摧毁钢墙
    } else {
      return 1;  // 其他等级威力 1
    }
  } else {
    // bot power 等级威力 2, 无法摧毁钢墙
    if (tank.level == TankLevel::Power) {
      return 2;
    } else {
      return 1;  // basic/fast/armor 威力 1
    }
  }
}

i32 getTankBulletInterval(const Tank& tank) {
  // 注释: 开火间隔影响开枪频率
  if (tank.level == TankLevel::Basic) {
    // basic 坦克开枪最慢 (300ms)
    return Constants::PLAYER_BASIC_BULLET_INTERVAL;
  } else {
    // 其他等级更快 (200ms)
    return Constants::PLAYER_POWER_BULLET_INTERVAL;
  }
}

i32 getTankBulletLimit(const Tank& tank) {
  // 注释: 同时在场的最大子弹数 (影响火力)
  if (tank.side == TankSide::Bot || 
      tank.level == TankLevel::Basic || 
      tank.level == TankLevel::Fast) {
    // bot 或玩家的弱坦克只能发射 1 颗子弹
    return 1;
  } else {
    // 玩家的 power/armor 可以同时发射 2 颗
    return 2;
  }
}

i32 getKillScore(TankLevel level) {
  // 注释: 不同等级坦克的击杀得分
  switch (level) {
    case TankLevel::Basic:
      return Constants::TANK_KILL_SCORE_BASIC;    // 100
    case TankLevel::Fast:
      return Constants::TANK_KILL_SCORE_FAST;     // 200
    case TankLevel::Power:
      return Constants::TANK_KILL_SCORE_POWER;    // 300
    case TankLevel::Armor:
      return Constants::TANK_KILL_SCORE_ARMOR;    // 400
    default:
      return 0;  // 不应该达到
  }
}

// ============================================================================
// 坦克运动系统
// ============================================================================

bool canTankMove(const TerrainMap& terrain, const std::vector<Tank>& otherTanks,
                 Tank& tank, f32 newX, f32 newY, f32 threshold) {
  // 注释: 检查坦克是否能在新位置，需要验证多个碰撞条件
  
  // 创建坦克在新位置的碰撞矩形
  Rect tankRect = {newX, newY, static_cast<f32>(Constants::TANK_SIZE),
                   static_cast<f32>(Constants::TANK_SIZE)};
  
  // 检查 1: 在场地边界内
  // 注释: 坦克位置必须完全在 208×208 的战场范围内
  const f32 FIELD_SIZE = static_cast<f32>(Constants::FIELD_SIZE);
  if (tankRect.x < 0 || tankRect.y < 0 || 
      tankRect.x + tankRect.w > FIELD_SIZE || 
      tankRect.y + tankRect.h > FIELD_SIZE) {
    return false;  // 超出边界
  }
  
  // 检查 2: 与地形碰撞 (砖块、钢块、河流)
  // 注释: 这里简化处理, 完整实现需要调用 terrain 的碰撞方法
  // if (terrain.collideWithBricks(tankRect, threshold)) return false;
  // if (terrain.collideWithSteels(tankRect, threshold)) return false;
  
  // 检查 3: 与其他坦克碰撞
  // 注释: 只检查其他活着的坦克
  for (const auto& other : otherTanks) {
    if (&other == &tank) continue;  // 跳过自己
    if (!other.alive) continue;     // 跳过死坦克
    
    // 两坦克碰撞检测
    Rect otherRect = other.getRect();
    if (test_rect_collide(tankRect, otherRect)) {
      return false;  // 与其他坦克碰撞
    }
  }
  
  // 所有检查都通过
  return true;
}

void updateTankMovement(Tank& tank, const TankIntent& intent, f32 dt) {
  // 注释: 更新坦克位置 (根据输入意图)
  
  // 第一步: 处理转向 (改变方向但不改变位置)
  if (intent.turnDirection.has_value()) {
    tank.direction = intent.turnDirection.value();
  }
  
  // 第二步: 处理前进 (沿当前方向移动)
  if (intent.moveForward) {
    // 获取坦克的移速
    f32 speed = getTankMoveSpeed(tank);
    
    // 计算本帧的位移量
    f32 distance = speed * dt;
    
    // 如果设置了最大距离限制，则不能超过
    if (intent.maxDistance > 0 && distance > intent.maxDistance) {
      distance = intent.maxDistance;
    }
    
    // 根据方向计算新位置
    f32 newX = tank.x;
    f32 newY = tank.y;
    
    switch (tank.direction) {
      case Direction::Up:
        newY -= distance;  // 向上减少 Y
        break;
      case Direction::Down:
        newY += distance;  // 向下增加 Y
        break;
      case Direction::Left:
        newX -= distance;  // 向左减少 X
        break;
      case Direction::Right:
        newX += distance;  // 向右增加 X
        break;
    }
    
    // 尝试移动到新位置 (这里简化, 实际需要检查碰撞)
    // 完整实现会调用 canTankMove 来验证
    tank.x = newX;
    tank.y = newY;
    tank.moving = true;  // 标记本帧发生了移动
  } else {
    tank.moving = false;  // 本帧没有移动
  }
  
  // 第三步: 更新冻结和冷却计时器 (每帧递减)
  if (tank.frozenTimeout > 0) {
    tank.frozenTimeout -= dt;
  }
  if (tank.cooldown > 0) {
    tank.cooldown -= dt;
  }
  if (tank.helmetDuration > 0) {
    tank.helmetDuration -= dt;
  }
}

void rotateTank(Tank& tank, Direction newDirection) {
  // 注释: 简单的转向操作 (不改变位置)
  tank.direction = newDirection;
}

// ============================================================================
// 坦克发射系统
// ============================================================================

Point calculateBulletSpawnPos(const Tank& tank) {
  // 注释: 计算子弹生成点 (坦克前方中心)
  
  // 坦克中心
  f32 centerX = tank.x + Constants::TANK_SIZE / 2.0f;
  f32 centerY = tank.y + Constants::TANK_SIZE / 2.0f;
  
  // 根据坦克方向，在前方生成子弹
  f32 offset = static_cast<f32>(Constants::TANK_SIZE) / 2.0f;
  
  switch (tank.direction) {
    case Direction::Up:
      return {centerX - Constants::BULLET_SIZE / 2.0f, 
              centerY - offset - Constants::BULLET_SIZE / 2.0f};
    case Direction::Down:
      return {centerX - Constants::BULLET_SIZE / 2.0f, 
              centerY + offset - Constants::BULLET_SIZE / 2.0f};
    case Direction::Left:
      return {centerX - offset - Constants::BULLET_SIZE / 2.0f, 
              centerY - Constants::BULLET_SIZE / 2.0f};
    case Direction::Right:
      return {centerX + offset - Constants::BULLET_SIZE / 2.0f, 
              centerY - Constants::BULLET_SIZE / 2.0f};
    default:
      return {centerX, centerY};  // 不应该达到
  }
}

std::optional<BulletSpawnParams> tryFireBullet(Tank& tank, i32 currentBulletCount) {
  // 注释: 尝试从坦克发射子弹 (检查限制条件)
  
  // 检查 1: 冷却是否完成
  if (tank.cooldown > 0) {
    return std::nullopt;  // 还在开火冷却中
  }
  
  // 检查 2: 已发射子弹数是否达到上限
  i32 limit = getTankBulletLimit(tank);
  if (currentBulletCount >= limit) {
    return std::nullopt;  // 达到子弹数上限
  }
  
  // 可以发射子弹
  // 注释: 创建子弹参数并设置冷却
  Point spawnPos = calculateBulletSpawnPos(tank);
  tank.cooldown = static_cast<f32>(getTankBulletInterval(tank));
  
  return BulletSpawnParams{
    tank.tankId,
    tank.side,
    tank.direction,
    getTankBulletSpeed(tank),
    getTankBulletPower(tank),
    spawnPos
  };
}

// ============================================================================
// 子弹移动系统
// ============================================================================

void updateBulletPosition(Bullet& bullet, f32 dt) {
  // 注释: 更新子弹位置 (直线移动)
  
  // 保存上一帧位置 (用于连续碰撞检测)
  bullet.lastX = bullet.x;
  bullet.lastY = bullet.y;
  
  // 计算位移
  f32 distance = bullet.speed * dt;
  
  // 根据方向更新位置
  switch (bullet.direction) {
    case Direction::Up:
      bullet.y -= distance;
      break;
    case Direction::Down:
      bullet.y += distance;
      break;
    case Direction::Left:
      bullet.x -= distance;
      break;
    case Direction::Right:
      bullet.x += distance;
      break;
  }
  
  // 简单的边界检查 (超出边界则标记死亡)
  Rect bulletRect = bullet.getRect();
  const f32 FIELD_SIZE = static_cast<f32>(Constants::FIELD_SIZE);
  if (bulletRect.x < 0 || bulletRect.y < 0 || 
      bulletRect.x + bulletRect.w > FIELD_SIZE || 
      bulletRect.y + bulletRect.h > FIELD_SIZE) {
    bullet.dead = true;  // 出界销毁
  }
}

// ============================================================================
// 碰撞检测和结算
// ============================================================================

std::vector<BulletTankHit> detectBulletTankCollisions(
    const std::vector<Bullet>& bullets,
    const std::vector<Tank>& tanks) {
  // 注释: 检测子弹与坦克的碰撞
  
  std::vector<BulletTankHit> hits;
  
  // 遍历所有子弹
  for (const auto& bullet : bullets) {
    if (bullet.dead) continue;  // 死亡的子弹不检测
    
    Rect bulletRect = bullet.getRect();
    
    // 与每个坦克检测碰撞
    for (const auto& tank : tanks) {
      if (!tank.alive) continue;  // 死坦克不检测
      
      // 同队坦克不互相伤害
      if (bullet.side == tank.side) continue;
      
      Rect tankRect = tank.getRect();
      
      // 检测碰撞
      if (test_rect_collide(bulletRect, tankRect)) {
        // 记录命中
        hits.push_back(BulletTankHit{
          tank.tankId,
          bullet.bulletId,
          bullet.power
        });
      }
    }
  }
  
  return hits;
}

std::vector<TileHit> detectBulletTerrainCollisions(
    const std::vector<Bullet>& bullets,
    TerrainMap& terrain) {
  // 注释: 检测子弹与地形的碰撞
  // 这是一个复杂的函数，需要访问地形网格的像素精度碰撞
  // 这里仅提供框架, 完整实现在 terrain 的碰撞检测模块中
  
  std::vector<TileHit> hits;
  // TODO: 实现详细的砖块/钢块碰撞检测
  return hits;
}

std::vector<TankTankCollision> detectTankTankCollisions(
    const std::vector<Tank>& tanks) {
  // 注释: 检测坦克与坦克的碰撞
  
  std::vector<TankTankCollision> collisions;
  
  // 遍历所有坦克对
  for (size_t i = 0; i < tanks.size(); ++i) {
    if (!tanks[i].alive) continue;
    
    for (size_t j = i + 1; j < tanks.size(); ++j) {
      if (!tanks[j].alive) continue;
      
      // 检测碰撞
      if (test_rect_collide(tanks[i].getRect(), tanks[j].getRect())) {
        collisions.push_back(TankTankCollision{
          tanks[i].tankId,
          tanks[j].tankId
        });
      }
    }
  }
  
  return collisions;
}

// ============================================================================
// 伤害和死亡处理
// ============================================================================

bool damageTank(Tank& tank, i32 damage) {
  // 注释: 对坦克造成伤害，检查是否死亡
  
  tank.hp -= damage;
  
  // 如果生命值 <= 0，坦克死亡
  if (tank.hp <= 0) {
    tank.alive = false;
    return true;  // 已死亡
  }
  
  return false;  // 尚未死亡
}

void killTank(Tank& tank, Explosion& explosion) {
  // 注释: 销毁坦克并创建爆炸效果
  tank.alive = false;
  // explosion 的初始化由调用者处理
}

// ============================================================================
// 特殊效果系统
// ============================================================================

void updateExplosion(Explosion& explosion, f32 dt) {
  // 注释: 更新爆炸动画计时
  explosion.elapsed += dt;
}

void updateScorePopup(ScorePopup& popup, f32 dt) {
  // 注释: 更新得分文字的生命周期
  popup.elapsed += dt;
}

void updateFlicker(Flicker& flicker, f32 dt) {
  // 注释: 更新闪烁效果的计时
  flicker.elapsed += dt;
}

// ============================================================================
// 其他函数 (暂时留空，将在后续实现)
// ============================================================================

// bot 出生相关
f32 calculateBotSpawnInterval(i32 stageNumber, bool isMultiplayer) {
  // 注释: bot 出生间隔随关卡号递减
  // 公式: 190 - 4 * min(stageNumber, 35) - (isMultiplayer ? 20 : 0)
  i32 baseTicks = 190 - 4 * (stageNumber < 35 ? stageNumber : 35);
  if (isMultiplayer) {
    baseTicks -= 20;
  }
  return Constants::FRAME_MS * baseTicks;  // 转换为毫秒
}

i32 calculateLogicSteps(f32 deltaTime, f32& accumulator, f32 stepMs) {
  // 注释: 根据时间增量计算需要推进多少个逻辑步
  accumulator += deltaTime;
  
  i32 steps = 0;
  while (accumulator >= stepMs) {
    steps++;
    accumulator -= stepMs;
  }
  
  return steps;
}

// ============================================================================
// 随机数生成器
// ============================================================================

SeededRandom::SeededRandom(u32 seed) : state(seed) {
  // 注释: 使用种子初始化 LCG 状态
}

f32 SeededRandom::next() {
  // 注释: 线性同余生成器 (LCG)
  // 公式: state = (a * state + c) mod m
  // 使用 MINSTD 参数以获得较好的随机性
  state = (1103515245u * state + 12345u) & 0x7fffffffu;
  return state / 2147483647.0f;  // 转换为 [0, 1)
}

i32 SeededRandom::nextInt(i32 max) {
  // 注释: 生成 [0, max) 的随机整数
  if (max <= 0) return 0;
  return static_cast<i32>(next() * max);
}

/**
 * C++ 游戏引擎 Node.js 绑定
 * 提供从 TypeScript 调用 C++ 函数的接口
 */

const ffi = require('ffi-napi');
const path = require('path');

// 动态库路径
const libPath = path.join(__dirname, '../build/cpp/libgame.dylib');

let gameEngine = null;

/**
 * 初始化 C++ 游戏引擎
 */
function initGameEngine() {
  if (gameEngine) return gameEngine;

  try {
    // 定义 C++ 函数签名
    gameEngine = ffi.Library(libPath, {
      // 基础函数
      'getTankMoveSpeed': ['double', ['int']], // int tankLevel -> double speed
      'getTankBulletSpeed': ['double', ['int']], // int tankLevel -> double speed
      'getTankBulletPower': ['int', ['int']], // int tankLevel -> int power
      'getTankBulletCooldown': ['double', ['int']], // int tankLevel -> double cooldown
      
      // 物理更新
      'updateTankPosition': ['void', ['double', 'double', 'double', 'int', 'double', 'double']],
      // direction, currentX, currentY, tankLevel, stageWidth, stageHeight
      
      'updateBulletPosition': ['void', ['double', 'double', 'double', 'int', 'double']],
      // direction, currentX, currentY, speed, deltaTime
      
      // 碰撞检测
      'checkTankCollision': ['int', ['double', 'double', 'int', 'int']],
      // tankX, tankY, tankSize, terrainMap
      
      'checkBulletCollision': ['int', ['double', 'double', 'int']],
      // bulletX, bulletY, terrainMap
    });

    console.log('✅ C++ 游戏引擎已加载');
    return gameEngine;
  } catch (err) {
    console.error('❌ 加载 C++ 引擎失败:', err.message);
    console.log('⚠️  将使用 TypeScript 版本');
    return null;
  }
}

/**
 * 安全调用 C++ 函数
 */
function safeCall(funcName, ...args) {
  try {
    const engine = gameEngine || initGameEngine();
    if (!engine || !engine[funcName]) {
      return null;
    }
    return engine[funcName](...args);
  } catch (err) {
    console.error(`❌ 调用 C++ 函数 ${funcName} 失败:`, err.message);
    return null;
  }
}

/**
 * 获取坦克移动速度（C++ 版本）
 */
function getTankMoveSpeedCpp(tankLevel) {
  const result = safeCall('getTankMoveSpeed', tankLevel);
  return result !== null ? result : null;
}

/**
 * 获取子弹速度（C++ 版本）
 */
function getTankBulletSpeedCpp(tankLevel) {
  const result = safeCall('getTankBulletSpeed', tankLevel);
  return result !== null ? result : null;
}

/**
 * 获取子弹伤害（C++ 版本）
 */
function getTankBulletPowerCpp(tankLevel) {
  const result = safeCall('getTankBulletPower', tankLevel);
  return result !== null ? result : null;
}

/**
 * 获取开火冷却时间（C++ 版本）
 */
function getTankBulletCooldownCpp(tankLevel) {
  const result = safeCall('getTankBulletCooldown', tankLevel);
  return result !== null ? result : null;
}

/**
 * 更新坦克位置（C++ 版本）
 */
function updateTankPositionCpp(direction, currentX, currentY, tankLevel, stageWidth, stageHeight) {
  return safeCall('updateTankPosition', direction, currentX, currentY, tankLevel, stageWidth, stageHeight);
}

/**
 * 检查碰撞（C++ 版本）
 */
function checkTankCollisionCpp(tankX, tankY, tankSize, terrainMap) {
  const result = safeCall('checkTankCollision', tankX, tankY, tankSize, terrainMap);
  return result !== null ? result : -1;
}

// 导出模块
module.exports = {
  initGameEngine,
  safeCall,
  getTankMoveSpeedCpp,
  getTankBulletSpeedCpp,
  getTankBulletPowerCpp,
  getTankBulletCooldownCpp,
  updateTankPositionCpp,
  checkTankCollisionCpp,
};

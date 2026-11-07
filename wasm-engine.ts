/**
 * 坦克大战 C++ 引擎 - WASM加载器和类型绑定
 * 
 * 此模块加载编译后的C++引擎,并为TypeScript UI和C++逻辑之间
 * 提供无缝互操作性的TypeScript绑定
 */

// ============================================================================
// TypeScript类型定义 - 与C++类型对应
// ============================================================================

export type Direction = 'up' | 'down' | 'left' | 'right'  // 方向类型(4个方向)

export interface Point {            // 二维点结构体
  x: number                         // X坐标
  y: number                         // Y坐标
}

export interface Rect {             // 矩形结构体
  x: number                         // 左上角X坐标
  y: number                         // 左上角Y坐标
  w: number                         // 矩形宽度
  h: number                         // 矩形高度
}

export interface Bullet {           // 子弹结构体
  bullet_id: number                 // 子弹唯一ID
  x: number                         // 当前X坐标
  y: number                         // 当前Y坐标
  last_x: number                    // 上一帧X坐标(连续碰撞检测)
  last_y: number                    // 上一帧Y坐标(连续碰撞检测)
  direction: Direction              // 飞行方向
  speed: number                     // 子弹速度
  tank_id: number                   // 发射坦克ID
  fired_at: number                  // 发射时时间戳
}

export interface CollisionResult {  // 碰撞检测结果
  collided: boolean                 // 是否发生碰撞
  overlap_x: number                 // X轴重叠距离
  overlap_y: number                 // Y轴重叠距离
}

export interface BulletHitPoint {   // 子弹击中点
  x: number                         // 击中点X坐标
  y: number                         // 击中点Y坐标
}

/**
 * WASM模块实例类型定义
 * 包含所有导出的C++函数的TypeScript签名
 */
interface WasmModule {              // 定义WASM模块接口
  // 内存管理函数
  _malloc(size: number): number     // 分配内存块
  _free(ptr: number): void          // 释放内存块
  
  // 引擎初始化函数
  _engine_init(seed: number): void  // 初始化引擎(设置随机数种子)
  
  // 几何计算函数
  _geom_test_rect_collide(         // 矩形碰撞测试
    ax: number, ay: number, aw: number, ah: number,  // 第一个矩形
    bx: number, by: number, bw: number, bh: number   // 第二个矩形
  ): boolean                        // 返回是否碰撞
  _geom_distance(x1: number, y1: number, x2: number, y2: number): number  // 计算两点距离
  _geom_distance_sq(x1: number, y1: number, x2: number, y2: number): number  // 计算距离平方
  
  // 子弹碰撞函数
  _bullet_calculate_hit_time(       // 计算两颗子弹碰撞时间
    b1_x: number, b1_y: number, b1_last_x: number, b1_last_y: number,  // 子弹1
    b1_dir: number, b1_speed: number,  // 子弹1方向和速度
    b2_x: number, b2_y: number, b2_last_x: number, b2_last_y: number,  // 子弹2
    b2_dir: number, b2_speed: number   // 子弹2方向和速度
  ): number                         // 返回碰撞时间或-1
  
  // 随机数函数
  _random_init(seed: number): void  // 初始化随机数生成器
  _random_next(): number            // 获取下一个随机数[0,1)
  _random_next_int(min: number, max: number): number  // 获取范围内随机整数
  
  // 工具函数(Emscripten提供)
  ccall(name: string, returnType: string, argTypes: string[], args: unknown[]): unknown  // 调用C函数
  cwrap(name: string, returnType: string, argTypes: string[]): Function  // 包装C函数
}

declare const Module: WasmModule | undefined  // 声明全局Module对象(由WASM加载器提供)

let wasmModule: WasmModule | null = null      // 缓存已加载的WASM模块

/**
 * 加载WASM引擎模块
 * 
 * 在使用任何引擎函数前,在应用初始化期间调用一次此函数
 * @return Promise<WasmModule> 已加载的WASM模块
 */
export async function loadWasmEngine(): Promise<WasmModule> {  // 定义WASM加载函数
  if (wasmModule) {                 // 如果模块已缓存
    return wasmModule               // 直接返回缓存的模块
  }
  // @ts-ignore WASM module loading
  return Module                    // 返回WASM模块
}

/**
 * 用随机数种子初始化引擎
 * @param seed 随机数种子
 * @return Promise<void> 初始化完成
 */
export async function initEngine(seed: number): Promise<void> {  // 定义引擎初始化函数
  const module = await loadWasmEngine()  // 加载WASM模块
  module._engine_init(seed)      // 调用C++初始化函数
}

// ============================================================================
// 几何运算API - 提供几何计算的异步接口
// ============================================================================

/**
 * 测试两个矩形的AABB碰撞
 * @param a 第一个矩形
 * @param b 第二个矩形
 * @return Promise<boolean> 是否碰撞
 */
export async function testRectCollide(  // 定义矩形碰撞测试函数
  a: Rect,                         // 参数a:第一个矩形
  b: Rect                          // 参数b:第二个矩形
): Promise<boolean> {              // 返回异步布尔值
  const module = await loadWasmEngine()  // 加载WASM模块
  return module._geom_test_rect_collide(a.x, a.y, a.w, a.h, b.x, b.y, b.w, b.h)  // 调用C++函数并返回
}

/**
 * 计算两点间的欧几里得距离
 * @param a 第一个点
 * @param b 第二个点
 * @return Promise<number> 两点间的距离
 */
export async function distance(a: Point, b: Point): Promise<number> {  // 定义距离计算函数
  const module = await loadWasmEngine()  // 加载WASM模块
  return module._geom_distance(a.x, a.y, b.x, b.y)  // 调用C++函数并返回距离
}

/**
 * 计算两点间的距离平方(用于比较时更快,避免开方)
 * @param a 第一个点
 * @param b 第二个点
 * @return Promise<number> 距离的平方
 */
export async function distanceSq(a: Point, b: Point): Promise<number> {  // 定义距离平方计算函数
  const module = await loadWasmEngine()  // 加载WASM模块
  return module._geom_distance_sq(a.x, a.y, b.x, b.y)  // 调用C++函数并返回距离平方
}

// ============================================================================
// 子弹碰撞API - 提供子弹物理计算的异步接口
// ============================================================================

/**
 * 计算两颗子弹的碰撞时间
 * @param b1 第一颗子弹
 * @param b2 第二颗子弹
 * @return Promise<number> 碰撞时间(毫秒),或-1表示不会碰撞
 */
export async function calculateBulletHitTime(  // 定义子弹碰撞时间计算函数
  b1: Bullet,                      // 参数b1:第一颗子弹
  b2: Bullet                       // 参数b2:第二颗子弹
): Promise<number> {              // 返回异步数字(碰撞时间)
  const module = await loadWasmEngine()  // 加载WASM模块
  const dirMap = { up: 0, down: 1, left: 2, right: 3 }  // 创建方向到整数的映射
  
  return module._bullet_calculate_hit_time(  // 调用C++碰撞计算函数
    b1.x, b1.y, b1.last_x, b1.last_y, dirMap[b1.direction], b1.speed,  // 子弹1参数
    b2.x, b2.y, b2.last_x, b2.last_y, dirMap[b2.direction], b2.speed   // 子弹2参数
  )
}

// ============================================================================
// 随机数生成API - 提供随机数生成的异步接口
// ============================================================================

/**
 * 用种子初始化随机数生成器
 * @param seed 随机数种子
 * @return Promise<void> 初始化完成
 */
export async function initRandom(seed: number): Promise<void> {  // 定义随机数初始化函数
  const module = await loadWasmEngine()  // 加载WASM模块
  module._random_init(seed)      // 调用C++初始化函数
}

/**
 * 生成范围[0, 1)内的下一个随机数
 * @return Promise<number> 0到1之间的随机数
 */
export async function randomNext(): Promise<number> {  // 定义随机数生成函数
  const module = await loadWasmEngine()  // 加载WASM模块
  return module._random_next()  // 调用C++函数并返回随机数
}

/**
 * 生成范围[min, max)内的随机整数
 * @param min 最小值(包含)
 * @param max 最大值(不包含)
 * @return Promise<number> min到max之间的随机整数
 */
export async function randomNextInt(min: number, max: number): Promise<number> {  // 定义随机整数生成函数
  const module = await loadWasmEngine()  // 加载WASM模块
  return module._random_next_int(min, max)  // 调用C++函数并返回随机整数
}

// ============================================================================
// 同步引擎包装 - 用于高性能路径(需谨慎使用)
// ============================================================================

/**
 * 同步几何函数包装
 * 仅在ensureWasmLoaded()和requestIdleCallback后使用
 * 避免异步开销,用于频繁调用的性能关键路径
 */
export const SyncEngine = {        // 定义同步引擎对象
  testRectCollide(a: Rect, b: Rect): boolean {  // 同步矩形碰撞测试
    if (!wasmModule) throw new Error('WASM module not loaded')  // 检查模块是否加载
    return (wasmModule as WasmModule)._geom_test_rect_collide(a.x, a.y, a.w, a.h, b.x, b.y, b.w, b.h)  // 直接调用C++函数
  },
  
  distance(a: Point, b: Point): number {  // 同步距离计算
    if (!wasmModule) throw new Error('WASM module not loaded')  // 检查模块是否加载
    return (wasmModule as WasmModule)._geom_distance(a.x, a.y, b.x, b.y)  // 直接调用C++函数
  },
  
  distanceSq(a: Point, b: Point): number {  // 同步距离平方计算
    if (!wasmModule) throw new Error('WASM module not loaded')  // 检查模块是否加载
    return (wasmModule as WasmModule)._geom_distance_sq(a.x, a.y, b.x, b.y)  // 直接调用C++函数
  },
  
  calculateBulletHitTime(b1: Bullet, b2: Bullet): number {  // 同步子弹碰撞时间计算
    if (!wasmModule) throw new Error('WASM module not loaded')  // 检查模块是否加载
    const dirMap = { up: 0, down: 1, left: 2, right: 3 }  // 创建方向映射
    return (wasmModule as WasmModule)._bullet_calculate_hit_time(  // 直接调用C++函数
      b1.x, b1.y, b1.last_x, b1.last_y, dirMap[b1.direction], b1.speed,  // 子弹1参数
      b2.x, b2.y, b2.last_x, b2.last_y, dirMap[b2.direction], b2.speed   // 子弹2参数
    )
  },
  
  randomNext(): number {            // 同步随机数生成
    if (!wasmModule) throw new Error('WASM module not loaded')  // 检查模块是否加载
    return (wasmModule as WasmModule)._random_next()  // 直接调用C++函数
  },
}

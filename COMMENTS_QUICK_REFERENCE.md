# 📝 代码注释快速参考

## 🎯 按用途快速查找

### 🚀 我想了解游戏如何工作
**阅读顺序**:
1. [types.h](/Users/shuwen/battle-city-cpp/include/types.h) - 了解所有数据类型
2. [geometry.h](/Users/shuwen/battle-city-cpp/include/geometry.h) - 了解几何计算
3. [collision.h](/Users/shuwen/battle-city-cpp/include/collision.h) - 了解碰撞检测

### 🎮 我想修改碰撞规则
**查看文件**:
- [collision.h](/Users/shuwen/battle-city-cpp/include/collision.h) - 接口定义
- [collision.cpp](/Users/shuwen/battle-city-cpp/cpp/src/collision.cpp) - 实现逻辑
- [types.h](/Users/shuwen/battle-city-cpp/include/types.h) 第 1-60 行 - 常数定义

### 💥 我想修改子弹物理
**查看文件**:
- [bullet-collision.h](/Users/shuwen/battle-city-cpp/include/bullet-collision.h) - 数据结构
- [bullet-collision.cpp](/Users/shuwen/battle-city-cpp/cpp/src/bullet-collision.cpp) - 所有实现

### 🔀 我想修改随机行为
**查看文件**:
- [random.h](/Users/shuwen/battle-city-cpp/include/random.h) - 算法定义
- [random.cpp](/Users/shuwen/battle-city-cpp/cpp/src/random.cpp) - 初始化

### 🌉 我想连接 TypeScript 和 C++
**查看文件**:
- [bindings.cpp](/Users/shuwen/battle-city-cpp/cpp/src/bindings.cpp) - C++ 导出
- [wasm-engine.ts](/Users/shuwen/battle-city-cpp/src/wasm-engine.ts) - TypeScript 导入

---

## 📍 按文件的关键行数

### types.h
```
1-50      基本类型别名
50-80     方向枚举
80-110    道具枚举
110-150   地图类型枚举
150-180   坦克、子弹、爆炸结构体
180-220   常数定义
```

### geometry.h
```
1-40      距离计算函数
40-60     数学工具函数
60-100    AABB 碰撞检测
100-140   圆形碰撞检测
```

### collision.h
```
1-40      地形和坦克接口
40-80     碰撞世界结构体
80-150    碰撞工具函数
```

### bullet-collision.h
```
1-50      子弹结构体
50-100    碰撞结果结构体
100-150   碰撞工具常量和函数
```

### collision.cpp
```
1-30      can_tank_move 函数(主函数)
30-50     is_valid_tank_position 函数
50-70     get_colliding_tanks 函数
```

### bullet-collision.cpp
```
1-50      方向旋转和 MBR 计算
50-100    子弹爆炸扩散
100-200   碰撞时间计算(最复杂)
200-250   运动计算和爆炸位置
```

### random.h
```
1-20      Mulberry32 算法类
20-40     next() 函数
40-60     next_int/next_float 函数
```

### bindings.cpp
```
1-100     EMSCRIPTEN_BINDINGS
100-170   C 接口函数
```

### wasm-engine.ts
```
1-50      TypeScript 类型定义
50-100    WASM 模块接口
100-150   WASM 加载函数
150-250   异步 API 函数
250-300   同步 API 函数
```

---

## 🔍 按功能查找代码

### 碰撞检测
```
AABB 碰撞:          geometry.h:60-70
圆形碰撞:          geometry.h:80-90
圆矩形碰撞:        geometry.h:100-120
坦克碰撞:          collision.cpp:1-50
子弹碰撞:          bullet-collision.cpp:150-250
```

### 物理计算
```
距离计算:          geometry.h:10-30
MBR 计算:          bullet-collision.cpp:40-60
爆炸扩散:          bullet-collision.cpp:60-100
运动计算:          bullet-collision.cpp:200-220
碰撞时间:          bullet-collision.cpp:100-180
```

### 游戏对象
```
坦克定义:          types.h:140-160
子弹定义:          bullet-collision.h:10-40
爆炸定义:          types.h:160-180
道具定义:          types.h:170-190
```

### 方向和速度
```
方向定义:          types.h:60-70
速度常数:          types.h:220-230
方向旋转:          bullet-collision.cpp:10-30
```

### 随机数
```
算法实现:          random.h:10-50
初始化:            random.cpp:10-20
API 导出:          bindings.cpp:80-100
```

---

## 📋 注释行的解读模式

### 类型注释
```cpp
enum class Direction : u32 {
  UP = 0,                // 向上方向
  //  ↑                   ↑
  //  值               说明含义
};
```

### 结构体注释
```cpp
struct Rect {
  f32 x;               // 矩形左上角X坐标
  //  ↑                ↑
  //  类型           用途说明
};
```

### 函数参数注释
```cpp
void someFunc(f32 threshold) {  // threshold: 碰撞阈值(-0.01)
  //                              ↑                ↑
  //                           参数名          说明+默认值
}
```

### 计算步骤注释
```cpp
f32 distance = std::sqrt(dx * dx + dy * dy);  // 返回欧几里得距离
// ↑                                          ↑
// 结果变量名                              计算说明
```

### 条件检查注释
```cpp
if (!is_in_field(tank_rect)) {  // 如果坦克不在场地内
  return false;                 // 返回false表示不能移动
  //     ↑                       ↑
  //   返回值                  含义说明
}
```

---

## 💡 常见概念解释

### MBR (最小包围矩形)
**用途**: 快速检测两个物体是否可能碰撞
**代码位置**: bullet-collision.cpp:40-60
**说明**: 包含运动路径的矩形,用于广泛阶段检测

### 碰撞时间
**用途**: 计算两颗子弹何时碰撞
**代码位置**: bullet-collision.cpp:100-180
**说明**: 使用数学公式,考虑速度和方向

### 连续碰撞检测
**用途**: 确保快速运动的物体不会穿过障碍物
**代码位置**: bullet-collision.cpp:150-200
**说明**: 使用上一帧和当前帧位置计算

### 爆炸范围
**用途**: 确定子弹击中时的伤害范围
**代码位置**: bullet-collision.cpp:60-100
**说明**: 垂直于飞行方向扩展以扩大效果

### 确定性随机
**用途**: 保证回放时游戏行为一致
**代码位置**: random.h 和 random.cpp
**说明**: 使用相同种子产生相同序列

---

## 🎯 学习路线图

### 第 1 周: 基础
- [ ] types.h - 理解所有类型
- [ ] geometry.h - 理解基本几何
- [ ] collision.h - 理解碰撞接口

### 第 2 周: 实现
- [ ] collision.cpp - 理解坦克碰撞
- [ ] bullet-collision.cpp (前半) - 理解 MBR 和爆炸
- [ ] bullet-collision.cpp (后半) - 理解碰撞时间计算

### 第 3 周: 集成
- [ ] bindings.cpp - 理解 WASM 导出
- [ ] wasm-engine.ts - 理解 TypeScript 接口
- [ ] random.h/cpp - 理解随机数

### 第 4 周: 应用
- [ ] 修改碰撞阈值
- [ ] 修改爆炸范围
- [ ] 修改坦克速度
- [ ] 运行测试验证

---

## 🐛 常见问题和对应代码位置

| 问题 | 查看位置 |
|------|---------|
| 坦克为什么穿过墙? | collision.cpp:1-50 |
| 子弹为什么穿过坦克? | bullet-collision.cpp:150-200 |
| 爆炸范围太小? | bullet-collision.cpp:60-100 |
| 碰撞不准确? | geometry.h:60-120 |
| 坦克移动卡顿? | collision.cpp:20-50 |
| 随机数不随机? | random.h:20-50 |
| JavaScript 调用太慢? | wasm-engine.ts:280+ |

---

## 🚀 快速修改指南

### 修改碰撞阈值
```
文件:       collision.h/cpp
参数:       threshold = -0.01f
含义:       允许微小重叠距离
位置:       can_tank_move 函数
```

### 修改爆炸范围
```
文件:       bullet-collision.cpp
常数:       BULLET_EXPLOSION_SPREAD = 4.0f
含义:       爆炸垂直方向的扩散距离
位置:       spread_bullet 函数
```

### 修改坦克速度
```
文件:       types.h
常数:       TANK_SPEED_BASIC = 64.0f
含义:       基础坦克的像素/秒速度
位置:       常数定义区域(第 220 行附近)
```

### 修改子弹速度
```
文件:       types.h
常数:       BULLET_SPEED = 128.0f
含义:       子弹的像素/秒速度
位置:       常数定义区域(第 220 行附近)
```

---

## 📊 代码复杂度概览

| 文件 | 复杂度 | 注释密度 | 学习时间 |
|------|--------|---------|---------|
| types.h | ⭐ | 高 | 30分钟 |
| geometry.h | ⭐ | 高 | 30分钟 |
| collision.h | ⭐⭐ | 高 | 1小时 |
| random.h | ⭐⭐ | 高 | 45分钟 |
| bullet-collision.h | ⭐⭐ | 高 | 1小时 |
| geometry.cpp | ⭐ | 中 | 10分钟 |
| collision.cpp | ⭐⭐ | 高 | 1小时 |
| bullet-collision.cpp | ⭐⭐⭐ | 高 | 3小时 |
| bindings.cpp | ⭐⭐ | 中 | 1小时 |
| wasm-engine.ts | ⭐ | 高 | 45分钟 |

---

🎓 **提示**: 使用 `Ctrl+F` (Windows/Linux) 或 `Cmd+F` (Mac) 快速搜索注释中的关键词!


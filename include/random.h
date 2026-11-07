#pragma once                        // 防止头文件被多次包含

#include "types.h"                  // 包含基本类型定义

/**
 * 采用Mulberry32算法的有种子伪随机数生成器
 * 确保确定性行为与TypeScript版本完全匹配
 * 输出范围: [0.0, 1.0)
 */
class SeededRandom {               // 定义随机数生成器类
private:                           // 私有部分
  u32 a;                           // 状态变量a(种子)
  
public:                            // 公开部分
  explicit SeededRandom(u32 seed) : a(seed) {}  // 构造函数:初始化种子值
  
  /**
   * 生成下一个随机数,范围[0.0, 1.0)
   * 必须与TypeScript版本的输出完全匹配
   * @return 范围在0.0到1.0之间的浮点数
   */
  f32 next() {                     // 生成下一个随机数函数
    a = (a + 0x6d2b79f5);          // 步骤1:状态值加上固定常数
    u32 t = a;                     // 步骤2:复制当前状态到临时变量
    t = ((t ^ (t >> 15)) * (t | 1));  // 步骤3:第一次XOR和乘法操作
    t ^= t + ((t ^ (t >> 7)) * (t | 61));  // 步骤4:第二次复杂变换
    return ((t ^ (t >> 14)) & 0xffffffff) / 4294967296.0f;  // 步骤5:最终变换并归一化到[0,1)
  }
  
  /**
   * 生成范围[min, max)内的随机整数
   * @param min 最小值(包含)
   * @param max 最大值(不包含)
   * @return 范围在min到max之间的随机整数
   */
  i32 next_int(i32 min, i32 max) {  // 生成随机整数函数
    return min + static_cast<i32>(next() * (max - min));  // 调用next()生成随机数并映射到范围
  }
  
  /**
   * 生成范围[min, max)内的随机浮点数
   * @param min 最小值(包含)
   * @param max 最大值(不包含)
   * @return 范围在min到max之间的随机浮点数
   */
  f32 next_float(f32 min, f32 max) {  // 生成随机浮点数函数
    return min + next() * (max - min);  // 调用next()生成随机数并映射到范围
  }
  
  /**
   * 获取当前种子状态(用于调试/日志)
   * @return 当前的种子状态值
   */
  u32 get_state() const {          // 获取状态函数(const表示不修改成员变量)
    return a;                      // 返回状态变量a的当前值
  }
};

/**
 * 全局随机数实例(根据需要用种子初始化)
 */
extern SeededRandom g_random;      // 声明全局随机数生成器对象(实现在.cpp文件中)

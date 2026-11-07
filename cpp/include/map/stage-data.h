// ============================================================================
// stage-data.h - 关卡数据结构和解析
// 
// 定义关卡配置的原始格式和解析后的完整数据结构。
// 支持从 JSON 格式解析关卡或直接加载预处理的关卡数据。
// ============================================================================

#ifndef STAGE_DATA_H
#define STAGE_DATA_H

#include "core/game-types.h"
#include "core/game-state.h"
#include <vector>
#include <string>
#include <array>

// ============================================================================
// 关卡原始配置 (JSON 格式解析后的数据)
// ============================================================================

// 原始关卡配置 (从 stage-*.json 直接映射)
struct RawStageConfig {
  std::string name;              // 关卡名称 (通常是数字字符串, 如 "1" 到 "35")
  bool custom;                   // 是否是自定义关卡
  i32 difficulty;                // 难度等级 (1-4, 影响 bot 战斗力)
  std::vector<std::string> map;  // 13×13 的地形编码字符串
  std::vector<std::string> bots; // bot 编队配置 (如 ["18*basic", "2*armor"])
};

// ============================================================================
// 砖块和钢块的精度编码
// ============================================================================

// 砖块子格是 4px (52×52 网格)，每个 16px 方格内的 4×4 砖块编码为 16bit
// 短格式: 单个十六进制数字 (0-f)
//   - 每一位代表一个 8×8 象限是否有砖块
//   - 0 表示象限全空, f 表示象限全满, 其他值表示象限内部分砖块
// 完整格式: 4 个十六进制数字
//   - 完整 16bit 编码, 直接写明 4×4 砖块的精确位置

// 解析砖块编码: 短格式或完整格式
// str: 1 个或 4 个十六进制数字的字符串
// returns: 16bit 编码，每一位对应一个砖块子格
u16 parseBrickHex(const std::string& str);

// 将砖块 16bit 编码按行展开到 52×52 网格中
// bricks: 目标布尔网格 (预先分配为 52×52 = 2704 个元素)
// row, col: 当前砖块所在的 13×13 方格坐标
// bits: 该砖块的 16bit 编码
void fillBrickGrid(std::vector<bool>& bricks, i32 row, i32 col, u16 bits);

// 将钢块 16bit 编码按行展开到 26×26 网格中
void fillSteelGrid(std::vector<bool>& steels, i32 row, i32 col, u16 bits);

// ============================================================================
// 解析后的完整关卡数据
// ============================================================================

// 解析后的关卡数据 (可用于游戏引擎)
struct ParsedStage {
  // 基本信息
  std::string name;           // 关卡名称
  i32 difficulty;             // 难度等级 (1-4)
  
  // 地形网格
  std::vector<bool> bricks;   // 52×52, true 表示该 4px 砖块存在
  std::vector<bool> steels;   // 26×26, true 表示该 8px 钢块存在
  std::vector<bool> rivers;   // 13×13, true 表示该方格是河
  std::vector<bool> snows;    // 13×13, true 表示该方格是雪
  std::vector<bool> forests;  // 13×13, true 表示该方格是森林
  
  // 老鹰 (保护目标)
  Point eaglePos;             // 老鹰左上角像素坐标 (-1, -1 表示无老鹰)
  
  // bot 编队配置
  std::vector<BotGroupConfig> bots;  // 本关要生成的所有 bot
  i32 totalBots;                     // 总共会生成多少个 bot
};

// ============================================================================
// 关卡解析器
// ============================================================================

// 从原始配置解析出完整的关卡数据
// rawConfig: 从 JSON 解析得到的原始配置
// returns: 解析后可直接用于游戏的关卡数据
ParsedStage parseStage(const RawStageConfig& rawConfig);

// 解析 bot 编队配置字符串 (如 "18*basic" 表示 18 个基础坦克)
// spec: 编队规格字符串
// returns: 编队配置，包含坦克等级和数量
BotGroupConfig parseBotGroup(const std::string& spec);

// ============================================================================
// 原始地图编码格式
// ============================================================================

// 地图编码使用符号表示不同地形:
//   'X' - 空地 (Empty)
//   'E' - 老鹰 (Eagle)
//   'B' - 砖块 (Brick) - 后面跟 1 或 4 个十六进制数字
//   'T' - 钢墙 (sTeEl) - 后面跟 1 或 4 个十六进制数字
//   'R' - 河流 (River)
//   'S' - 雪地 (Snow)
//   'F' - 森林 (Forest)

// 示例:
//   "X" 表示空地
//   "B1" 表示砖块 (短格式, 象限 0 满)
//   "Bffff" 表示砖块 (完整格式, 全满)
//   "T3" 表示钢墙

// ============================================================================
// 内置关卡常量
// ============================================================================

// Battle City NES 版本的 35 个经典关卡名称
namespace StageNames {
  constexpr const char* STAGE_1 = "1";
  constexpr const char* STAGE_35 = "35";
  // ... (可在运行时按数字索引访问)
  
  // 获取关卡名称 (1-35)
  const char* getStageName(i32 stageNumber);
}

// ============================================================================
// 关卡数据库
// ============================================================================

// 内置关卡集合
class StageDatabase {
public:
  // 加载所有内置关卡 (调用一次初始化)
  static void loadBuiltinStages();
  
  // 获取指定关卡的原始配置
  // stageName: 关卡名称字符串
  // returns: 指向原始配置的指针，未找到返回 nullptr
  static const RawStageConfig* getRawStage(const std::string& stageName);
  
  // 获取指定关卡的解析后数据
  static ParsedStage getParsedStage(const std::string& stageName);
  
  // 获取所有内置关卡总数
  static i32 getTotalStages();
  
  // 获取第 N 个关卡的名称 (按字典序)
  static std::string getStageName(i32 index);

private:
  // 这里会存储内置关卡数据
  static std::vector<RawStageConfig> builtinStages;
};

// ============================================================================
// 工具函数
// ============================================================================

// 检查字符串是否为有效的十六进制数
bool isValidHex(const std::string& str);

// 将字符串转换为十六进制值
u16 hexStringToInt(const std::string& str);

// 检查关卡配置的完整性
// rawConfig: 要检查的配置
// returns: 如果配置有效返回 true, 否则返回 false (可记录错误)
bool validateStageConfig(const RawStageConfig& rawConfig);

// ============================================================================
// 关卡地图渲染 (用于测试和调试)
// ============================================================================

// 将解析的关卡渲染成可视化的地图字符串 (用于快照和调试)
// parsed: 解析后的关卡
// returns: 52 行字符串，每行 52 个字符
//   '#' - 砖块
//   '@' - 钢块
//   '~' - 河流
//   ':' - 雪地
//   '%' - 森林
//   '.' - 空地
std::vector<std::string> renderStageMap(const ParsedStage& parsed);

#endif  // STAGE_DATA_H

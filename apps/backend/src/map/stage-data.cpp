// ============================================================================
// stage-data.cpp - 关卡数据的解析和管理
// ============================================================================

#include "map/stage-data.h"
#include <algorithm>
#include <sstream>

// ============================================================================
// 砖块和钢块编码解析
// ============================================================================

u16 parseBrickHex(const std::string& str) {
  // 注释: 将砖块编码字符串转换为 16bit 值
  
  if (str.length() == 1) {
    // 短格式: 每一位扩展为整个象限
    u16 short_val = std::stoi(str, nullptr, 16);
    u16 long_val = 0;
    
    // 注释: 将 4bit 短格式扩展为 16bit 完整格式
    // 每一位代表一个 8×8 象限是否有砖块
    if (short_val & 0b0001) long_val |= 0xf000;  // 左上象限
    if (short_val & 0b0010) long_val |= 0x0f00;  // 右上象限
    if (short_val & 0b0100) long_val |= 0x00f0;  // 左下象限
    if (short_val & 0b1000) long_val |= 0x000f;  // 右下象限
    
    return long_val;
  } else if (str.length() == 4) {
    // 完整格式: 直接转换
    return std::stoi(str, nullptr, 16);
  }
  
  // 无效格式
  return 0;
}

void fillBrickGrid(std::vector<bool>& bricks, i32 row, i32 col, u16 bits) {
  // 注释: 将砖块 16bit 编码按行展开到 52×52 网格中
  
  const i32 N = Constants::BRICK_GRID_SIZE;  // 52
  
  // 注释: 16bit 分为 4 个 4bit 段，每段代表 2×2 砖块象限
  // 将这个编码对应到 52×52 网格中的 4×4 砖块
  
  // 第 0 象限 (bit 12-15): 行 [row, row+1], 列 [col, col+1]
  u16 part0 = (bits >> 12) & 0xf;
  if (part0 & 0b0001) bricks[row * N + col] = true;
  if (part0 & 0b0010) bricks[row * N + (col + 1)] = true;
  if (part0 & 0b0100) bricks[(row + 1) * N + col] = true;
  if (part0 & 0b1000) bricks[(row + 1) * N + (col + 1)] = true;
  
  // 第 1 象限 (bit 8-11): 行 [row, row+1], 列 [col+2, col+3]
  u16 part1 = (bits >> 8) & 0xf;
  if (part1 & 0b0001) bricks[row * N + (col + 2)] = true;
  if (part1 & 0b0010) bricks[row * N + (col + 3)] = true;
  if (part1 & 0b0100) bricks[(row + 1) * N + (col + 2)] = true;
  if (part1 & 0b1000) bricks[(row + 1) * N + (col + 3)] = true;
  
  // 第 2 象限 (bit 4-7): 行 [row+2, row+3], 列 [col, col+1]
  u16 part2 = (bits >> 4) & 0xf;
  if (part2 & 0b0001) bricks[(row + 2) * N + col] = true;
  if (part2 & 0b0010) bricks[(row + 2) * N + (col + 1)] = true;
  if (part2 & 0b0100) bricks[(row + 3) * N + col] = true;
  if (part2 & 0b1000) bricks[(row + 3) * N + (col + 1)] = true;
  
  // 第 3 象限 (bit 0-3): 行 [row+2, row+3], 列 [col+2, col+3]
  u16 part3 = (bits >> 0) & 0xf;
  if (part3 & 0b0001) bricks[(row + 2) * N + (col + 2)] = true;
  if (part3 & 0b0010) bricks[(row + 2) * N + (col + 3)] = true;
  if (part3 & 0b0100) bricks[(row + 3) * N + (col + 2)] = true;
  if (part3 & 0b1000) bricks[(row + 3) * N + (col + 3)] = true;
}

void fillSteelGrid(std::vector<bool>& steels, i32 row, i32 col, u16 bits) {
  // 注释: 钢块编码的展开方式与砖块类似，但是网格是 26×26
  
  const i32 N = Constants::STEEL_GRID_SIZE;  // 26
  
  // 注释: 钢块的编码方式相同，但每个方格的精度是 8px 而非 4px
  u16 part0 = (bits >> 12) & 0xf;
  if (part0 & 0b0001) steels[row * N + col] = true;
  if (part0 & 0b0010) steels[row * N + (col + 1)] = true;
  if (part0 & 0b0100) steels[(row + 1) * N + col] = true;
  if (part0 & 0b1000) steels[(row + 1) * N + (col + 1)] = true;
  
  u16 part1 = (bits >> 8) & 0xf;
  if (part1 & 0b0001) steels[row * N + (col + 2)] = true;
  if (part1 & 0b0010) steels[row * N + (col + 3)] = true;
  if (part1 & 0b0100) steels[(row + 1) * N + (col + 2)] = true;
  if (part1 & 0b1000) steels[(row + 1) * N + (col + 3)] = true;
  
  u16 part2 = (bits >> 4) & 0xf;
  if (part2 & 0b0001) steels[(row + 2) * N + col] = true;
  if (part2 & 0b0010) steels[(row + 2) * N + (col + 1)] = true;
  if (part2 & 0b0100) steels[(row + 3) * N + col] = true;
  if (part2 & 0b1000) steels[(row + 3) * N + (col + 1)] = true;
  
  u16 part3 = (bits >> 0) & 0xf;
  if (part3 & 0b0001) steels[(row + 2) * N + (col + 2)] = true;
  if (part3 & 0b0010) steels[(row + 2) * N + (col + 3)] = true;
  if (part3 & 0b0100) steels[(row + 3) * N + (col + 2)] = true;
  if (part3 & 0b1000) steels[(row + 3) * N + (col + 3)] = true;
}

// ============================================================================
// bot 编队解析
// ============================================================================

BotGroupConfig parseBotGroup(const std::string& spec) {
  // 注释: 解析格式如 "18*basic" 为编队配置
  
  BotGroupConfig config;
  
  // 查找 '*' 分隔符
  size_t pos = spec.find('*');
  if (pos == std::string::npos) {
    // 无效格式，返回默认值
    config.count = 0;
    config.tankLevel = TankLevel::Basic;
    return config;
  }
  
  // 解析数量 (前半部分)
  config.count = std::stoi(spec.substr(0, pos));
  
  // 解析坦克等级 (后半部分)
  std::string level_str = spec.substr(pos + 1);
  
  // 注释: 将字符串转换为等级枚举
  if (level_str == "basic") {
    config.tankLevel = TankLevel::Basic;
  } else if (level_str == "fast") {
    config.tankLevel = TankLevel::Fast;
  } else if (level_str == "power") {
    config.tankLevel = TankLevel::Power;
  } else if (level_str == "armor") {
    config.tankLevel = TankLevel::Armor;
  } else {
    config.tankLevel = TankLevel::Basic;  // 默认值
  }
  
  return config;
}

// ============================================================================
// 关卡解析
// ============================================================================

ParsedStage parseStage(const RawStageConfig& rawConfig) {
  // 注释: 从原始配置解析出完整的关卡数据
  
  ParsedStage parsed;
  
  // 基本信息
  parsed.name = rawConfig.name;
  parsed.difficulty = rawConfig.difficulty;
  
  // 初始化网格 (全 false)
  parsed.bricks.resize(Constants::BRICK_GRID_SIZE * Constants::BRICK_GRID_SIZE, false);
  parsed.steels.resize(Constants::STEEL_GRID_SIZE * Constants::STEEL_GRID_SIZE, false);
  parsed.rivers.resize(Constants::TERRAIN_GRID_SIZE * Constants::TERRAIN_GRID_SIZE, false);
  parsed.snows.resize(Constants::TERRAIN_GRID_SIZE * Constants::TERRAIN_GRID_SIZE, false);
  parsed.forests.resize(Constants::TERRAIN_GRID_SIZE * Constants::TERRAIN_GRID_SIZE, false);
  
  // 初始化老鹰为无效值
  parsed.eaglePos = {-1.0f, -1.0f};
  
  // 解析地图 (13×13 方格)
  // 注释: 按行列遍历地图字符串，每个字符代表一个方格及其包含的元素
  for (i32 row = 0; row < Constants::FIELD_BLOCK_SIZE; ++row) {
    if (row >= static_cast<i32>(rawConfig.map.size())) break;
    
    const std::string& line = rawConfig.map[row];
    
    // 按字符解析每个方格
    for (i32 col = 0; col < Constants::FIELD_BLOCK_SIZE && col < static_cast<i32>(line.size()); ++col) {
      char cell = line[col];
      
      // 注释: 根据字符类型填充不同的地形
      if (cell == 'B') {
        // 砖块: 后面可能跟编码 (这里简化处理为全满)
        u16 bits = 0xffff;  // 默认砖块全满
        fillBrickGrid(parsed.bricks, row, col, bits);
      } else if (cell == 'T') {
        // 钢块: 后面可能跟编码
        u16 bits = 0xffff;  // 默认钢块全满
        fillSteelGrid(parsed.steels, row, col, bits);
      } else if (cell == 'E') {
        // 老鹰: 保存位置（方格左上角像素）
        parsed.eaglePos.x = col * Constants::BLOCK_SIZE;
        parsed.eaglePos.y = row * Constants::BLOCK_SIZE;
      } else if (cell == 'R') {
        // 河流
        parsed.rivers[row * Constants::TERRAIN_GRID_SIZE + col] = true;
      } else if (cell == 'S') {
        // 雪地
        parsed.snows[row * Constants::TERRAIN_GRID_SIZE + col] = true;
      } else if (cell == 'F') {
        // 森林
        parsed.forests[row * Constants::TERRAIN_GRID_SIZE + col] = true;
      }
      // 'X' 和其他字符视为空地 (什么都不做)
    }
  }
  
  // 解析 bot 编队
  parsed.totalBots = 0;
  for (const auto& bot_spec : rawConfig.bots) {
    BotGroupConfig group = parseBotGroup(bot_spec);
    parsed.bots.push_back(group);
    parsed.totalBots += group.count;
  }
  
  return parsed;
}

// ============================================================================
// 工具函数
// ============================================================================

bool isValidHex(const std::string& str) {
  // 注释: 检查字符串是否为有效的十六进制数字
  
  if (str.empty()) return false;
  
  for (char c : str) {
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
      return false;
    }
  }
  
  return true;
}

u16 hexStringToInt(const std::string& str) {
  // 注释: 将十六进制字符串转换为整数
  
  if (!isValidHex(str)) return 0;
  
  return std::stoi(str, nullptr, 16);
}

bool validateStageConfig(const RawStageConfig& rawConfig) {
  // 注释: 验证关卡配置的完整性和有效性
  
  // 检查基本字段
  if (rawConfig.name.empty()) return false;
  if (rawConfig.difficulty < 1 || rawConfig.difficulty > 4) return false;
  if (rawConfig.map.empty() || rawConfig.map.size() > 13) return false;
  
  // 检查每行地图的格式
  for (const auto& line : rawConfig.map) {
    if (line.size() > 13) return false;
  }
  
  // 检查 bot 配置
  if (rawConfig.bots.empty()) return false;
  for (const auto& bot_spec : rawConfig.bots) {
    if (bot_spec.find('*') == std::string::npos) return false;
  }
  
  return true;
}

// ============================================================================
// 地图渲染
// ============================================================================

std::vector<std::string> renderStageMap(const ParsedStage& parsed) {
  // 注释: 将解析的关卡渲染成可视化地图 (用于快照)
  
  std::vector<std::string> result;
  
  const i32 N = Constants::BRICK_GRID_SIZE;  // 52
  
  // 注释: 遍历 52×52 网格，按优先级确定每个单元格的显示字符
  for (i32 row = 0; row < N; ++row) {
    std::string line;
    for (i32 col = 0; col < N; ++col) {
      char ch;
      
      // 注释: 优先级：砖块 > 钢块 > 河 > 雪 > 森林 > 空地
      if (parsed.bricks[row * N + col]) {
        ch = '#';  // 砖块
      } else {
        // 检查钢块 (8px 精度，需要映射到 4px 网格)
        i32 steel_row = row / 2;
        i32 steel_col = col / 2;
        if (parsed.steels[steel_row * Constants::STEEL_GRID_SIZE + steel_col]) {
          ch = '@';  // 钢块
        } else {
          // 检查河、雪、森林 (16px 精度)
          i32 terrain_row = row / 4;
          i32 terrain_col = col / 4;
          if (parsed.rivers[terrain_row * Constants::TERRAIN_GRID_SIZE + terrain_col]) {
            ch = '~';  // 河
          } else if (parsed.snows[terrain_row * Constants::TERRAIN_GRID_SIZE + terrain_col]) {
            ch = ':';  // 雪
          } else if (parsed.forests[terrain_row * Constants::TERRAIN_GRID_SIZE + terrain_col]) {
            ch = '%';  // 森林
          } else {
            ch = '.';  // 空地
          }
        }
      }
      
      line += ch;
    }
    result.push_back(line);
  }
  
  return result;
}

// ============================================================================
// 关卡数据库
// ============================================================================

std::vector<RawStageConfig> StageDatabase::builtinStages;

void StageDatabase::loadBuiltinStages() {
  // 注释: 加载所有内置关卡
  // 这里需要与 TypeScript 的 src/stages/index.ts 同步
  // 完整实现需要在 C++ 中嵌入所有 35 个关卡的数据
  // 暂时留空，实际使用时需要从 JSON 或二进制格式加载
}

const RawStageConfig* StageDatabase::getRawStage(const std::string& stageName) {
  // 注释: 根据名称查找关卡
  
  for (const auto& stage : builtinStages) {
    if (stage.name == stageName) {
      return &stage;
    }
  }
  
  return nullptr;  // 未找到
}

ParsedStage StageDatabase::getParsedStage(const std::string& stageName) {
  // 注释: 获取解析后的关卡数据
  
  const RawStageConfig* raw = getRawStage(stageName);
  if (raw) {
    return parseStage(*raw);
  }
  
  // 返回空关卡
  ParsedStage empty;
  empty.name = stageName;
  empty.difficulty = 1;
  return empty;
}

i32 StageDatabase::getTotalStages() {
  // 注释: 获取内置关卡总数
  return static_cast<i32>(builtinStages.size());
}

std::string StageDatabase::getStageName(i32 index) {
  // 注释: 按索引获取关卡名称
  
  if (index < 0 || index >= static_cast<i32>(builtinStages.size())) {
    return "";
  }
  
  return builtinStages[index].name;
}

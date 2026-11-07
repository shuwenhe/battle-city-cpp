#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "../external/httplib.h"
#include "../include/core/game-types.h"
#include "../include/core/game-state.h"
#include "../include/core/game-logic.h"

// 简单的 JSON 生成函数（避免依赖 nlohmann）
std::string json_object(const std::string& content) {
    return "{" + content + "}";
}

std::string json_number(const std::string& key, int value) {
    return "\"" + key + "\":" + std::to_string(value);
}

std::string json_number(const std::string& key, float value) {
    std::ostringstream oss;
    oss << value;
    return "\"" + key + "\":" + oss.str();
}

std::string json_string(const std::string& key, const std::string& value) {
    return "\"" + key + "\":\"" + value + "\"";
}

std::string json_bool(const std::string& key, bool value) {
    return "\"" + key + "\":" + (value ? "true" : "false");
}

std::string json_array_start(const std::string& key) {
    return "\"" + key + "\":[";
}

std::string json_array_end() {
    return "]";
}

// 游戏状态管理
class GameServer {
public:
    GameServer() : port_(3000), running_(false) {
        // 初始化游戏状态
        isGameOver_ = false;
        isPaused_ = false;
        currentStage_ = 1;
        score1_ = 0;
        score2_ = 0;
        kills_ = 0;
        gameTime_ = 0;
    }
    
    void start() {
        running_ = true;
        
        // 在后台线程中启动服务器
        std::thread([this]() {
            httplib::Server svr;
            
            // 获取游戏状态
            svr.Get("/api/game-state", [this](const httplib::Request&, httplib::Response& res) {
                std::lock_guard<std::mutex> lock(stateMutex_);
                
                // 输出游戏数据到控制台
                std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
                std::cout << "📊 游戏状态查询" << std::endl;
                std::cout << "   舞台: " << currentStage_ << " | 时间: " << (int)gameTime_ << "s" << std::endl;
                std::cout << "   玩家1分数: " << score1_ << " | 玩家2分数: " << score2_ << " | 击杀: " << kills_ << std::endl;
                std::cout << "   游戏状态: " << (isPaused_ ? "⏸️  暂停" : (isGameOver_ ? "❌ 结束" : "▶️  运行中")) << std::endl;
                std::cout << std::endl;
                
                // 输出坦克信息
                std::cout << "🛫 坦克数量: " << tanks_.size() << std::endl;
                for (size_t i = 0; i < tanks_.size(); ++i) {
                    const Tank& t = tanks_[i];
                    std::cout << "   [" << i << "] ID=" << t.tankId 
                              << " 位置=(" << (int)t.x << "," << (int)t.y << ")"
                              << " 方向=" << (int)t.direction
                              << " 阵营=" << (t.side == TankSide::Player ? "玩家" : "敌人")
                              << " 状态=" << (t.alive ? "✓存活" : "✗已死") << std::endl;
                }
                std::cout << std::endl;
                
                // 输出子弹信息
                std::cout << "💣 子弹数量: " << bullets_.size() << std::endl;
                for (size_t i = 0; i < bullets_.size(); ++i) {
                    const Bullet& b = bullets_[i];
                    std::cout << "   [" << i << "] 位置=(" << (int)b.x << "," << (int)b.y << ")"
                              << " 方向=" << (int)b.direction
                              << " 来自坦克=" << b.tankId << std::endl;
                }
                std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" << std::endl;
                
            // 构建坦克数组
                std::string tanks = json_array_start("tanks");
                for (size_t i = 0; i < tanks_.size(); ++i) {
                    if (i > 0) tanks += ",";
                    const Tank& t = tanks_[i];
                    tanks += json_object(
                        json_number("id", t.tankId) + "," +
                        json_number("x", (int)t.x) + "," +
                        json_number("y", (int)t.y) + "," +
                        json_number("direction", (int)t.direction) + "," +
                        json_string("side", t.side == TankSide::Player ? "player" : "bot") + "," +
                        json_bool("alive", t.alive)
                    );
                }
                tanks += json_array_end();
                
                // 构建子弹数组
                std::string bullets = json_array_start("bullets");
                for (size_t i = 0; i < bullets_.size(); ++i) {
                    if (i > 0) bullets += ",";
                    const Bullet& b = bullets_[i];
                    bullets += json_object(
                        json_number("x", (int)b.x) + "," +
                        json_number("y", (int)b.y) + "," +
                        json_number("direction", (int)b.direction) + "," +
                        json_number("tankId", b.tankId)
                    );
                }
                bullets += json_array_end();
                
                // 构建完整响应
                std::string state = json_object(
                    tanks + "," +
                    bullets + "," +
                    json_array_start("map") + json_array_end() + "," +
                    "\"score\":{" + json_number("player1", score1_) + "," + json_number("player2", score2_) + "," +
                    json_number("kills", kills_) + "," + json_number("stage", currentStage_) + "}," +
                    json_number("gameTime", (int)gameTime_) + "," +
                    json_bool("gameOver", isGameOver_) + "," +
                    json_bool("paused", isPaused_)
                );
                res.set_content(state, "application/json");
            });
            
            // 发送玩家输入给后端
            svr.Post("/api/game-input", [this](const httplib::Request& req, httplib::Response& res) {
                try {
                    processInput(req.body);
                    res.set_content(R"({"ok":true})", "application/json");
                } catch (const std::exception& e) {
                    res.set_content(R"({"error":")" + std::string(e.what()) + R"("})", "application/json");
                    res.status = 400;
                }
            });
            
            // 启动游戏
            svr.Post("/api/game/start", [this](const httplib::Request& req, httplib::Response& res) {
                try {
                    startGame(req.body);
                    res.set_content(R"({"ok":true})", "application/json");
                } catch (const std::exception& e) {
                    res.status = 400;
                    res.set_content(R"({"error":")" + std::string(e.what()) + R"("})", "application/json");
                }
            });
            
            // 暂停/继续
            svr.Post("/api/game/pause", [this](const httplib::Request&, httplib::Response& res) {
                std::lock_guard<std::mutex> lock(stateMutex_);
                isPaused_ = !isPaused_;
                std::string response = json_object(json_bool("paused", isPaused_));
                res.set_content(response, "application/json");
            });
            
            // 健康检查
            svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
                res.set_content(R"({"status":"ok"})", "application/json");
            });
            
            // 启用 CORS
            svr.set_post_routing_handler([](const httplib::Request&, httplib::Response& res) {
                res.set_header("Access-Control-Allow-Origin", "*");
                res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
                res.set_header("Access-Control-Allow-Headers", "Content-Type");
            });
            
            std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
            std::cout << "║ 🎮 Battle City C++ 服务器已启动                       ║" << std::endl;
            std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
            std::cout << "║ 📍 端口: " << port_ << "                                               ║" << std::endl;
            std::cout << "║ 🔍 健康检查: http://localhost:" << port_ << "/health                  ║" << std::endl;
            std::cout << "║ 📊 游戏状态: http://localhost:" << port_ << "/api/game-state           ║" << std::endl;
            std::cout << "║ 🎮 开始游戏: POST http://localhost:" << port_ << "/api/game/start        ║" << std::endl;
            std::cout << "║ ⏸️  暂停游戏: POST http://localhost:" << port_ << "/api/game/pause        ║" << std::endl;
            std::cout << "║ ⌨️  游戏输入: POST http://localhost:" << port_ << "/api/game-input        ║" << std::endl;
            std::cout << "╚════════════════════════════════════════════════════════╝\n" << std::endl;
            
            if (!svr.listen("0.0.0.0", port_)) {
                std::cerr << "❌ Failed to start server" << std::endl;
            }
        }).detach();
        
        // 给服务器启动时间
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    void startGame(const std::string& params) {
        std::lock_guard<std::mutex> lock(stateMutex_);
        
        int stageId = 1;
        size_t pos = params.find("\"stageId\":");
        if (pos != std::string::npos) {
            stageId = std::stoi(params.substr(pos + 10));
        }
        
        isGameOver_ = false;
        isPaused_ = false;
        currentStage_ = stageId;
        gameTime_ = 0;
        score1_ = 0;
        score2_ = 0;
        kills_ = 0;
        
        // 初始化游戏对象
        tanks_.clear();
        bullets_.clear();
        
        // 创建玩家坦克
        Tank player1(1, TankSide::Player, TankLevel::Basic, TankColor::Yellow, 64, 256, Direction::Up);
        tanks_.push_back(player1);
        
        // 输出游戏启动日志
        std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║ 🎮 游戏已启动                                          ║" << std::endl;
        std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
        std::cout << "║ 舞台: " << stageId << "                                             ║" << std::endl;
        std::cout << "║ 玩家坦克数: " << tanks_.size() << "                                          ║" << std::endl;
        std::cout << "║ 初始位置: (64, 256)                                   ║" << std::endl;
        std::cout << "║ 初始方向: 向上                                        ║" << std::endl;
        std::cout << "╚════════════════════════════════════════════════════════╝\n" << std::endl;
    }
    
    void processInput(const std::string& input) {
        std::lock_guard<std::mutex> lock(stateMutex_);
        
        // 解析输入类型
        std::string inputType = "未知";
        if (input.find("\"direction\"") != std::string::npos) {
            inputType = "移动";
        } else if (input.find("\"fire\"") != std::string::npos) {
            inputType = "射击";
        }
        
        std::cout << "⌨️  玩家输入: " << inputType << " | 原始数据: " << input << std::endl;
    }
    
    bool isGameOver() const { return isGameOver_; }
    
private:
    int port_;
    bool running_;
    bool isPaused_;
    bool isGameOver_;
    
    int score1_;
    int score2_;
    int kills_;
    int currentStage_;
    float gameTime_;
    
    std::vector<Tank> tanks_;
    std::vector<Bullet> bullets_;
    
    std::mutex stateMutex_;
};

int main() {
    GameServer server;
    server.start();
    return 0;
}

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <nlohmann/json.hpp>
#include "../external/httplib.h"
#include "engine/types.ts"

using json = nlohmann::json;

// 游戏状态管理
class GameServer {
public:
    GameServer() : port_(3000), running_(false) {}
    
    void start() {
        running_ = true;
        
        httplib::Server svr;
        
        // WebSocket 端点：获取游戏状态
        svr.Get("/api/game-state", [this](const httplib::Request&, httplib::Response& res) {
            json state = {
                {"tanks", getTanksState()},
                {"bullets", getBulletsState()},
                {"map", getMapState()},
                {"score", getScoreState()},
                {"gameOver", isGameOver_}
            };
            res.set_content(state.dump(), "application/json");
        });
        
        // 发送玩家输入给后端
        svr.Post("/api/game-input", [this](const httplib::Request& req, httplib::Response& res) {
            try {
                auto input = json::parse(req.body);
                processInput(input);
                res.set_content(R"({"ok":true})", "application/json");
            } catch (const std::exception& e) {
                res.set_content(R"({"error":")" + std::string(e.what()) + R"("})", "application/json");
                res.status = 400;
            }
        });
        
        // 启动游戏
        svr.Post("/api/game/start", [this](const httplib::Request& req, httplib::Response& res) {
            try {
                auto params = json::parse(req.body);
                startGame(params);
                res.set_content(R"({"ok":true})", "application/json");
            } catch (const std::exception& e) {
                res.status = 400;
                res.set_content(R"({"error":")" + std::string(e.what()) + R"("})", "application/json");
            }
        });
        
        // 暂停/继续
        svr.Post("/api/game/pause", [this](const httplib::Request&, httplib::Response& res) {
            isPaused_ = !isPaused_;
            res.set_content(json{{"paused", isPaused_}}.dump(), "application/json");
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
        
        std::cout << "🎮 Battle City C++ Server starting on port " << port_ << std::endl;
        
        if (!svr.listen("0.0.0.0", port_)) {
            std::cerr << "❌ Failed to start server" << std::endl;
            return;
        }
    }
    
    void startGame(const json& params) {
        int stageId = params.value("stageId", 1);
        int mode = params.value("mode", 0); // 0: 1P, 1: 2P
        isGameOver_ = false;
        currentStage_ = stageId;
        std::cout << "🎮 Game started: Stage " << stageId << ", Mode: " << mode << std::endl;
    }
    
    void processInput(const json& input) {
        int playerId = input.value("playerId", 0);
        std::string action = input.value("action", "");
        
        // 处理玩家 1 或玩家 2 的输入
        if (playerId == 0) {
            player1Input_ = action;
        } else if (playerId == 1) {
            player2Input_ = action;
        }
    }
    
    json getTanksState() {
        // 返回所有坦克的状态
        json tanks = json::array();
        // TODO: 连接真实游戏引擎数据
        return tanks;
    }
    
    json getBulletsState() {
        json bullets = json::array();
        // TODO: 返回所有子弹数据
        return bullets;
    }
    
    json getMapState() {
        json mapData = json::array();
        // TODO: 返回地图数据
        return mapData;
    }
    
    json getScoreState() {
        return json{
            {"player1", score1_},
            {"player2", score2_},
            {"kills", kills_},
            {"stage", currentStage_}
        };
    }
    
    bool isGameOver() const { return isGameOver_; }
    
private:
    int port_;
    bool running_;
    bool isPaused_ = false;
    bool isGameOver_ = false;
    
    int score1_ = 0;
    int score2_ = 0;
    int kills_ = 0;
    int currentStage_ = 1;
    
    std::string player1Input_;
    std::string player2Input_;
    
    std::mutex stateMutex_;
};

int main() {
    GameServer server;
    server.start();
    return 0;
}

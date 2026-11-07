// Frontend API to communicate with C++ backend
// 与 C++ 后端通信的 API

const API_BASE = process.env.REACT_APP_API_URL || 'http://localhost:3000';

export interface GameState {
  tanks: any[];
  bullets: any[];
  map: any[];
  score: {
    player1: number;
    player2: number;
    kills: number;
    stage: number;
  };
  gameOver: boolean;
}

export interface GameInput {
  playerId: number;
  action: string; // 'up' | 'down' | 'left' | 'right' | 'fire'
}

class GameAPI {
  private baseUrl: string;
  
  constructor(baseUrl = API_BASE) {
    this.baseUrl = baseUrl;
  }
  
  /**
   * 健康检查 - 验证服务器是否运行
   */
  async health(): Promise<boolean> {
    try {
      const res = await fetch(`${this.baseUrl}/health`);
      return res.ok;
    } catch (err) {
      console.error('❌ Server health check failed:', err);
      return false;
    }
  }
  
  /**
   * 获取当前游戏状态
   */
  async getGameState(): Promise<GameState> {
    try {
      const res = await fetch(`${this.baseUrl}/api/game-state`);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      return await res.json();
    } catch (err) {
      console.error('❌ Failed to get game state:', err);
      throw err;
    }
  }
  
  /**
   * 发送玩家输入
   */
  async sendInput(input: GameInput): Promise<void> {
    try {
      const res = await fetch(`${this.baseUrl}/api/game-input`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(input)
      });
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
    } catch (err) {
      console.error('❌ Failed to send input:', err);
      throw err;
    }
  }
  
  /**
   * 启动游戏
   */
  async startGame(stageId: number, mode: number = 0): Promise<void> {
    try {
      const res = await fetch(`${this.baseUrl}/api/game/start`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ stageId, mode })
      });
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
    } catch (err) {
      console.error('❌ Failed to start game:', err);
      throw err;
    }
  }
  
  /**
   * 暂停/继续游戏
   */
  async togglePause(): Promise<boolean> {
    try {
      const res = await fetch(`${this.baseUrl}/api/game/pause`, {
        method: 'POST'
      });
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      const data = await res.json();
      return data.paused;
    } catch (err) {
      console.error('❌ Failed to toggle pause:', err);
      throw err;
    }
  }
}

export default new GameAPI();

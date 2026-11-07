// React Hook for game engine backend communication
import { useEffect, useRef, useState, useCallback } from 'react';
import gameApi, { GameState, GameInput } from '../api/gameApi';

export function useGameBackend() {
  const [gameState, setGameState] = useState<GameState | null>(null);
  const [connected, setConnected] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const gameLoopRef = useRef<NodeJS.Timeout | null>(null);
  
  // 连接到后端
  useEffect(() => {
    let isMounted = true;
    
    const checkConnection = async () => {
      try {
        const isHealthy = await gameApi.health();
        if (isMounted) {
          setConnected(isHealthy);
          if (!isHealthy) {
            setError('Cannot connect to game server at http://localhost:3000');
          } else {
            setError(null);
          }
        }
      } catch (err) {
        if (isMounted) {
          setError(String(err));
          setConnected(false);
        }
      }
    };
    
    checkConnection();
    const interval = setInterval(checkConnection, 2000); // 每2秒检查一次
    
    return () => {
      isMounted = false;
      clearInterval(interval);
    };
  }, []);
  
  // 游戏循环 - 定时获取游戏状态
  useEffect(() => {
    if (!connected) return;
    
    const fetchGameState = async () => {
      try {
        const state = await gameApi.getGameState();
        setGameState(state);
      } catch (err) {
        console.error('Error fetching game state:', err);
      }
    };
    
    gameLoopRef.current = setInterval(fetchGameState, 1000 / 60); // 60 FPS
    
    return () => {
      if (gameLoopRef.current) clearInterval(gameLoopRef.current);
    };
  }, [connected]);
  
  // 发送输入
  const sendInput = useCallback(async (playerId: number, action: string) => {
    if (!connected) {
      console.warn('Not connected to server');
      return;
    }
    
    try {
      await gameApi.sendInput({ playerId, action });
    } catch (err) {
      console.error('Error sending input:', err);
    }
  }, [connected]);
  
  // 启动游戏
  const startGame = useCallback(async (stageId: number, mode: number = 0) => {
    if (!connected) {
      console.warn('Not connected to server');
      return;
    }
    
    try {
      await gameApi.startGame(stageId, mode);
    } catch (err) {
      console.error('Error starting game:', err);
      setError(String(err));
    }
  }, [connected]);
  
  // 暂停/继续
  const togglePause = useCallback(async () => {
    if (!connected) {
      console.warn('Not connected to server');
      return;
    }
    
    try {
      const isPaused = await gameApi.togglePause();
      console.log('Game paused:', isPaused);
    } catch (err) {
      console.error('Error toggling pause:', err);
    }
  }, [connected]);
  
  return {
    gameState,
    connected,
    error,
    sendInput,
    startGame,
    togglePause
  };
}

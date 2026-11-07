/**
 * 游戏引擎类型定义
 */

export type TankLevel = 1 | 2 | 3 | 4;

export type Direction = 'up' | 'down' | 'left' | 'right' | 'none';

export interface Position {
  x: number;
  y: number;
}

export interface Tank {
  id: string;
  x: number;
  y: number;
  direction: Direction;
  level: TankLevel;
  health: number;
}

export interface Bullet {
  id: string;
  x: number;
  y: number;
  direction: Direction;
  speed: number;
  power: number;
}

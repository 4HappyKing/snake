#ifndef __SNAKE_H__
#define __SNAKE_H__

// 坐标
struct Position {
	int x;
	int y;
};

// 链表结点
struct Node {
	struct Position position;
	struct Node *pNext;
};

// 蛇朝向
enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// 蛇结构体
struct Snake {
	enum Direction direction;
	struct Node *pBody;
};

// 游戏结构体
struct Game {
	int width;	// 游戏宽度（不包括墙）
	int height;	// 游戏高度（不包括墙）
	int score;	// 当前得分
	int scorePerFood;	// 每个食物得分

	struct Snake snake;	// 蛇
	struct Position foodPosition;	// 当前食物坐标
};

enum ExitStatus {
	QUIT,	// 正常退出
	KILLED_BY_WALL,	// 撞墙了
	KILLED_BY_SELF	// 撞自己了
};

#endif
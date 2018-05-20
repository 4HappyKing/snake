#ifndef __SNAKE_H__
#define __SNAKE_H__

// Coordinate
struct Position {
	int x;
	int y;
};

// Linked list node
struct Node {
	struct Position position;
	struct Node *pNext;
};

// Snake direction
enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// Snake structure
struct Snake {
	enum Direction direction;
	struct Node *pBody;
};

// Game structure
struct Game {
	int width;	       // Game width
	int height;	       // Game height
	int score;	       // Current score
	int scorePerFood;  // Score per food

	struct Snake snake;	            // Snake
	struct Position foodPosition;	// Current food coordinates
};

enum ExitStatus {
	QUIT,	         // Normal exit
	KILLED_BY_WALL,	 // Hit the wall
	KILLED_BY_SELF	 // Hit yourself
};

#endif
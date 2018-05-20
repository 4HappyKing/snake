#ifndef __SNAKE_H__
#define __SNAKE_H__

// ����
struct Position {
	int x;
	int y;
};

// ������
struct Node {
	struct Position position;
	struct Node *pNext;
};

// �߳���
enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// �߽ṹ��
struct Snake {
	enum Direction direction;
	struct Node *pBody;
};

// ��Ϸ�ṹ��
struct Game {
	int width;	// ��Ϸ��ȣ�������ǽ��
	int height;	// ��Ϸ�߶ȣ�������ǽ��
	int score;	// ��ǰ�÷�
	int scorePerFood;	// ÿ��ʳ��÷�

	struct Snake snake;	// ��
	struct Position foodPosition;	// ��ǰʳ������
};

enum ExitStatus {
	QUIT,	// �����˳�
	KILLED_BY_WALL,	// ײǽ��
	KILLED_BY_SELF	// ײ�Լ���
};

#endif
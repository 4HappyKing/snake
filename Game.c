#include "Game.h"
#include "UI.h"
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>


// ��ʼ���߽ṹ��
static void _InitializeSnake(struct Snake *pSnake);
// �ж�ʳ�������Ƿ�������ص�
static bool _IsOverlapSnake(int x, int y, const struct Snake *pSnake);
// ����һ��ʳ��
static void _GenerateFood(struct Game *pGame);
// ��ȡ�߼������������
static struct Position _GetNextPosition(const struct Snake *pSnake);
// �ж����Ƿ񽫳Ե�ʳ��
static bool _IsWillEatFood(struct Position nextPosition, struct Position foodPosition);
// �����߲��ҽ�����ʾ
static void _GrowSnakeAndDisplay(struct Snake *pSnake, struct Position foodPosition, const struct UI *pUI);
// ������ͷ
static void _AddHead(struct Snake *pSnake, struct Position nextPosition, const struct UI *pUI);
// ɾ����β
static void _RemoveTail(struct Snake *pSnake, const struct UI *pUI);
// �ƶ��߲��ҽ�����ʾ
static void _MoveSnakeAndDisplay(struct Snake *pSnake, struct Position nextPosition, const struct UI *pUI);
// ���Ƿ�ײǽ��
static bool _IsKilledByWall(const struct Node *pHead, int width, int height);
// ���Ƿ�ײ�Լ���
static bool _IsKilledBySelf(const struct Node *pHead, const struct Snake *pSnake);
// ���Ƿ���
static bool _IsSnakeAlive(const struct Game *pGame, enum ExitStatus *exitStatus);
// ������ָ��
static void _HandleDirective(struct Game *pGame);
// ��ʾ��������
static void _DisplaySnake(const struct UI *pUI, const struct Snake *pSnake);
// ��ͣ
static void _Pause();


struct Game * CreateGame()
{
	struct Game *pGame = (struct Game *)malloc(sizeof(struct Game));
	// TODO: �쳣����
	pGame->width = 28;
	pGame->height = 27;
	pGame->score = 0;
	pGame->scorePerFood = 10;
	
	_InitializeSnake(&pGame->snake);
	_GenerateFood(pGame);

	return pGame;
}

void StartGame(struct Game *pGame)
{
	struct UI *pUI = UIInitialize(pGame->width, pGame->height);
	enum ExitStatus exitStatus = QUIT;

	UIDisplayWizard(pUI);
	UIDisplayGameWindow(pUI, pGame->score, pGame->scorePerFood);
	_DisplaySnake(pUI, &pGame->snake);
	UIDisplayFoodAtXY(pUI, pGame->foodPosition.x, pGame->foodPosition.y);

	while (1) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		} else if (GetAsyncKeyState(VK_SPACE)) {
			_Pause();
		}
		
		_HandleDirective(pGame);

		struct Position nextPosition = _GetNextPosition(&pGame->snake);

		if (_IsWillEatFood(nextPosition, pGame->foodPosition)) {
			pGame->score += pGame->scorePerFood;
			UIDisplayScore(pUI, pGame->score, pGame->scorePerFood);
			_GrowSnakeAndDisplay(&pGame->snake, pGame->foodPosition, pUI);
			_GenerateFood(pGame);
			UIDisplayFoodAtXY(pUI, pGame->foodPosition.x, pGame->foodPosition.y);
		}
		else {
			_MoveSnakeAndDisplay(&pGame->snake, nextPosition, pUI);
		}

		if (!_IsSnakeAlive(pGame, &exitStatus)) {
			break;
		}

		Sleep(300);
	}

	char *messages[3];
	messages[QUIT] = "��Ϸ����";
	messages[KILLED_BY_WALL] = "��Ϸ������ײ��ǽ��";
	messages[KILLED_BY_SELF] = "��Ϸ������ײ���Լ���";
	UIShowMessage(pUI, messages[exitStatus]);

	UIDeinitialize(pUI);
}

void DestroyGame(struct Game *pGame)
{
	struct Node *pNode, *pNext;

	for (pNode = pGame->snake.pBody; pNode != NULL; pNode = pNext) {
		pNext = pNode->pNext;
		free(pNode);
	}
	free(pGame);
}

static void _InitializeSnake(struct Snake *pSnake)
{
	const int length = 3;
	const int x = 5;
	const int y = 5;
	int i;
	struct Node *pNode;

	pSnake->direction = RIGHT;
	pSnake->pBody = NULL;

	for (i = 0; i < length; i++) {
		pNode = (struct Node *)malloc(sizeof(struct Node));
		pNode->position.x = x + i;
		pNode->position.y = y;
		pNode->pNext = pSnake->pBody;
		pSnake->pBody = pNode;
	}
}

static bool _IsOverlapSnake(int x, int y, const struct Snake *pSnake)
{
	struct Node *pNode;

	for (pNode = pSnake->pBody; pNode != NULL; pNode = pNode->pNext) {
		if (pNode->position.x == x && pNode->position.y == y) {
			return true;
		}
	}

	return false;
}

static void _GenerateFood(struct Game *pGame)
{
	int x, y;

	do {
		x = rand() % pGame->width;
		y = rand() % pGame->height;
	} while (_IsOverlapSnake(x, y, &pGame->snake));

	pGame->foodPosition.x = x;
	pGame->foodPosition.y = y;
}

static struct Position _GetNextPosition(const struct Snake *pSnake)
{
	int nextX, nextY;

	nextX = pSnake->pBody->position.x;
	nextY = pSnake->pBody->position.y;

	switch (pSnake->direction) {
	case UP:
		nextY -= 1;
		break;
	case DOWN:
		nextY += 1;
		break;
	case LEFT:
		nextX -= 1;
		break;
	case RIGHT:
		nextX += 1;
		break;
	}

	struct Position position = {
		.x = nextX,
		.y = nextY
	};

	return position;
}

static bool _IsWillEatFood(struct Position nextPosition, struct Position foodPosition)
{
	return nextPosition.x == foodPosition.x && nextPosition.y == foodPosition.y;
}

static void _GrowSnakeAndDisplay(struct Snake *pSnake, struct Position foodPosition, const struct UI *pUI)
{
	struct Node *pHead = (struct Node *)malloc(sizeof(struct Node));
	pHead->position.x = foodPosition.x;
	pHead->position.y = foodPosition.y;
	pHead->pNext = pSnake->pBody;
	pSnake->pBody = pHead;

	UIDisplaySnakeBlockAtXY(pUI, foodPosition.x, foodPosition.y);
}

static void _AddHead(struct Snake *pSnake, struct Position nextPosition, const struct UI *pUI)
{
	struct Node *pNode = (struct Node *)malloc(sizeof(struct Node));

	pNode->position.x = nextPosition.x;
	pNode->position.y = nextPosition.y;
	pNode->pNext = pSnake->pBody;

	pSnake->pBody = pNode;

	UIDisplaySnakeBlockAtXY(pUI, nextPosition.x, nextPosition.y);
}

static void _RemoveTail(struct Snake *pSnake, const struct UI *pUI)
{
	struct Node *pNode = pSnake->pBody;

	// ����ʲôǰ�᣿
	while (pNode->pNext->pNext != NULL) {
		pNode = pNode->pNext;
	}

	UICleanBlockAtXY(pUI, pNode->pNext->position.x, pNode->pNext->position.y);

	free(pNode->pNext);
	pNode->pNext = NULL;
}

static void _MoveSnakeAndDisplay(struct Snake *pSnake, struct Position nextPosition, const struct UI *pUI)
{
	_RemoveTail(pSnake, pUI);
	_AddHead(pSnake, nextPosition, pUI);
}

static bool _IsKilledByWall(const struct Node *pHead, int width, int height)
{
	if (pHead->position.x < 0 || pHead->position.x >= width) {
		return true;
	}

	if (pHead->position.y < 0 || pHead->position.y >= height) {
		return true;
	}

	return false;
}

static bool _IsKilledBySelf(const struct Node *pHead, const struct Snake *pSnake)
{
	struct Node *pNode;

	// ����ʲôǰ��ô��
	for (pNode = pSnake->pBody->pNext; pNode != NULL; pNode = pNode->pNext) {
		if (pHead->position.x == pNode->position.x && pHead->position.y == pNode->position.y) {
			return true;
		}
	}

	return false;
}

static bool _IsSnakeAlive(const struct Game *pGame, enum ExitStatus *exitStatus)
{
	struct Node *pHead = pGame->snake.pBody;

	if (_IsKilledByWall(pHead, pGame->width, pGame->height)) {
		*exitStatus = KILLED_BY_WALL;
		return false;
	}

	if (_IsKilledBySelf(pHead, &pGame->snake)) {
		*exitStatus = KILLED_BY_SELF;
		return false;
	}

	return true;
}

static void _HandleDirective(struct Game *pGame)
{
	if (GetAsyncKeyState(VK_UP) && pGame->snake.direction != DOWN) {
		pGame->snake.direction = UP;
	} else if (GetAsyncKeyState(VK_DOWN) && pGame->snake.direction != UP) {
		pGame->snake.direction = DOWN;
	}
	else if (GetAsyncKeyState(VK_LEFT) && pGame->snake.direction != RIGHT) {
		pGame->snake.direction = LEFT;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && pGame->snake.direction != LEFT) {
		pGame->snake.direction = RIGHT;
	}
}

static void _DisplaySnake(const struct UI *pUI, const struct Snake *pSnake)
{
	struct Node *pNode;

	if (pSnake->pBody == NULL) {
		return;
	}

	for (pNode = pSnake->pBody; pNode != NULL; pNode = pNode->pNext) {
		UIDisplaySnakeBlockAtXY(pUI, pNode->position.x, pNode->position.y);
	}
}

static void _Pause()
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}
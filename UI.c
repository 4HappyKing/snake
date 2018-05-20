#define _CRT_SECURE_NO_WARNINGS

#include "UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static void _SetPos(int x, int y);                                 // Move the cursor to X,Y
static void _DisplayWall(const struct UI *pUI);                    // Display wall
static void _DisplayDesc(const struct UI *pUI);                    // Display right-hand information
static void _CoordinatePosAtXY(const struct UI *pUI, int x, int y);//Convert the XY coordinates of the game disk to the XY relative to the entire screen
static void _ResetCursorPosition(const struct UI *pUI);            // Reset the cursor to the bottom of the screen



struct UI * UIInitialize(int width, int height)
{
	const int left = 2;
	const int top = 2;
	const int blockWidth = 2;	
	const int descWidth = 50;

	struct UI *pUI = (struct UI *)malloc(sizeof(struct UI));
	pUI->gameWidth = width;
	pUI->gameHeight = height;
	pUI->marginLeft = left;
	pUI->marginTop = top;
	pUI->windowWidth = left + (width + 2) * blockWidth + descWidth;
	pUI->windowHeight = top + height + 2 + 3;
	pUI->foodBlock = "¨€";
	pUI->snakeBlock = "¨€";
	pUI->wallBlock = "¨€";
	pUI->blockWidth = strlen(pUI->wallBlock);

	char modeCommand[1024];
	sprintf(modeCommand, "mode con cols=%d lines=%d", pUI->windowWidth, pUI->windowHeight);
	system(modeCommand);

	return pUI;
} 

void UIDisplayWizard(const struct UI *pUI)
{
	int i;
	const char *messages[3] = {
		"Welcome to greedy snake games",
		"The snake movement is controlled separately with ¡ü.¡ý.¡û.¡ú.F1 for acceleration and f2 for deceleration.",
		"Acceleration will result in higher scores"
	};

	i = 0;
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2);
	printf("%s\n", messages[i]);
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2 + 2);
	system("pause");
	system("cls");

	i = 1;
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2);
	printf("%s\n", messages[i]);

	i = 2;
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2 + 2);
	printf("%s\n", messages[i]);
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2 + 4);
	system("pause");
	system("cls");
}

void UIDisplayGameWindow(const struct UI *pUI, int score, int scorePerFood)
{
	_DisplayWall(pUI);
	UIDisplayScore(pUI, score, scorePerFood);
	_DisplayDesc(pUI);

	_ResetCursorPosition(pUI);
}

void UIDisplayFoodAtXY(const struct UI *pUI, int x, int y)
{
	_CoordinatePosAtXY(pUI, x, y);
	printf(pUI->foodBlock);

	_ResetCursorPosition(pUI);
}

void UIDisplaySnakeBlockAtXY(const struct UI *pUI, int x, int y)
{
	_CoordinatePosAtXY(pUI, x, y);
	printf(pUI->snakeBlock);

	_ResetCursorPosition(pUI);
}

void UICleanBlockAtXY(const struct UI *pUI, int x, int y)
{
	_CoordinatePosAtXY(pUI, x, y);
	int i;

	for (i = 0; i < pUI->blockWidth; i++) {
		printf(" ");
	}

	_ResetCursorPosition(pUI);
}

void UIDisplayScore(const struct UI *pUI, int score, int scorePerFood)
{
	int blockWidth = strlen(pUI->wallBlock);
	int left = pUI->marginLeft + (pUI->gameWidth + 2) * blockWidth + 2;
	_SetPos(left, pUI->marginTop + 8);
	printf("Score: %3d£¬Score per food: %3d", score, scorePerFood);

	_ResetCursorPosition(pUI);
}

void UIShowMessage(const struct UI *pUI, const char *message)
{

	_SetPos(
		pUI->marginLeft + (
		(1 + pUI->gameWidth / 2) * pUI->blockWidth)
		- strlen(message) / 2,
		pUI->marginTop + 1 + pUI->gameHeight / 2);
	printf("%s\n", message);

	_ResetCursorPosition(pUI);
}

void UIDeinitialize(struct UI *pUI)
{
	free(pUI);
}

static void _DisplayWall(const struct UI *pUI)
{
	int left = pUI->marginLeft;
	int top = pUI->marginTop;
	int width = pUI->gameWidth;
	int height = pUI->gameHeight;
	int blockWidth = pUI->blockWidth;
	int i;

	// Up
	_SetPos(left, top);
	for (i = 0; i < width + 2; i++) {
		printf("%s", pUI->wallBlock);
	}

	// Down
	_SetPos(left, top + 1 + height);
	for (i = 0; i < width + 2; i++) {
		printf("%s", pUI->wallBlock);
	}

	// Left
	for (i = 0; i < height + 2; i++) {
		_SetPos(left, top + i);
		printf("%s", pUI->wallBlock);
	}

	// Right
	for (i = 0; i < height + 2; i++) {
		_SetPos(left + (1 + width) * blockWidth, top + i);
		printf("%s", pUI->wallBlock);
	}
}

static void _DisplayDesc(const struct UI *pUI)
{
	int left = pUI->marginLeft + (pUI->gameWidth + 2) * pUI->blockWidth + 2;
	const char *messages[] = {
		"Can't go through the wall, can't bite yourself.",
		"Control the ¡ü ¡ý ¡û ¡ú of the snake separately.¡£",
		"F 1 is acceleration and f 2 is deceleration.",
		"Esc exits the game. Space pauses the game¡£",
	};
	int i;

	for (i = 0; i < sizeof(messages) / sizeof(char *); i++) {
		_SetPos(left, pUI->marginTop + 10 + i);
		printf("%s\n", messages[i]);
	}
}

static void _SetPos(int x, int y)
{
	COORD position = { x, y };
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hOutput, position);
}

static void _CoordinatePosAtXY(const struct UI *pUI, int x, int y)
{
	_SetPos(pUI->marginLeft + (1 + x) * pUI->blockWidth,
		pUI->marginTop + 1 + y);
}

static void _ResetCursorPosition(const struct UI *pUI)
{
	_SetPos(0, pUI->windowHeight - 1);
}
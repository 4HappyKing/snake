#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "UI.h"

int main(int argc, const char *argv[])
{
	srand((unsigned int)time(NULL));

	struct Game *pGame = CreateGame();

	StartGame(pGame);
	DestroyGame(pGame);

	return 0;
}
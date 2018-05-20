#ifndef __GAME_H__
#define __GAME_H__

#include "Snake.h"

struct Game * CreateGame();             // Create and initialize the game
void StartGame(struct Game *pGame);     // Start the game
void DestroyGame(struct Game *pGame);   // Destruction of game resources

#endif
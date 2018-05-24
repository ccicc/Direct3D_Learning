#ifndef GAME_H_
#define GAME_H_

#include <Windows.h>

CONST INT SPRITES_NUMBER = 4;

BOOL Game_Init(HWND hWnd);

VOID Game_Paint(HWND hWnd);

BOOL Game_CleanUp(HWND hWnd);

#endif
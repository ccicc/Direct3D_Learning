#ifndef GAME_H_
#define GAME_H_

#include <Windows.h>

struct sWordBullets
{
	int x, y;
	bool exists;
};

CONST INT BULLETS_NUM = 30;

extern BOOL Game_Init(HWND hWnd);
extern VOID Game_Paint(HWND hWnd);
extern BOOL Game_CleanUp(HWND hWnd);

#endif
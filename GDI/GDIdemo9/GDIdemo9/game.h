#pragma once

#include <Windows.h>

typedef struct
{
	int direction;
	int x, y;
} Sprite;

CONST INT IMAGES_COUNT = 4;
CONST INT SPRITES_COUNT = 30;

BOOL Game_Init(HWND hWnd);

VOID Game_Paint(HWND hWnd);

BOOL Game_CleanUp(HWND hWnd);

VOID Bubble_Sprites(Sprite [], int n);
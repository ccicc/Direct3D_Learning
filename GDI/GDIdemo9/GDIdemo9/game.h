#ifndef MAIN_H_

#define MAIN_H_
typedef struct
{
	int x, y;
	int direction;
} SPRITE;

extern "C" BOOL Game_Init(HWND hWnd);

extern "C" VOID Game_Paint(HWND hWnd);

extern "C" BOOL Game_CleanUp(HWND hWnd);

extern "C" VOID Bubble_Sprites(SPRITE sprites[], INT n);

#endif
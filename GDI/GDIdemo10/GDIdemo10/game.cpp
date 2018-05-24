#include <Windows.h>
#include <tchar.h> 
#include "Game.h"

extern DWORD g_tPrev;
extern DWORD g_tNow;
extern CONST INT WND_WIDTH;
extern CONST INT WND_HEIGHT;

HDC g_hdc = NULL, g_mdc = NULL, g_bufferDC = NULL;
HBITMAP g_hBg = NULL, g_hSprites[SPRITES_NUMBER] = { 0 };
INT g_nNum = 0, g_nPosX = 0, g_nPosY = 0, g_nDirection = 0;

BOOL Game_Init(HWND hWnd)
{
	HBITMAP bitmap = NULL;
	WCHAR filename[10];

	// Èý¼¶»º´æ
	g_hdc = GetDC(hWnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferDC = CreateCompatibleDC(g_mdc);

	bitmap = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);

	SelectObject(g_mdc, bitmap);

	g_hBg = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);

	for (int i = 0; i < SPRITES_NUMBER; i++)
	{
		memset(filename, 0, sizeof(filename));
		swprintf_s(filename, L"go%d.bmp", i + 1);
		g_hSprites[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 480, 216, LR_LOADFROMFILE);
	}

	g_nPosX = 150;
	g_nPosY = 300;
	g_nDirection = 3;

	Game_Paint(hWnd);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	SelectObject(g_bufferDC, g_hBg);
	BitBlt(g_mdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_bufferDC, 0, 0, SRCCOPY);

	// Í¸Ã÷ÌùÍ¼(Í¸Ã÷ÕÚÕÖ·¨)
	SelectObject(g_bufferDC, g_hSprites[g_nDirection]);
	BitBlt(g_mdc, g_nPosX, g_nPosY, 60, 108, g_bufferDC, g_nNum * 60, 108, SRCAND);
	BitBlt(g_mdc, g_nPosX, g_nPosY, 60, 108, g_bufferDC, g_nNum * 60, 0, SRCPAINT);
	
	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPrev = GetTickCount();
	g_nNum++;
	if (g_nNum >= 8)g_nNum = 0;
}

BOOL Game_CleanUp(HWND hWnd)
{
	ReleaseDC(hWnd, g_hdc);
	DeleteDC(g_mdc);
	DeleteDC(g_bufferDC);

	DeleteObject(g_hBg);
	
	for (int i = 0; i < SPRITES_NUMBER; i++)
	{
		DeleteObject(g_hSprites[i]);
	}

	return TRUE;
}
#include <Windows.h>
#include <time.h>
#include <tchar.h>
#include "game.h"

#pragma comment(lib, "msimg32.lib")

extern DWORD g_tPrev, g_tNow;
extern CONST INT WND_WIDTH;
extern CONST INT WND_HEIGHT;

HDC g_hdc = NULL, g_mdc = NULL, g_bufferDC = NULL;
HBITMAP g_hBg = NULL, g_hImages[IMAGES_COUNT];
Sprite g_sprites[SPRITES_COUNT];
INT g_nNum = 0;


BOOL Game_Init(HWND hWnd)
{
	srand((unsigned)time(NULL));
	HBITMAP bitmap = NULL;
	WCHAR filename[20] = { 0 };

	g_hdc = GetDC(hWnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferDC = CreateCompatibleDC(g_hdc);

	bitmap = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);

	SelectObject(g_mdc, bitmap);

	g_hBg = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);

	for (int i = 0; i < IMAGES_COUNT; i++)
	{
		memset(filename, 0, sizeof(filename));
		swprintf_s(filename, L"%d.bmp", 11 * (i + 1));
		g_hImages[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	}

	for (int i = 0; i < SPRITES_COUNT; i++)
	{
		g_sprites[i].direction = rand() % 4;
		g_sprites[i].x = rand() % WND_WIDTH;
		g_sprites[i].y = rand() % WND_HEIGHT;
	}

	Game_Paint(hWnd);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	if (g_nNum >= 4)
	{
		g_nNum = 0;
	}

	SelectObject(g_bufferDC, g_hBg);
	BitBlt(g_mdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_bufferDC, 0, 0, SRCCOPY);

	Bubble_Sprites(g_sprites, SPRITES_COUNT);

	for (int i = 0; i < SPRITES_COUNT; i++)
	{
		SelectObject(g_bufferDC, g_hImages[g_sprites[i].direction]);
		TransparentBlt(
			g_mdc, g_sprites[i].x, g_sprites[i].y, 96, 96,
			g_bufferDC, g_nNum * 96, 0, 96, 96, RGB(0, 0, 0)
		);
	}

	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPrev = GetTickCount();

	g_nNum++;

	for (int i = 0; i < SPRITES_COUNT; i++)
	{
		switch (rand() % 4)
		{
			case 0:	// об
				g_sprites[i].y += 5;
				if (g_sprites[i].y >= WND_HEIGHT - 100)g_sprites[i].y = WND_HEIGHT - 100;
				g_sprites[i].direction = 0;
				break;
			case 1:	// вС
				g_sprites[i].x -= 5;
				if (g_sprites[i].x <= 0)g_sprites[i].x = 0;
				g_sprites[i].direction = 1;
				break;
			case 2:	// ср
				g_sprites[i].x += 5;
				if (g_sprites[i].x >= WND_WIDTH - 100)g_sprites[i].x = WND_WIDTH - 100;
				g_sprites[i].direction = 2;
				break;
			case 3: // ио
				g_sprites[i].y -= 5;
				if (g_sprites[i].y <= 0) g_sprites[i].y = 0;
				g_sprites[i].direction = 3;
				break;
		}
	}
}

BOOL Game_CleanUp(HWND hWnd)
{
	ReleaseDC(hWnd, g_hdc);
	DeleteDC(g_mdc);
	DeleteDC(g_bufferDC);
	DeleteObject(g_hBg);
	for (int i = 0; i < IMAGES_COUNT; i++)
	{
		DeleteObject(g_hImages[i]);
	}

	return TRUE;
}

VOID Bubble_Sprites(Sprite sprites[], int n)
{
	int i, j;
	Sprite temp;
	BOOL isSorted = TRUE;

	for (i = 0; i < n - 1; i++)
	{
		isSorted = TRUE;
		for (j = 0; j < n - 1 - i; j++)
		{
			if (sprites[j].y > sprites[j + 1].y)
			{
				temp = sprites[j];
				sprites[j] = sprites[j + 1];
				sprites[j + 1] = temp;
				isSorted = FALSE;
			}
		}
		if (isSorted)break;
	}
}
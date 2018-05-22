#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include "main.h"
#include "game.h"

#pragma comment(lib, "msimg32.lib")

extern DWORD g_tPrev, g_tNow;

static HDC g_hdc = NULL, g_mdc = NULL, g_bufferDC = NULL;
static HBITMAP g_hBackground = NULL, g_hSprites[SPRITE_COUNT];
static SPRITE sprites[SPRITE_NUMBERS] = { 0 };
static INT g_nPicNumber = 0, g_nPosX = 0, g_nPosY = 0;

BOOL Game_Init(HWND hWnd)
{
	srand((unsigned)time(NULL));
	HBITMAP bmp;

	g_hdc = GetDC(hWnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferDC = CreateCompatibleDC(g_hdc);

	bmp = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);
	SelectObject(g_mdc, bmp);

	g_hBackground = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);

	WCHAR filename[20];

	for (int i = 0; i < SPRITE_COUNT; i++)
	{
		memset(filename, 0, sizeof(filename));
		swprintf_s(filename, L"%d.bmp", 11 * (i + 1));
		g_hSprites[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	}

	for (int i = 0; i < SPRITE_NUMBERS; i++)
	{
		sprites[i].direction = 3;
		sprites[i].x = rand() % WND_WIDTH;
		sprites[i].y = rand() % WND_HEIGHT;
	}

	Game_Paint(hWnd);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	if (g_nPicNumber >= 4)
	{
		g_nPicNumber = 0;
	}

	SelectObject(g_bufferDC, g_hBackground);
	BitBlt(g_mdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_bufferDC, 0, 0, SRCCOPY);

	Bubble_Sprites(sprites, SPRITE_NUMBERS);

	for (int i = 0; i < SPRITE_NUMBERS; i++)
	{
		SelectObject(g_bufferDC, g_hSprites[sprites[i].direction]);
		TransparentBlt(g_mdc, sprites[i].x, sprites[i].y, 96, 96, g_bufferDC, g_nPicNumber * 96, 0, 96, 96, RGB(0, 0, 0));
	}

	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_nPicNumber++;
	g_tPrev = GetTickCount();

	for (int i = 0; i < SPRITE_NUMBERS; i++)
	{
		switch (rand() % 4)
		{
			case 0: // об
				sprites[i].y += 2;
				if (sprites[i].y >= WND_HEIGHT)
				{
					sprites[i].y = WND_HEIGHT;
				}
				sprites[i].direction = 0;
				break;
			case 1: // вС
				sprites[i].x -= 2;
				if (sprites[i].x <= 0)
				{
					sprites[i].x = 0;
				}
				sprites[i].direction = 1;
				break;
			case 2: // ср
				sprites[i].x += 2;
				if (sprites[i].x >= WND_WIDTH)
				{
					sprites[i].x = WND_WIDTH;
				}
				sprites[i].direction = 2;
				break;
			case 3: // ио
				sprites[i].y -= 2;
				if (sprites[i].y <= 0)
				{
					sprites[i].y = 0;
				}
				sprites[i].direction = 3;
				break;
		}
	}
}

VOID Bubble_Sprites(SPRITE sprites[], INT n)
{
	int i, j;
	SPRITE temp;
	BOOL isSorted;

	for (i = 0; i < n - 1; i++)
	{
		isSorted = true;
		for (j = 0; j < n - 1 - i; j++)
		{
			if (sprites[j].y > sprites[j + 1].y)
			{
				temp = sprites[j];
				sprites[j] = sprites[j + 1];
				sprites[j + 1] = temp;
				isSorted = false;
			}
		}
		if (isSorted)break;
	}
}

BOOL Game_CleanUp(HWND hWnd)
{
	ReleaseDC(hWnd, g_hdc);
	DeleteDC(g_mdc);
	DeleteDC(g_bufferDC);

	DeleteObject(g_hBackground);
	for (int i = 0; i < SPRITE_COUNT; i++)
	{
		DeleteObject(g_hSprites[i]);
	}

	return TRUE;
}
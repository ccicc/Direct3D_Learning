#include <Windows.h>
#include "Game.h"

#pragma comment(lib, "msimg32.lib")

extern DWORD g_tPrev, g_tNow;
extern CONST INT WND_WIDTH;
extern CONST INT WND_HEIGHT;

HDC g_hdc = NULL, g_mdc = NULL, g_bufferDC = NULL;
HBITMAP g_hBackground = NULL, g_hSwordMan = NULL, g_hSwordBlade = NULL;
INT g_nPosX, g_nPosY, g_nNowX, g_nNowY;

BOOL Game_Init(HWND hWnd)
{
	HBITMAP bitmap = NULL;
	g_hdc = GetDC(hWnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferDC = CreateCompatibleDC(g_hdc);

	bitmap = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);
	SelectObject(g_mdc, bitmap);

	// 初始化光标位置
	g_nPosX = g_nNowX = 300;
	g_nPosY = g_nNowY = 150;

	// 加载位图
	g_hBackground = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);
	g_hSwordMan = (HBITMAP)LoadImage(NULL, L"swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlade = (HBITMAP)LoadImage(NULL, L"swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);

	POINT pt, lt, rb;
	RECT rect;
	
	// 初始化光标位置
	pt.x = 300;
	pt.y = 150;
	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	ShowCursor(false);

	// 限定光标位置在窗口内部
	GetClientRect(hWnd, &rect);
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;
	ClientToScreen(hWnd, &lt);
	ClientToScreen(hWnd, &rb);
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	ClipCursor(&rect);

	Game_Paint(hWnd);
	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	SelectObject(g_bufferDC, g_hBackground);
	// 滚动贴图
	BitBlt(g_mdc, 0, 0, BG_OFFSET, WND_HEIGHT, g_bufferDC, WND_WIDTH - BG_OFFSET, 0, SRCCOPY);
	BitBlt(g_mdc, BG_OFFSET, 0, WND_WIDTH - BG_OFFSET, WND_HEIGHT, g_bufferDC, BG_OFFSET, 0, SRCCOPY);

	// 人物跟随鼠标光标移动
	if (g_nNowX < g_nPosX)
	{
		g_nNowX += 10;
		if (g_nNowX > g_nPosX) g_nNowX = g_nPosX;
	}
	else
	{
		g_nNowX -= 10;
		if (g_nNowX < g_nPosX) g_nNowX = g_nPosX;
	}

	if (g_nNowY < g_nPosY)
	{
		g_nNowY += 10;
		if (g_nNowY > g_nPosY)g_nNowY = g_nNowY;
	}
	else
	{
		g_nNowY -= 10;
		if (g_nNowY < g_nPosY)g_nNowY = g_nPosY;
	}

	SelectObject(g_bufferDC, g_hSwordMan);
	TransparentBlt(g_mdc, g_nNowX, g_nNowY, 317, 283, g_bufferDC, 0, 0, 317, 283, RGB(0, 0, 0));
	
	// todo 剑气(子弹)贴图
}

// todo 资源清理
BOOL Game_CleanUp(HWND hWnd)
{

}
// GDIdemo13 
// 重力模拟

#include <Windows.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_TITLE L"GDIdemo13 - 重力模拟"
#define WND_CLASS L"GDIdemo13"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);
VOID Game_Paint(HWND hWnd);
BOOL Game_CleanUp(HWND hWnd);

HDC g_hdc, g_mdc, g_bufferDC;
HBITMAP g_hBird, g_hBg;
DWORD g_tPrev, g_tNow;
INT g_nPosX, g_nPosY, g_nSpeedX, g_nSpeedY, g_nGravity;
RECT g_rect;

INT WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hprevInstance,
	LPSTR hCmdLine,
	INT nCmdShow
)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbWndExtra = 0;
	wndClass.cbClsExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName = WND_CLASS;
	wndClass.lpszMenuName = NULL;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	HWND hWnd = CreateWindow(
		WND_CLASS, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"初始化失败!", L"警告", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"湖.wav", NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPrev > 40)
			{
				Game_Paint(hWnd);
			}
		}
	}

	UnregisterClass(WND_CLASS, wndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY:
			Game_CleanUp(hWnd);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL Game_Init(HWND hWnd)
{
	HBITMAP bmp;

	g_hdc = GetDC(hWnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferDC = CreateCompatibleDC(g_hdc);
	bmp = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);

	SelectObject(g_mdc, bmp);

	g_hBg = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);
	g_hBird = (HBITMAP)LoadImage(NULL, L"angrybird.bmp", IMAGE_BITMAP, 120, 60, LR_LOADFROMFILE);

	GetClientRect(hWnd, &g_rect);

	g_tPrev = g_tNow = 0;
	g_nPosX = 0;
	g_nPosY = 100;
	g_nGravity = 10;
	g_nSpeedX = 10;
	g_nSpeedY = 0;

	Game_Paint(hWnd);

	return true;
}

VOID Game_Paint(HWND hWnd)
{
	SelectObject(g_bufferDC, g_hBg);
	BitBlt(g_mdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_bufferDC, 0, 0, SRCCOPY);

	SelectObject(g_bufferDC, g_hBird);
	BitBlt(g_mdc, g_nPosX, g_nPosY, 60, 60, g_bufferDC, 60, 0, SRCAND);
	BitBlt(g_mdc, g_nPosX, g_nPosY, 60, 60, g_bufferDC, 0, 0, SRCPAINT);

	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_nPosX += g_nSpeedX;
	g_nSpeedY += g_nGravity;
	g_nPosY += g_nSpeedY;

	if (g_nPosY > g_rect.bottom - 60)
	{
		g_nSpeedY = -g_nSpeedY;
		g_nPosY = g_rect.bottom - 60;
	}

	g_tPrev = GetTickCount();
}

BOOL Game_CleanUp(HWND hWnd)
{
	ReleaseDC(hWnd, g_hdc);
	DeleteDC(g_mdc);
	DeleteDC(g_bufferDC);
	DeleteObject(g_hBg);
	DeleteObject(g_hBird);
	return true;
}
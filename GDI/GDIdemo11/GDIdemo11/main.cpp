// GDIdemo11
// Windows消息处理 (鼠标消息处理, 滚动贴图)

#include <Windows.h>
#include <playsoundapi.h>
#include "Game.h"

#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern CONST INT WND_WIDTH = 800;
extern CONST INT WND_HEIGHT = 600;
extern CONST WCHAR *WND_TITLE = L"GDIdemo11 windows消息处理(鼠标消息)";
extern CONST WCHAR *WND_CLASS = L"GDIdemo11";

extern sWordBullets bullets[BULLETS_NUM];
extern INT g_nPosX, g_nPosY, g_nNowX, g_nNowY;
extern INT g_nBladeNum;
DWORD g_tPrev = 0, g_tNow = 0;

INT WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
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
	wndClass.hIconSm = NULL;

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
		MessageBox(NULL, L"资源初始化失败", L"警告", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"仙剑三.wav", NULL, SND_ASYNC|SND_LOOP|SND_FILENAME);

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
			if (g_tNow - g_tPrev >= 100)
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
			switch (wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hWnd);
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_DESTROY:
			Game_CleanUp(hWnd);
			PostQuitMessage(0);
			break;
			// 鼠标消息处理
		case WM_LBUTTONDOWN:
			for (int i = 0; i < BULLETS_NUM; i++)
			{
				if (!bullets[i].exists)
				{
					bullets[i].x = g_nNowX;
					bullets[i].y = g_nNowY;
					bullets[i].exists = true;
					g_nBladeNum++;
					break;
				}
			}
			break;
		case WM_MOUSEMOVE:
			g_nPosX = LOWORD(lParam);
			if (g_nPosX > WND_WIDTH - 317)
			{
				g_nPosX = WND_WIDTH - 317;
			}
			else if (g_nPosX < 0)
			{
				g_nPosX = 0;
			}

			g_nPosY = HIWORD(lParam);
			if (g_nPosY > WND_HEIGHT - 283)
			{
				g_nPosY = WND_HEIGHT - 283;
			}
			else if (g_nPosY < 0)
			{
				g_nPosY = 0;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// GDIdemo7
// 游戏循环

#include <Windows.h>
#include <tchar.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_TITLE L"GDIdemo7 - 游戏循环"
#define WND_CLASS L"GDIdemo7"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND);
VOID Game_Paint(HWND);
BOOL Game_CleanUp(HWND);

CONST INT g_nSize = 11;
INT g_nNumber = 0;

HDC g_hdc = NULL, g_mdc = NULL;
HBITMAP g_hSprites[g_nSize] = { 0 };

DWORD g_tPrev = 0, g_tNow = 0;

INT WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hprevInstance,
	LPSTR lpCmdLine,
	INT nCmdShow
)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WND_CLASS;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	HWND hWnd = CreateWindow(
		WND_CLASS, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, TRUE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!", L"警告", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"仙剑重楼.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

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
			// 游戏循环
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
					break;
				case VK_SPACE:
					g_nNumber = 0;
					break;
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
	g_hdc = GetDC(hWnd);

	WCHAR filename[20] = { 0 };

	for (int i = 0; i < g_nSize; i++)
	{
		memset(filename, 0, sizeof(filename));	// 初始化filename
		swprintf_s(filename, L"%d.bmp", i);		// 生成位图名称
		g_hSprites[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);
	}

	g_mdc = CreateCompatibleDC(g_hdc);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	if (g_nNumber == 10) return;

	SelectObject(g_mdc, g_hSprites[g_nNumber]);

	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPrev = GetTickCount();	// 更新此次渲染时间

	g_nNumber++;
}

BOOL Game_CleanUp(HWND hWnd)
{
	for (int i = 0; i < g_nSize; i++)
	{
		DeleteObject(g_hSprites[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hWnd, g_hdc);
	return TRUE;
}
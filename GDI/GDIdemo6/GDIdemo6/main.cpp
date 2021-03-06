// GDIdemo6
// 定时器动画循环

#include <Windows.h>
#include <tchar.h>

#pragma comment(lib, "winmm.lib")

#define WND_WIDTH 356
#define WND_HEIGHT 200
#define WND_TITLE L"GDIdemo6 - 定时器动画循环"
#define WND_CLASS L"GDIdemo6"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);
VOID Game_Paint(HWND hWnd);
BOOL Game_CleanUp(HWND hWnd);

CONST INT g_nSize = 12;
INT g_nNumber = 0;

HDC g_hdc, g_mdc;
HBITMAP g_hSprite[g_nSize];

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
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WND_CLASS;
	wndClass.hIconSm = NULL;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	HWND hWnd = CreateWindow(
		WND_CLASS, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	MoveWindow(hWnd, 600, 300, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!", L"提示", MB_OK|MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"鸟之诗.wav", NULL, SND_ASYNC|SND_LOOP|SND_FILENAME);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(WND_CLASS, wndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct = { 0 };
	switch (message)
	{
		case WM_TIMER:
			Game_Paint(hWnd);
			break;
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
	// 创建设备环境句柄
	g_hdc = GetDC(hWnd);

	wchar_t filename[20];

	// 载入位图
	for (int i = 0; i < g_nSize; i++)
	{
		memset(filename, 0, sizeof(filename));
		swprintf_s(filename, L"%d.bmp", i);
		g_hSprite[i] = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);
	}

	// 创建兼容内存DC
	g_mdc = CreateCompatibleDC(g_hdc);

	g_nNumber = 0;

	// 设置定时器
	SetTimer(hWnd, 1, 80, NULL);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	if (g_nNumber == 11)g_nNumber = 0;
	SelectObject(g_mdc, g_hSprite[g_nNumber]);
	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_nNumber++;
}

BOOL Game_CleanUp(HWND hWnd)
{
	KillTimer(hWnd, 1);

	for (int i = 0; i < g_nSize; i++)
	{
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hWnd, g_hdc);

	return TRUE;
}
// GDIdemo5
// 透明贴图
// 2.透明颜色法

#include <Windows.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_TITLE L"GDIdemo5 - 透明颜色法"
#define WND_CLASS L"GDIdemo5"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);
VOID Game_Paint(HWND hWnd);
BOOL Game_CleanUp(HWND hWnd);

HDC g_hdc, g_mdc;
HBITMAP g_hBackground, g_hSprite1, g_hSprite2;

INT WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	INT nCmdShow
)
{
	WNDCLASSEX WndClass = { 0 };
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.lpfnWndProc = WndProc;
	WndClass.hInstance = hInstance;
	WndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName = WND_CLASS;
	WndClass.lpszMenuName = NULL;

	if (!RegisterClassEx(&WndClass))
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
		MessageBox(NULL, L"资源初始化失败!", L"警告", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"水乡.wav", NULL, SND_ASYNC|SND_LOOP|SND_FILENAME);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(WND_CLASS, WndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	switch (message)
	{
		case WM_PAINT:
			g_hdc = BeginPaint(hWnd, &paintStruct);
			Game_Paint(hWnd);
			EndPaint(hWnd, &paintStruct);
			ValidateRect(hWnd, NULL);
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
	g_hdc = GetDC(hWnd);

	g_hBackground = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	g_hSprite1 = (HBITMAP)LoadImage(NULL, L"character1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	g_hSprite2 = (HBITMAP)LoadImage(NULL, L"character2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	g_mdc = CreateCompatibleDC(g_hdc);

	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	SelectObject(g_mdc, g_hBackground);
	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	SelectObject(g_mdc, g_hSprite1);
	TransparentBlt(g_hdc, -50, 50, 535, 650, g_mdc, 0, 0, 535, 650, RGB(0, 0, 0));

	SelectObject(g_mdc, g_hSprite2);
	TransparentBlt(g_hdc, 350, 50, 506, 650, g_mdc, 0, 0, 506, 650, RGB(0, 0, 0));
}

BOOL Game_CleanUp(HWND hWnd)
{
	DeleteDC(g_mdc);
	DeleteObject(g_hBackground);
	DeleteObject(g_hSprite1);
	DeleteObject(g_hSprite2);

	return TRUE;
}
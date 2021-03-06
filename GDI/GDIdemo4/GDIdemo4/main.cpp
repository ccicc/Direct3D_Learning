// GDIdemo4
// 透明贴图
// 1. 透明遮罩法

#include <Windows.h>
#include <wchar.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);
VOID Game_Paint(HWND hWnd);
BOOL Game_CleanUp(HWND hWnd);

HDC g_hdc, g_mdc;
HBITMAP g_hBackground, g_hSprite1, g_hSprite2;

const int Wnd_Width = 800;
const int Wnd_Height = 600;
const wchar_t Wnd_Class[] = L"GDIdemo4";
const wchar_t Wnd_Title[] = L"GDIdemo4 - 透明遮罩法";

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
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = Wnd_Class;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	HWND hWnd = CreateWindow(
		Wnd_Class, Wnd_Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		Wnd_Width, Wnd_Height, NULL, NULL, hInstance, NULL
	);

	MoveWindow(hWnd, 300, 150, Wnd_Width, Wnd_Height, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!", L"警告", MB_OK|MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"宝瓶时代1.wav", NULL, SND_ASYNC|SND_LOOP|SND_FILENAME);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(Wnd_Class, wndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	switch (message)
	{
		case WM_PAINT:
			BeginPaint(hWnd, &paintStruct);
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
	BitBlt(g_hdc, 0, 0, Wnd_Width, Wnd_Height, g_mdc, 0, 0, SRCCOPY);

	SelectObject(g_mdc, g_hSprite1);
	BitBlt(g_hdc, 50, Wnd_Height - 579, 320, 640, g_mdc, 320, 0, SRCAND);
	BitBlt(g_hdc, 50, Wnd_Height - 579, 320, 640, g_mdc, 0, 0, SRCPAINT);

	SelectObject(g_mdc, g_hSprite2);
	BitBlt(g_hdc, 400, Wnd_Height - 584, 400, 640, g_mdc, 400, 0, SRCAND);
	BitBlt(g_hdc, 400, Wnd_Height - 584, 400, 640, g_mdc, 0, 0, SRCPAINT);
}

BOOL Game_CleanUp(HWND hWnd)
{
	DeleteDC(g_mdc);
	DeleteObject(g_hSprite1);
	DeleteObject(g_hSprite2);

	return TRUE;
}
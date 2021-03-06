// GDIdemo8
// 透明贴图动画

#include <Windows.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_CLASS L"GDIdemo8"
#define WND_TITLE L"GDIdemo8 - 透明贴图动画"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);
VOID Game_Paint(HWND hWnd);
BOOL Game_CleanUp(HWND hWnd);

HDC g_hdc = NULL, g_mdc = NULL, g_bufferDC = NULL;
DWORD g_tPrev = 0, g_tNow = 0;
HBITMAP g_hBg = NULL, g_hSprite = NULL;
// 贴图坐标位置
INT g_nPosx = 0, g_nPosy = 0;
INT g_nNumber = 0;

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
	wndClass.cbWndExtra = 0;
	wndClass.cbClsExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
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

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!", L"警告", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"景天", NULL, SND_ASYNC|SND_LOOP|SND_FILENAME);

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
			if (g_tNow - g_tPrev > 100)
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
			if (wParam = VK_ESCAPE)
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
	// 三缓冲
	g_hdc = GetDC(hWnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferDC = CreateCompatibleDC(g_hdc);

	// 创建兼容性的位图
	HBITMAP bmp = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);

	SelectObject(g_mdc, bmp);

	g_hBg = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);
	g_hSprite = (HBITMAP)LoadImage(NULL, L"goright.bmp", IMAGE_BITMAP, 480, 108, LR_LOADFROMFILE);

	g_nPosx = 0;
	g_nPosy = 350;

	Game_Paint(hWnd);

	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	if (g_nNumber >= 8) g_nNumber = 0;

	// 透明化处理(透明色彩法)
	SelectObject(g_bufferDC, g_hBg);
	BitBlt(g_mdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_bufferDC, 0, 0, SRCCOPY);

	SelectObject(g_bufferDC, g_hSprite);
	TransparentBlt(g_mdc, g_nPosx, g_nPosy, 60, 108, g_bufferDC, 60*g_nNumber, 0, 60, 108, RGB(255, 0, 0));
	
	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPrev = GetTickCount();
	g_nPosx += 10;
	g_nNumber++;
	if (g_nPosx > WND_WIDTH)
	{
		g_nPosx = -60;
	}
}

BOOL Game_CleanUp(HWND hWnd)
{
	ReleaseDC(hWnd, g_hdc);
	DeleteDC(g_mdc);
	DeleteDC(g_bufferDC);
	DeleteObject(g_hBg);
	DeleteObject(g_hSprite);
	return TRUE;
}
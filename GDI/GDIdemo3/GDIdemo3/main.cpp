// GDIdemo3
// 基础位图绘制

#include <Windows.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_TITLE L"GDIdemo3 基础位图绘制"
#define WND_CLASS L"GDIdemo3"

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL GameInit(HWND hWnd);
VOID GamePaint(HWND hWnd);
BOOL GameCleanUp(HWND hWnd);

HDC g_hdc = NULL, g_mdc = NULL;
HBITMAP g_hBitmap = NULL;

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
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
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

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!GameInit(hWnd))
	{
		MessageBox(NULL, L"资源初始化错误", L"警告", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"紧迫.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

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
	PAINTSTRUCT paintStruct;
	switch (message)
	{
		case WM_PAINT:
			g_hdc = BeginPaint(hWnd, &paintStruct);
			GamePaint(hWnd);
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
			GameCleanUp(hWnd);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

BOOL GameInit(HWND hWnd)
{
	g_hdc = GetDC(hWnd);

	// 1. 载入位图
	g_hBitmap = (HBITMAP)LoadImage(NULL, L"Naruto.bmp", IMAGE_BITMAP, WND_WIDTH, WND_HEIGHT, LR_LOADFROMFILE);

	// 2. 创建兼容DC
	g_mdc = CreateCompatibleDC(g_hdc);

	GamePaint(hWnd);

	ReleaseDC(hWnd, g_hdc);

	return TRUE;
}

VOID GamePaint(HWND hWnd)
{
	// 3. 选择位图
	SelectObject(g_mdc, g_hBitmap);

	// 4. 进行贴图
	BitBlt(g_hdc, 0, 0, WND_WIDTH, WND_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

BOOL GameCleanUp(HWND hWnd)
{
	DeleteObject(g_hBitmap);
	DeleteDC(g_mdc);
	return TRUE;
}
// GDI通用框架

#include <Windows.h>

#define WND_WIDTH 1024
#define WND_HEIGHT 768
#define WND_CLASSNAME L"GDI_example"
#define WND_TITLE L"GDI_game"

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);		// 资源初始化函数
VOID Game_Paint(HWND hWnd);		// 图形绘制
BOOL Game_CleanUp(HWND hWnd);	// 资源清理函数

HDC g_hdc = NULL;	// 全局设备环境句柄

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
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"example.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WND_CLASSNAME;
	wndClass.hIconSm = NULL;

	if (!RegisterClassEx(&wndClass))return -1;

	HWND hWnd = CreateWindow(
		WND_CLASSNAME, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, TRUE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!!!", L"警告", MB_OK|MB_ICONERROR);
		return FALSE;
	}

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(WND_CLASSNAME, wndClass.hInstance);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	switch (message)
	{
		case WM_PAINT:
			g_hdc = BeginPaint(hWnd, &paintStruct);	// 绘图工作的准备
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
	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);
	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	// 图形绘制
}

BOOL Game_CleanUp(HWND hWnd)
{
	return TRUE;
}

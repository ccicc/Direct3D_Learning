// GDIdemo1 
// GDI基本绘图和随机数系统

#include <Windows.h>
#include <time.h>

#define WND_WIDTH 1200
#define WND_HEIGHT 800
#define WND_CLASSNAME L"GDIdemo1"
#define WND_TITLE L"GDIdemo1"

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hWnd);		// 资源初始化
VOID Game_Paint(HWND hWnd);		// 图形绘制
BOOL Game_CleanUp(HWND hWnd);	// 资源清理

HDC g_hdc = NULL;				// 全局设备环境句柄
HPEN g_ihPen[7] = { 0 };		// 画笔句柄数组
HBRUSH g_ihBrush[7] = { 0 };	// 画刷句柄数组

// 画笔样式
int g_ihPenStyle[7] = { PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_INSIDEFRAME, PS_NULL };
// 画刷样式
int g_ihBrushStyle[6] = { HS_HORIZONTAL, HS_VERTICAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_BDIAGONAL };

// 入口函数
INT WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	INT nShowCmd
)
{
	WNDCLASSEX WndClass = {
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		(HICON)::LoadImage(NULL, L"example.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(LTGRAY_BRUSH),
		NULL,
		WND_CLASSNAME,
		NULL
	};

	if (!RegisterClassEx(&WndClass))return -1;

	HWND hWnd = CreateWindow(
		WND_CLASSNAME, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!", L"警告", MB_OK | MB_ICONERROR);
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

	UnregisterClass(WND_CLASSNAME, WndClass.hInstance);

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
	srand((unsigned)time(NULL));
	for (int i = 0; i <= 6; ++i)
	{
		g_ihPen[i] = CreatePen(g_ihPenStyle[i], 1, RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i == 6)
		{
			g_ihBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		else
		{
			g_ihBrush[i] = CreateHatchBrush(g_ihBrushStyle[i], RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	}
	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);
	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	int y = 0;
	for (int i = 0; i <= 6; i++)
	{
		y = (i + 1) * 100;
		SelectObject(g_hdc, g_ihPen[i]);
		MoveToEx(g_hdc, 30, y, NULL);
		LineTo(g_hdc, 130, y);
	}

	int x1 = 150;
	int x2 = 250;

	for (int i = 0; i <= 6; i++)
	{
		SelectObject(g_hdc, g_ihBrush[i]);
		Rectangle(g_hdc, x1, 100, x2, y);
		x1 += 130;
		x2 += 130;
	}
}

BOOL Game_CleanUp(HWND hWnd)
{
	for (int i = 0; i <= 6; i++)
	{
		DeleteObject(g_ihPen[i]);
		DeleteObject(g_ihBrush[i]);
	}

	return TRUE;
}
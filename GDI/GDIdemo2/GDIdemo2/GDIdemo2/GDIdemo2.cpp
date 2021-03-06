// GDIdemo2
// 基本文字输出

#include <Windows.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_TITLE L"GDIdemo2"
#define WND_CLASS L"GDIdemo2"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wPARAM, LPARAM lParam);

BOOL Game_Init(HWND hWnd);
VOID Game_Paint(HWND hWnd);
BOOL Game_CleanUp(HWND hWnd);

HDC g_hdc = NULL;

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
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"example.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
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

	MoveWindow(hWnd, 300, 150, WND_WIDTH, WND_HEIGHT, TRUE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!Game_Init(hWnd))
	{
		MessageBox(NULL, L"资源初始化失败!", L"提示", MB_OK | MB_ICONWARNING);
		return -1;
	}

	PlaySound(L"鸟之诗.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

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
			Game_Paint(hWnd);
			ValidateRect(hWnd, NULL);
			EndPaint(hWnd, &paintStruct);
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
	SetBkMode(g_hdc, TRANSPARENT);
	ReleaseDC(hWnd, g_hdc);
	return TRUE;
}

VOID Game_Paint(HWND hWnd)
{
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, L"微软雅黑");
	SelectObject(g_hdc, hFont);

	wchar_t text1[] = L"我们所有梦想都能成真，只要我们有勇气去追求它们!";
	wchar_t text2[] = L"All our dreams can come true, if we have the courage to pursue them.";
	wchar_t text3[] = L"------- 沃尔特 迪士尼";

	SetTextColor(g_hdc, RGB(50,255,50));
	TextOut(g_hdc, 30, 150, text1, wcslen(text1));

	SetTextColor(g_hdc, RGB(50,50,255));
	TextOut(g_hdc, 30, 200, text2, wcslen(text2));

	SetTextColor(g_hdc, RGB(255, 150, 50));
	TextOut(g_hdc, 500, 250, text3, wcslen(text3));

	DeleteObject(hFont);
}

BOOL Game_CleanUp(HWND hWnd)
{
	return TRUE;
}
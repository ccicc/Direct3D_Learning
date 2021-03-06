// GDIdemo10
// Windows消息处理 - 键盘消息

#include <Windows.h>
#include <playsoundapi.h>

#pragma comment(lib, "winmm.lib")

extern CONST INT WND_WIDTH = 800;
extern CONST INT WND_HEIGHT = 600;
extern CONST WCHAR* WND_TITLE = L"GDIdemo10 - Windows消息处理 - 键盘消息";
extern CONST WCHAR* WND_CLASS = L"GDIdemo10";

extern INT g_nPosX;
extern INT g_nPosY;
extern INT g_nDirection;

DWORD g_tPrev = 0, g_tNow = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern BOOL Game_Init(HWND hWnd);
extern VOID Game_Paint(HWND hWnd);
extern BOOL Game_CleanUp(HWND hWnd);

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

	PlaySound(L"仙剑_温慧.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

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
			switch (wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hWnd);
					break;
				case VK_UP:
					g_nPosY -= 5;
					if (g_nPosY <= 0)g_nPosY = 0;
					g_nDirection = 0;
					break;
				case VK_DOWN:
					g_nPosY += 5;
					if (g_nPosY >= WND_HEIGHT - 135)g_nPosY = WND_HEIGHT - 135;
					g_nDirection = 1;
					break;
				case VK_LEFT:
					g_nPosX -= 5;
					if (g_nPosX <= 0)g_nPosX = 0;
					g_nDirection = 2;
					break;
				case VK_RIGHT:
					g_nPosX += 5;
					if (g_nPosX >= WND_WIDTH - 75)g_nPosX = WND_WIDTH - 75;
					g_nDirection = 3;
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
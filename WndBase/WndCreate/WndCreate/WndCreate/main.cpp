//-------------------基础窗口框架--------------------

#include <Windows.h>

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_CLASSNAME L"example"
#define WND_TITLE L"基础窗口"

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// 入口函数
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{
	//----------------------1. 窗口类的设计---------------------------
	WNDCLASSEX wndClass = { 0 };

	// 字节数大小(UINT)
	wndClass.cbSize = sizeof(WNDCLASSEX);
	// 窗口类风格样式(UINT)
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	// 窗口过程函数(WNDPROC)
	wndClass.lpfnWndProc = WndProc;
	// 窗口类附加内存(int)
	wndClass.cbClsExtra = 0;
	// 窗口附加内存(int)
	wndClass.cbClsExtra = 0;
	// 程序实例句柄(HINSTANCE)
	wndClass.hInstance = hInstance;
	// 图标句柄(HICO)
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"example.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	// 光标句柄(HCURSOR)
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// 画刷句柄(HBRUSH)
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// 资源菜单名
	wndClass.lpszMenuName = NULL;
	// 窗口类名
	wndClass.lpszClassName = WND_CLASSNAME;
	// 小图标句柄
	wndClass.hIconSm = NULL;

	//----------------------2. 窗口类的注册---------------------------
	if (!RegisterClassEx(&wndClass)) return -1;

	//----------------------3. 窗口的创建-----------------------------
	HWND hWnd = CreateWindow(
		WND_CLASSNAME, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	//----------------------4. 窗口的移动，显示和更新------------------
	MoveWindow(hWnd, 300, 300, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	//----------------------5. 消息循环--------------------------------
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	// 将虚拟键消息转换为字符消息
			DispatchMessage(&msg);	// 分发消息给窗口程序
		}
	}

	//----------------------6. 窗口类的注销---------------------------
	UnregisterClass(WND_CLASSNAME, wndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:	// 重绘消息
			ValidateRect(hWnd, NULL); // 更新客户区显示
			break;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				DestroyWindow(hWnd); // 销毁窗口，分发WM_DESTROY消息
			}
			if (wParam == VK_SPACE)
			{
				MessageBox(NULL, L"按下了空格按钮", L"提示", MB_OK | MB_ICONINFORMATION);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);	//响应WM_DESTROY,表明线程终止请求
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
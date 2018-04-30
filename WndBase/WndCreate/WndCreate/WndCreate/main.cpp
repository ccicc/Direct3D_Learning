//-------------------�������ڿ��--------------------

#include <Windows.h>

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_CLASSNAME L"example"
#define WND_TITLE L"��������"

// ���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// ��ں���
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd
)
{
	//----------------------1. ����������---------------------------
	WNDCLASSEX wndClass = { 0 };

	// �ֽ�����С(UINT)
	wndClass.cbSize = sizeof(WNDCLASSEX);
	// ����������ʽ(UINT)
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	// ���ڹ��̺���(WNDPROC)
	wndClass.lpfnWndProc = WndProc;
	// �����฽���ڴ�(int)
	wndClass.cbClsExtra = 0;
	// ���ڸ����ڴ�(int)
	wndClass.cbClsExtra = 0;
	// ����ʵ�����(HINSTANCE)
	wndClass.hInstance = hInstance;
	// ͼ����(HICO)
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"example.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	// �����(HCURSOR)
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// ��ˢ���(HBRUSH)
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// ��Դ�˵���
	wndClass.lpszMenuName = NULL;
	// ��������
	wndClass.lpszClassName = WND_CLASSNAME;
	// Сͼ����
	wndClass.hIconSm = NULL;

	//----------------------2. �������ע��---------------------------
	if (!RegisterClassEx(&wndClass)) return -1;

	//----------------------3. ���ڵĴ���-----------------------------
	HWND hWnd = CreateWindow(
		WND_CLASSNAME, WND_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL
	);

	//----------------------4. ���ڵ��ƶ�����ʾ�͸���------------------
	MoveWindow(hWnd, 300, 300, WND_WIDTH, WND_HEIGHT, true);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	//----------------------5. ��Ϣѭ��--------------------------------
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	// ���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);	// �ַ���Ϣ�����ڳ���
		}
	}

	//----------------------6. �������ע��---------------------------
	UnregisterClass(WND_CLASSNAME, wndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:	// �ػ���Ϣ
			ValidateRect(hWnd, NULL); // ���¿ͻ�����ʾ
			break;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				DestroyWindow(hWnd); // ���ٴ��ڣ��ַ�WM_DESTROY��Ϣ
			}
			if (wParam == VK_SPACE)
			{
				MessageBox(NULL, L"�����˿ո�ť", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);	//��ӦWM_DESTROY,�����߳���ֹ����
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
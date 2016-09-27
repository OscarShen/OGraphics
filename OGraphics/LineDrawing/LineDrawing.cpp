#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Windowsx.h>
#include <stdlib.h>

#define WINDOW_CLASS_NAME "WINCLASS1"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEYUP(vk_code)

HWND main_window_handle = NULL;

HINSTANCE hinstance_app = NULL;
char buffer[80];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	PAINTSTRUCT ps;
	HDC hdc;
	char buffer[80];

	switch (msg)
	{
	case WM_CREATE: {
		//do initialization
		return 0;
	}break;
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;
	HDC hdc;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	hinstance_app = hinstance;
	if (!RegisterClassEx(&winclass))
		return 0;
	if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, "Line Drawing Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hinstance, NULL)))
		return 0;

	main_window_handle = hwnd;
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		hdc = GetDC(hwnd);
		HPEN hpen = CreatePen(PS_SOLID, 1, RGB(rand() % 256, rand() % 256, (rand() % 256)));
		HPEN old_pen = (HPEN)SelectObject(hdc, hpen);
		MoveToEx(hdc, rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, NULL);
		LineTo(hdc, rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
		SelectObject(hdc, old_pen);
		DeleteObject(hpen);

		ReleaseDC(hwnd, hdc);

		if (KEYDOWN(VK_ESCAPE))
			SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
	return msg.wParam;
}
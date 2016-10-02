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

	HPEN pen = NULL;//use to draw screen saver
	int color_change_count = 100;//used to strack when to change color

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
	if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, "Time Inverval Locked Screen Saver", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hinstance, NULL)))
		return 0;

	main_window_handle = hwnd;

	hdc = GetDC(hwnd);
	srand(GetTickCount());
	int x1 = rand() % WINDOW_WIDTH;
	int y1 = rand() % WINDOW_HEIGHT;
	int x2 = rand() % WINDOW_WIDTH;
	int y2 = rand() % WINDOW_HEIGHT;

	int x1v = -4 + rand() % 8;
	int y1v = -4 + rand() % 8;
	int x2v = -4 + rand() % 8;
	int y2v = -4 + rand() % 8;



	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD start_time = GetTickCount();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		//is it time to change color
		if (++color_change_count >= 100) {
			color_change_count = 0;

			if (pen == NULL)
				DeleteObject(pen);
			//create a random colored pen
			pen = CreatePen(PS_SOLID, 1, RGB(rand() % 256, rand() % 256, rand() % 256));

			SelectObject(hdc, pen);
		}
		x1 += x1v;
		y1 += y1v;

		x2 += x2v;
		y2 += y2v;

		if (x1 < 0 || x1 >= WINDOW_WIDTH) {
			x1v = -x1v;
			x1 += x1v;
		}

		if (y1 < 0 || y1 >= WINDOW_HEIGHT) {
			y1v = -y1v;
			y1 += y1v;
		}

		if (x2 < 0 || x2 >= WINDOW_WIDTH) {
			x2v = -x2v;
			x2 += x2v;
		}

		if (y2 < 0 || y2 >= WINDOW_HEIGHT) {
			y2v = -y2v;
			y2 += y2v;
		}

		//draw a line
		MoveToEx(hdc, x1, x2, NULL);
		LineTo(hdc, x2, y2);

		//use this to lock time interval in 33Hz
		while (GetTickCount() - start_time < 33)
			;
		if (KEYDOWN(VK_ESCAPE)) {
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
	}
	ReleaseDC(hwnd, hdc);
	return msg.wParam;
}
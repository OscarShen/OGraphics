#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Windowsx.h>
#include <stdlib.h>
#include "OGraphics.h"
#include <iostream>
#include "OGUtils.h"

#define WINDOW_CLASS_NAME "WINCLASS1"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEYUP(vk_code)

HWND main_window_handle = nullptr;
HINSTANCE hinstance_app = nullptr;
HDC hdc = nullptr;
char buffer[80];

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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

//program in this function
int gameMain() {
	OGPoly poly(std::vector<OGPoint*>{&OGPoint(100, 100), &OGPoint(200, 200), &OGPoint(100, 200), &OGPoint(400, 150), &OGPoint(200, 100) });
	ogFillPoly(hdc, poly, RGB(0, 255, 0));
	ogDrawCircle(hdc, OGPoint(500, 500), 50, RGB(255, 0, 0));
	ogDrawEllipse(hdc, OGPoint(800, 100), 100, 50, RGB(0, 0, 255));
	return 1;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = windowProc;
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
		400, 150, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hinstance, NULL)))
		return 0;

	main_window_handle = hwnd;

	hdc = GetDC(hwnd);


	// game main function
	gameMain();
	// end game main function
	ReleaseDC(hwnd, hdc);

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			//hdc = GetDC(hwnd);

			// game main function
			// gameMain();
			// end game main function
			// ReleaseDC(hwnd, hdc);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (KEYDOWN(VK_ESCAPE))
			SendMessage(hwnd, WM_CLOSE, 0, 0);

	}
	return msg.wParam;
}
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
	if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, "Ellipse Animation Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hinstance, NULL)))
		return 0;

	main_window_handle = hwnd;

	hdc = GetDC(hwnd);
	srand(GetTickCount());

	HPEN white_pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN black_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH green_brush = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0));

	//starting position
	int ball_x = WINDOW_WIDTH / 2;
	int ball_y = WINDOW_HEIGHT / 2;

	//velocity
	int xv = -4 + rand() % 8;
	int yv = -4 + rand() % 2;

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		SelectObject(hdc, black_pen);
		SelectObject(hdc, black_brush);

		Ellipse(hdc, ball_x, ball_y, ball_x + 32, ball_y + 32);

		ball_x += xv;
		ball_y += yv;
		
		if (ball_x<0 || ball_x>WINDOW_WIDTH - 32) {
			xv = -xv;
			ball_x += xv;
		}
		if (ball_y<0 || ball_y>WINDOW_HEIGHT - 32) {
			yv = -yv;
			ball_y += yv;
		}

		SelectObject(hdc, white_pen);
		SelectObject(hdc, green_brush);

		Ellipse(hdc, ball_x, ball_y, ball_x + 32, ball_y + 32);

		if (KEYDOWN(VK_ESCAPE))
			SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
	return msg.wParam;
}
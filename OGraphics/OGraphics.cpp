#include <Windows.h>
#include <math.h>

void ogline(HDC hdc, int x0, int y0, int x1, int y1, COLORREF color, int width) {
	int dx = abs(x1 - x0), dy = abs(y1 - y0), e = 2 * dy - dx, twodx = 2 * dx, twody = 2 * dy;
	int tx = x1 > x0 ? 1 : -1, ty = y1 > y0 ? 1 : -1;
	int x = x0, y = y0;
	if (dx > dy) {
		for (int i = 0; i <= dx; ++i) {
			SetPixel(hdc, x, y, color);
			x += tx;
			if (e > 0) {
				e += twody - twodx;
				y += ty;
			}
			else {
				e += twody;
			}
		}
	}
	else {
		for (int i = 0; i <= dy; ++i) {
			SetPixel(hdc, x, y, color);
			y += ty;
			if (e > 0) {
				e += twodx - twody;
				x += tx;
			}
			else {
				e += twodx;
			}
		}
	}
}
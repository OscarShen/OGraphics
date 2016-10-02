#pragma once
#ifndef OGraphics_h_
#define OGraphics_h_
#include "OGUtils.h"

//画线算法（Bresenham算法）
void ogDrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color = RGB(0, 0, 0));

//裁剪画线辅助函数
int ogDrawClipLine(HDC hdc, int x1, int y1, int x2, int y2, OGRect &clipRect, COLORREF color = RGB(0, 0, 0));

//裁剪画线（Cohen-Sutherland算法）
int ogClipLine(int &x1, int &y1, int &x2, int &y2, OGRect &rect);
#endif // !OGraphics_h_
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

//画椭圆算法（Bresenham算法）（x、y为中心坐标，a、b为长轴、短轴）
int ogDrawEllipse(HDC hdc, OGPoint& p, int a, int b, COLORREF color = RGB(255, 255, 255));

//画圆算法
int ogDrawCircle(HDC hdc, OGPoint& p, int r, COLORREF color = RGB(255, 255, 255));

//填充算法（扫描线算法）这儿有一个很严重的问题：没有将数据处理和展示分开
int ogFillPoly(HDC hdc, OGPoly poly, COLORREF color = RGB(255, 255, 255));
#endif // !OGraphics_h_
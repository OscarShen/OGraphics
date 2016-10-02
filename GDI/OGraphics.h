#pragma once
#ifndef OGraphics_h_
#define OGraphics_h_
#include "OGUtils.h"

//�����㷨��Bresenham�㷨��
void ogDrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color = RGB(0, 0, 0));

//�ü����߸�������
int ogDrawClipLine(HDC hdc, int x1, int y1, int x2, int y2, OGRect &clipRect, COLORREF color = RGB(0, 0, 0));

//�ü����ߣ�Cohen-Sutherland�㷨��
int ogClipLine(int &x1, int &y1, int &x2, int &y2, OGRect &rect);
#endif // !OGraphics_h_
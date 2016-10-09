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

//����Բ�㷨��Bresenham�㷨����x��yΪ�������꣬a��bΪ���ᡢ���ᣩ
int ogDrawEllipse(HDC hdc, OGPoint& p, int a, int b, COLORREF color = RGB(255, 255, 255));

//��Բ�㷨
int ogDrawCircle(HDC hdc, OGPoint& p, int r, COLORREF color = RGB(255, 255, 255));

//����㷨��ɨ�����㷨�������һ�������ص����⣺û�н����ݴ����չʾ�ֿ�
int ogFillPoly(HDC hdc, OGPoly poly, COLORREF color = RGB(255, 255, 255));
#endif // !OGraphics_h_
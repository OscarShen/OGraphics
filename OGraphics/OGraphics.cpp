#include <Windows.h>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include "OGraphics.h"

void ogline(HDC hdc, int x0, int y0, int x1, int y1, COLORREF color, int width)
{
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

double crossProduct(double x0, double y0, double x1, double y1)
{
	return x0*y1 - x1*y0;
}

double dotProduct(double x0, double y0, double x1, double y1)
{
	return x0*x1 + y0*y1;
}

bool isPointInRect(const Rect& rect, const Point& p)
{
	double xl, xr, yt, yb;

	if (rect.p1.x < rect.p2.x) {
		xl = rect.p1.x;
		xr = rect.p2.x;
	}
	else {
		xl = rect.p2.x;
		xr = rect.p1.x;
	}

	if (rect.p1.y < rect.p2.y) {
		yb = rect.p1.y;
		yt = rect.p2.y;
	}
	else {
		yb = rect.p2.y;
		yt = rect.p1.y;
	}
	return (p.x >= xl&&p.x <= xr) && (p.y >= yb&&p.y <= yt);
}

double pointDistance(const Point& p1, const Point& p2)
{
	return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y-p2.y));
}

void getLineSegmentRect(const LineSeg& ls, Rect& rect) {
	rect = Rect(ls.pbegin, ls.pend);
}


bool isPointOnLineSegment(const LineSeg& ls, const Point& p)
{
	Rect rect;
	getLineSegmentRect(ls, rect);
	double cp = crossProduct(ls.pend.x - ls.pbegin.x, ls.pend.y - ls.pbegin.y, 
		p.x - ls.pbegin.x, p.y - ls.pbegin.y);
	return isPointInRect(rect, p) && isZeroFloatValue(cp);
}

bool isZeroFloatValue(double num)
{
	return num - 0.0 < 1e-8;
}

bool isRectIntersect(const Rect & rect1, const Rect & rect2)
{
	return max(rect1.p1.x, rect1.p2.x) >= min(rect2.p1.x, rect2.p2.x)
		&& max(rect1.p1.y, rect1.p2.y) >= min(rect2.p1.y, rect2.p2.y)
		&& max(rect2.p1.x, rect2.p2.x) >= min(rect1.p1.x, rect1.p2.x)
		&& max(rect2.p1.y, rect2.p2.y) >= min(rect1.p1.y, rect1.p2.y);
}

bool isLineSegmentIntersect(const LineSeg & ls1, const LineSeg & ls2)
{
	if (!isRectOfLineIntersect(ls1, ls2))
		return false;
	double cp1 = crossProduct(ls2.pbegin.x - ls1.pbegin.x, ls2.pbegin.y - ls1.pbegin.y, 
		ls1.pend.x - ls1.pbegin.x, ls1.pend.y - ls1.pbegin.y);
	double cp2 = crossProduct(ls1.pend.x - ls1.pbegin.x, ls1.pend.y - ls1.pbegin.y, 
		ls2.pend.x - ls1.pbegin.x, ls2.pend.y - ls1.pbegin.y);
	if (cp1*cp2 < 0)
		return false;
	cp1 = crossProduct(ls1.pbegin.x - ls2.pbegin.x, ls1.pbegin.y - ls2.pbegin.y, 
		ls2.pend.x - ls2.pbegin.x, ls2.pend.y - ls2.pbegin.y);
	cp2 = crossProduct(ls2.pend.x - ls2.pbegin.x, ls2.pend.y - ls2.pbegin.y, 
		ls1.pend.x - ls2.pbegin.x, ls1.pend.y - ls2.pbegin.y);
	if (cp1*cp2 < 0)
		return false;
	return true;
}

bool isRectOfLineIntersect(const LineSeg & ls1, const LineSeg & ls2)
{
	Rect rect1, rect2;
	getLineSegmentRect(ls1, rect1);
	getLineSegmentRect(ls2, rect2);
	return isRectIntersect(rect1, rect2);
}


int main()
{
	Point p1(0, 0), p2(3, 4);
	Rect rect(p1, p2);
	if (isPointInRect(rect, Point(0, 0)))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	std::cout << pointDistance(Point(2,2),Point(2,3)) << std::endl;
	std::cout << "--------------------------" << std::endl;
	if (isZeroFloatValue(0.00000000001))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (isPointOnLineSegment(LineSeg(Point(0, 0), Point(4, 4)), Point(2, 2)))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	std::cout << "-----------------------" << std::endl;
	Rect r1(Point(0, 0), Point(3, 3)), r2(Point(4, 0), Point(5, 3));
	if (isRectIntersect(r1, r2))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	LineSeg ls1(Point(0, 0), Point(3, 3)), ls2(Point(4, 0), Point(5, 3));
	if(isRectOfLineIntersect(ls1,ls2))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;

	Rect r3(Point(0, 0), Point(3, 3)), r4(Point(1, 0), Point(5, 3));
	if (isRectIntersect(r3, r4))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	LineSeg ls3(Point(0, 0), Point(3, 3)), ls4(Point(1, 0), Point(6, 3));
	if (isRectOfLineIntersect(ls3, ls4))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	return 0;
}
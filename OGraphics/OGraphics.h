#pragma once
#ifndef OGraphics_h_
#define OGraphics_h_
#include <Windows.h>

struct Point {
	double x;
	double y;
	Point() {}
	Point(double x, double y) :x(x), y(y) {}
};

typedef struct Rectangle {
	Point p1, p2;
	Rectangle() {}
	Rectangle(Point p1, Point p2) :p1(p1), p2(p2) {}
}Rect;

struct Circle {
	Point p;
	double r;
	Circle(Point p, double r) :p(p), r(r) {}
};

struct Polygon {
	std::vector<Point> points;
	int n;
	Polygon(std::vector<Point>& points, int n) :points(points), n(n) {}
};

typedef struct LineSegment {
	Point pbegin;
	Point pend;
	LineSegment(Point pbegin, Point pend) :pbegin(pbegin), pend(pend) {}
}LineSeg;

//矢量叉乘
double crossProduct(double x0, double y0, double x1, double y1);

//矢量点乘
double dotProduct(double x0, double y0, double x1, double y1);

//画线算法
void ogline(HDC hdc, int x0, int y0, int x1, int y1, COLORREF color = RGB(0, 0, 0), int width = 1);

//判断点是否在矩阵内
bool isPointInRect(const Rect& rect, const Point& p);

//计算平面两点的距离
double pointDistance(const Point& p1, const Point& p2);

//判断点是否在线段上
bool isPointOnLineSegment(const LineSeg& ls, const Point& p);

//获得线段对应的矩阵
void getLineSegmentRect(const LineSeg& ls, Rect& rect);

//浮点精度1e-8的条件下判断为零
bool isZeroFloatValue(double num);

//判断两个矩形是否相交
bool isRectIntersect(const Rect& rect1, const Rect& rect2);

//判断线段是否相交
bool isLineSegmentIntersect(const LineSeg& ls1, const LineSeg& ls2);

//判断线段生成的矩形
bool isRectOfLineIntersect(const LineSeg& ls1, const LineSeg& ls2);
#endif // !OGraphics_h_
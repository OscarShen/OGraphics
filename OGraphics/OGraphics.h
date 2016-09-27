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

//ʸ�����
double crossProduct(double x0, double y0, double x1, double y1);

//ʸ�����
double dotProduct(double x0, double y0, double x1, double y1);

//�����㷨
void ogline(HDC hdc, int x0, int y0, int x1, int y1, COLORREF color = RGB(0, 0, 0), int width = 1);

//�жϵ��Ƿ��ھ�����
bool isPointInRect(const Rect& rect, const Point& p);

//����ƽ������ľ���
double pointDistance(const Point& p1, const Point& p2);

//�жϵ��Ƿ����߶���
bool isPointOnLineSegment(const LineSeg& ls, const Point& p);

//����߶ζ�Ӧ�ľ���
void getLineSegmentRect(const LineSeg& ls, Rect& rect);

//���㾫��1e-8���������ж�Ϊ��
bool isZeroFloatValue(double num);

//�ж����������Ƿ��ཻ
bool isRectIntersect(const Rect& rect1, const Rect& rect2);

//�ж��߶��Ƿ��ཻ
bool isLineSegmentIntersect(const LineSeg& ls1, const LineSeg& ls2);

//�ж��߶����ɵľ���
bool isRectOfLineIntersect(const LineSeg& ls1, const LineSeg& ls2);
#endif // !OGraphics_h_
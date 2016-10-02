#pragma once
#ifndef OGUtils_h_
#define OGUtils_h_
#include <vector>

//点
struct OGPoint {
	double x;
	double y;
	OGPoint() {}
	OGPoint(double x, double y) :x(x), y(y) {}
};

//矩形
typedef struct OGRectangle {
	OGPoint p1, p2;
	OGRectangle() {}
	OGRectangle(OGPoint p1, OGPoint p2) :p1(p1), p2(p2) {}
}OGRect;

//圆
struct OGCircle {
	OGPoint p;
	double r;
	OGCircle(OGPoint p, double r) :p(p), r(r) {}
};

//多边形
typedef struct OGPolygon {
	std::vector<OGPoint> points;
	int n;
	OGPolygon(std::vector<OGPoint>& points, int n) :points(points), n(n) {}
	bool isvalid();
}OGPoly;

//线段
typedef struct LineSegment {
	OGPoint pbegin;
	OGPoint pend;
	LineSegment(OGPoint pbegin, OGPoint pend) :pbegin(pbegin), pend(pend) {}
}OGLineSeg;

//矢量叉乘
double crossProduct(double x0, double y0, double x1, double y1);

//矢量点乘
double dotProduct(double x0, double y0, double x1, double y1);


//判断点是否在矩阵内
bool isPointInRect(const OGRect& rect, const OGPoint& p);

//计算平面两点的距离
double pointDistance(const OGPoint& p1, const OGPoint& p2);

//判断点是否在线段上
bool isPointOnLineSegment(const OGLineSeg& ls, const OGPoint& p);

//获得线段对应的矩阵
void getLineSegmentRect(const OGLineSeg& ls, OGRect& rect);

//浮点精度1e-8的条件下判断为零
bool isZeroFloatValue(double num);

//判断两个矩形是否相交
bool isRectIntersect(const OGRect& rect1, const OGRect& rect2);

//判断线段是否相交
bool isLineSegmentIntersect(const OGLineSeg& ls1, const OGLineSeg& ls2);

//判断线段生成的矩形
bool isRectOfLineIntersect(const OGLineSeg& ls1, const OGLineSeg& ls2);

bool isPointInPolygon(const OGPoly& OGPoly, const OGPoint& p);
#endif // !OGUtils_h_

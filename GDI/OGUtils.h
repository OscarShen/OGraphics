#pragma once
#ifndef OGUtils_h_
#define OGUtils_h_
#include <vector>

//��
struct OGPoint {
	double x;
	double y;
	OGPoint() {}
	OGPoint(double x, double y) :x(x), y(y) {}
};

//����
typedef struct OGRectangle {
	OGPoint p1, p2;
	OGRectangle() {}
	OGRectangle(OGPoint p1, OGPoint p2) :p1(p1), p2(p2) {}
}OGRect;

//Բ
struct OGCircle {
	OGPoint p;
	double r;
	OGCircle(OGPoint p, double r) :p(p), r(r) {}
};

//�����
typedef struct OGPolygon {
	std::vector<OGPoint> points;
	int n;
	OGPolygon(std::vector<OGPoint>& points, int n) :points(points), n(n) {}
	bool isvalid();
}OGPoly;

//�߶�
typedef struct LineSegment {
	OGPoint pbegin;
	OGPoint pend;
	LineSegment(OGPoint pbegin, OGPoint pend) :pbegin(pbegin), pend(pend) {}
}OGLineSeg;

//ʸ�����
double crossProduct(double x0, double y0, double x1, double y1);

//ʸ�����
double dotProduct(double x0, double y0, double x1, double y1);


//�жϵ��Ƿ��ھ�����
bool isPointInRect(const OGRect& rect, const OGPoint& p);

//����ƽ������ľ���
double pointDistance(const OGPoint& p1, const OGPoint& p2);

//�жϵ��Ƿ����߶���
bool isPointOnLineSegment(const OGLineSeg& ls, const OGPoint& p);

//����߶ζ�Ӧ�ľ���
void getLineSegmentRect(const OGLineSeg& ls, OGRect& rect);

//���㾫��1e-8���������ж�Ϊ��
bool isZeroFloatValue(double num);

//�ж����������Ƿ��ཻ
bool isRectIntersect(const OGRect& rect1, const OGRect& rect2);

//�ж��߶��Ƿ��ཻ
bool isLineSegmentIntersect(const OGLineSeg& ls1, const OGLineSeg& ls2);

//�ж��߶����ɵľ���
bool isRectOfLineIntersect(const OGLineSeg& ls1, const OGLineSeg& ls2);

bool isPointInPolygon(const OGPoly& OGPoly, const OGPoint& p);
#endif // !OGUtils_h_

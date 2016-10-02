#include "OGUtils.h"
#include <algorithm>
#include <iostream>

double crossProduct(double x0, double y0, double x1, double y1)
{
	return x0*y1 - x1*y0;
}

double dotProduct(double x0, double y0, double x1, double y1)
{
	return x0*x1 + y0*y1;
}

bool isPointInRect(const OGRect& rect, const OGPoint& p)
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

double pointDistance(const OGPoint& p1, const OGPoint& p2)
{
	return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

void getLineSegmentRect(const OGLineSeg& ls, OGRect& rect) {
	rect = OGRect(ls.pbegin, ls.pend);
}


bool isPointOnLineSegment(const OGLineSeg& ls, const OGPoint& p)
{
	OGRect rect;
	getLineSegmentRect(ls, rect);
	double cp = crossProduct(ls.pend.x - ls.pbegin.x, ls.pend.y - ls.pbegin.y,
		p.x - ls.pbegin.x, p.y - ls.pbegin.y);
	return isPointInRect(rect, p) && isZeroFloatValue(cp);
}

bool isZeroFloatValue(double num)
{
	return num - 0.0 < 1e-8;
}

bool isRectIntersect(const OGRect & rect1, const OGRect & rect2)
{
	return std::max(rect1.p1.x, rect1.p2.x) >= std::min(rect2.p1.x, rect2.p2.x)
		&& std::max(rect1.p1.y, rect1.p2.y) >= std::min(rect2.p1.y, rect2.p2.y)
		&& std::max(rect2.p1.x, rect2.p2.x) >= std::min(rect1.p1.x, rect1.p2.x)
		&& std::max(rect2.p1.y, rect2.p2.y) >= std::min(rect1.p1.y, rect1.p2.y);
}

bool isLineSegmentIntersect(const OGLineSeg & ls1, const OGLineSeg & ls2)
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

bool isRectOfLineIntersect(const OGLineSeg & ls1, const OGLineSeg & ls2)
{
	OGRect rect1, rect2;
	getLineSegmentRect(ls1, rect1);
	getLineSegmentRect(ls2, rect2);
	return isRectIntersect(rect1, rect2);
}

bool isPointInPolygon(const OGPoly & poly, const OGPoint & p)
{

	return false;
}
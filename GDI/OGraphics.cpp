#include <Windows.h>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <stack>
#include <functional>
#include "OGraphics.h"

void ogDrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	int dx = abs(x2 - x1), dy = abs(y2 - y1), e = 2 * dy - dx, twodx = 2 * dx, twody = 2 * dy;
	int tx = x2 > x1 ? 1 : -1, ty = y2 > y1 ? 1 : -1;
	int x = x1, y = y1;
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

int ogDrawClipLine(HDC hdc, int x1, int y1, int x2, int y2, OGRect &clipRect, COLORREF color)
{
	int cx0 = x1, cy0 = y1, cx1 = x2, cy1 = y2;
	if (ogClipLine(cx0, cy0, cx1, cy1, clipRect))
		ogDrawLine(hdc, cx0, cy0, cx1, cy1,color);
	return 1;
}

int ogClipLine(int &x1, int &y1, int &x2, int &y2, OGRect &rect)
{
	//分区方法来自CSDN，保证位运算后不会出现歧义
	//       nw 0x0009                 n 0x0008                       ne 0x000a
	//                         ------------------------------
	//                         |                            |
	//       w 0x0001          |        c 0x0000            |         e 0x0002
	//                         |                            |
	//                         |                            |
	//                         ------------------------------
	//       sw 0x0005                  s 0x0004                       se 0x0006
#define CLIP_C 0x0000 //center of clip district
#define CLIP_W 0x0001 //west
#define CLIP_E 0x0002 //east
#define CLIP_S 0x0004 //south
#define CLIP_N 0x0008 //north

#define CLIP_NE 0x000a //north-east
#define CLIP_SE 0x0006 //south-east
#define CLIP_NW 0x0009 //north-weast
#define CLIP_SW 0x0005 //south-weast

	int cx1 = x1,cy1 = y1,cx2 = x2,cy2 = y2;

	//用来记录直线可能穿越的两个方位代码
	int p1_code = 0,p2_code = 0;
	int min_y, min_x, max_y, max_x;
	if (rect.p1.x < rect.p2.x) {
		min_x = (int)rect.p1.x;
		max_x = (int)rect.p2.x;
	}
	if (rect.p1.y < rect.p2.y) {
		min_y = (int)rect.p1.y;
		max_y = (int)rect.p2.y;
	}

	if (y1 < min_y)
		p1_code |= CLIP_N; //0x0009 or 0x0008 or 0x000a
	else if (y1 > max_y)
		p1_code |= CLIP_S;

	if (x1 < min_x)
		p1_code |= CLIP_W;
	else if (x1 > max_x)
		p1_code |= CLIP_E;

	if (y2 < min_y)
		p2_code |= CLIP_N; //0x0009 or 0x0008 or 0x000a
	else if (y2 > max_y)
		p2_code |= CLIP_S;

	if (x2 < min_x)
		p2_code |= CLIP_W;
	else if (x2 > max_x)
		p2_code |= CLIP_E;

	//line in the same direction, must be out of rectangle
	if (p1_code&p2_code)
		return 0;
	
	//if the line is totally in the rectangle
	if (p1_code == 0 && p2_code == 0)
		return 1;

	//calculate the position of p1
	switch (p1_code)
	{
	case CLIP_C:
		break;
	case CLIP_N:
		cy1 = min_y;
		cx1 = (int)(x1 + 0.5 + (min_y - y1)*(x2 - x1) / (y2 - y1));
		break;
	case CLIP_S:
		cy1 = max_y;
		cx1 = (int)(x1 + 0.5 + (max_y - y1)*(x2 - x1) / (y2 - y1));
		break;
	case CLIP_W:
		cx1 = min_x;
		cy1 = (int)(y1 + 0.5 + (min_x - x1)*(y2 - y1) / (x2 - x1));
		break;
	case CLIP_E:
		cx1 = max_x;
		cy1 = (int)(y1 + 0.5 + (max_x - x1)*(y2 - y1) / (x2 - x1));
		break;
	case CLIP_NE:
		cy1 = min_y;
		cx1 = (int)(x1 + 0.5 + (min_y - y1)*(x2 - x1) / (y2 - y1));
		if (cx1<min_x || cx1>max_x) {
			cx1 = max_x;
			cy1 = (int)(y1 + 0.5 + (max_x - x1)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	case CLIP_SE:
		cy1 = max_y;
		cx1 = (int)(x1 + 0.5 + (max_y - y1)*(x2 - x1) / (y2 - y1));
		if (cx1<min_x || cx1>max_x) {
			cx1 = max_x;
			cy1 = (int)(y1 + 0.5 + (max_x - x1)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	case CLIP_NW:
		cy1 = min_y;
		cx1 = (int)(x1 + 0.5 + (min_y - y1)*(x2 - x1) / (y2 - y1));
		if (cx1<min_x || cx1>max_x) {
			cx1 = min_x;
			cy1 = (int)(y1 + 0.5 + (min_x - x1)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	case CLIP_SW:
		cy1 = max_y;
		cx1 = (int)(x1 + 0.5 + (max_y - y1)*(x2 - x1) / (y2 - y1));
		if (cx1<min_x || cx1>max_x) {
			cx1 = max_x;
			cy1 = (int)(y1 + 0.5 + (min_x - x1)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	default:break;
	}
	switch (p2_code)
	{
	case CLIP_C:
		break;
	case CLIP_N:
		cy2 = min_y;
		cx2 = (int)(x2 + 0.5 + (min_y - y2)*(x2 - x1) / (y2 - y1));
		break;
	case CLIP_S:
		cy2 = max_y;
		cx2 = (int)(x2 + 0.5 + (max_y - y2)*(x2 - x1) / (y2 - y1));
		break;
	case CLIP_W:
		cx2 = min_x;
		cy2 = (int)(y2 + 0.5 + (min_x - x2)*(y2 - y1) / (x2 - x1));
		break;
	case CLIP_E:
		cx2 = max_x;
		cy2 = (int)(y2 + 0.5 + (max_x - x2)*(y2 - y1) / (x2 - x1));
		break;
	case CLIP_NE:
		cy2 = min_y;
		cx2 = (int)(x2 + 0.5 + (min_y - y2)*(x2 - x1) / (y2 - y1));
		if (cx2<min_x || cx2>max_x) {
			cx2 = max_x;
			cy2 = (int)(y2 + 0.5 + (max_x - x2)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	case CLIP_SE:
		cy2 = max_y;
		cx2 = (int)(x2 + 0.5 + (max_y - y2)*(x2 - x1) / (y2 - y1));
		if (cx2<min_x || cx2>max_x) {
			cx2 = max_x;
			cy2 = (int)(y2 + 0.5 + (max_x - x2)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	case CLIP_NW:
		cy2 = min_y;
		cx2 = (int)(x2 + 0.5 + (min_y - y2)*(x2 - x1) / (y2 - y1));
		if (cx2<min_x || cx2>max_x) {
			cx2 = min_x;
			cy2 = (int)(y2 + 0.5 + (min_x - x2)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	case CLIP_SW:
		cy2 = max_y;
		cx2 = (int)(x2 + 0.5 + (max_y - y2)*(x2 - x1) / (y2 - y1));
		if (cx2<min_x || cx2>max_x) {
			cx2 = max_x;
			cy2 = (int)(y2 + 0.5 + (min_x - x2)*(y2 - y1) / (x2 - x1));
		}//also maybe not have intersection with rectangle
		break;
	default:break;
	}
	//do not have intersection
	if (cx1<min_x || cx1>max_x
		|| cy1<min_y || cy1>max_y
		|| cx2<min_x || cx2>max_x
		|| cy2<min_y || cy2>max_y)
		return 0;

	x1 = cx1;
	y1 = cy1;
	x2 = cx2;
	y2 = cy2;
	return 1;
}

int ogDrawEllipse(HDC hdc, OGPoint& p, int a, int b, COLORREF color)
{
	int aa = a*a, bb = b*b, twoaa = 2 * aa, twobb = 2 * bb;
	int x = 0, y = b;
	int d;
	int dx = 0, dy = twoaa*y;
	d = (int)(bb + aa*(-b + 0.25) + 0.5);
	SetPixel(hdc, p.x + x, p.y + y, color);
	SetPixel(hdc, p.x - x, p.y + y, color);
	SetPixel(hdc, p.x + x, p.y - y, color);
	SetPixel(hdc, p.x - x, p.y - y, color);
	while (dx < dy) {
		++x;
		dx += twobb;
		if (d < 0)
			d += bb + dx;
		else {
			dy -= twoaa;
			d += bb + dx - dy;
			--y;
		}
		SetPixel(hdc, p.x + x, p.y + y, color);
		SetPixel(hdc, p.x - x, p.y + y, color);
		SetPixel(hdc, p.x + x, p.y - y, color);
		SetPixel(hdc, p.x - x, p.y - y, color);
	}

	d = (int)(bb*(x + 0.5)*(x + 0.5) + aa*(y - 1)*(y - 1) - aa*bb + 0.5);
	while (y > 0) {
		--y;
		dy -= twoaa;
		if (d > 0)
			d += aa - dy;
		else {
			++x;
			dx += twobb;
			d += aa - dy + dx;
		}
		SetPixel(hdc, p.x + x, p.y + y, color);
		SetPixel(hdc, p.x - x, p.y + y, color);
		SetPixel(hdc, p.x + x, p.y - y, color);
		SetPixel(hdc, p.x - x, p.y - y, color);
	}
	return 1;
}

void ogCirclePoints(HDC hdc, OGPoint& p, int x, int y, COLORREF color) {
	SetPixel(hdc, x + p.x, y + p.y, color);
	SetPixel(hdc, y + p.x, x + p.y, color);
	SetPixel(hdc, -x + p.x, y + p.y, color);
	SetPixel(hdc, y + p.x, -x + p.y, color);
	SetPixel(hdc, x + p.x, -y + p.y, color);
	SetPixel(hdc, -y + p.x, x + p.y, color);
	SetPixel(hdc, -x + p.x, -y + p.y, color);
	SetPixel(hdc, -y + p.x, -x + p.y, color);
}

int ogDrawCircle(HDC hdc, OGPoint& p, int r, COLORREF color) {
	int x = 0, y = r, e = 1 - r;
	ogCirclePoints(hdc, p, x, y, color);
	while (x <= y) {
		if (e < 0)
			e += 2 * x + 3;
		else {
			e += 2 * (x - y) + 5;
			--y;
		}
		++x;
		ogCirclePoints(hdc, p, x, y, color);
	}
	return 1;
}

typedef struct tagEDGE
{
	double xi;
	double dx;
	int ymax;
}EDGE;

void initScanLineNewEdgeTable(HDC hdc, std::vector<std::list<EDGE>>& slNet, OGPoly& py, 
	int& ymin, int& ymax, COLORREF color = RGB(255, 255, 255)) {
	EDGE e;
	int n = py.points.size();
	for (int i = 0; i < n; i++)
	{
		const OGPoint& ps = *(py.points[i]);
		const OGPoint& pe = *(py.points[(i + 1) % n]);
		const OGPoint& pss = *(py.points[(i - 1 + n) % n]);
		const OGPoint& pee = *(py.points[(i + 2) % n]);
		if (pe.y != ps.y)
		{
			e.dx = double(pe.x - ps.x) / double(pe.y - ps.y);
			if (pe.y > ps.y)
			{
				e.xi = ps.x;
				if (pee.y >= pe.y)
					e.ymax = pe.y - 1;
				else
					e.ymax = pe.y;
				slNet[ps.y - ymin].push_front(e);
			}
			else
			{
				e.xi = pe.x;
				if (pss.y >= ps.y)
					e.ymax = ps.y - 1;
				else
					e.ymax = ps.y;
				slNet[pe.y - ymin].push_front(e);
			}
		}
		else {
			if (pss.y < ps.y)
				ogDrawLine(hdc, ps.x, ps.y, pe.x, pe.y, color);
		}
	}
}

bool IsEdgeOutOfActive(EDGE e, int y)
{
	return (e.ymax == y);
}
void RemoveNonActiveEdgeFromAet(std::list<EDGE>& aet, int y)
{
	aet.remove_if(std::bind2nd(std::ptr_fun(IsEdgeOutOfActive), y));
}

void UpdateAetEdgeInfo(EDGE& e)
{
	e.xi += e.dx;
}

void UpdateAndResortAet(std::list<EDGE>& aet)
{
	for (auto& a : aet)
		UpdateAetEdgeInfo(a);
}

void InsertNetListToAet(std::list<EDGE>& net, std::list<EDGE>& aet) {
	for (auto it1 = net.begin(); it1 != net.end(); ++it1)
	{
		aet.push_back(*it1);
	}
}

void FillAetScanLine(HDC hdc, std::list<EDGE> aet, int y, COLORREF color = RGB(255, 255, 255)) {
	bool flag;
	flag = true;
	auto it = aet.begin();
	auto it1 = it;
	for (; it != aet.end(); it++)
	{
		it1 = it;
		it1++;
		if (it1 == aet.end())
		{
			break;
		}
		if (flag)
		{
			ogDrawLine(hdc, it->xi, y, it1->xi, y, color);
		}
		flag = !flag;
	}

}


void processScanLineFill(HDC hdc, std::vector< std::list<EDGE>>& slNet, int ymin, int ymax, COLORREF color = RGB(255, 255, 255)) {
	std::list<EDGE> aet;

	for (int y = ymin; y <= ymax; y++)
	{
		InsertNetListToAet(slNet[y - ymin], aet);
		FillAetScanLine(hdc, aet, y, color);
		//删除非活动边
		RemoveNonActiveEdgeFromAet(aet, y);
		//更新活动边表中每项的xi值，并根据xi重新排序
		UpdateAndResortAet(aet);
	}
}

int ogFillPoly(HDC hdc, OGPoly py, COLORREF color) {
	if (!py.isValid())
		return -1;
	int ymin = 0;
	int ymax = 0;
	getPolygonMinMax(py, ymin, ymax);
	std::vector< std::list<EDGE> > slNet(ymax - ymin + 1);
	initScanLineNewEdgeTable(hdc, slNet, py, ymin, ymax, color);
	processScanLineFill(hdc, slNet, ymin, ymax, color);
	for (auto l : slNet)
		l.clear();
	slNet.clear();
	return 1;
}


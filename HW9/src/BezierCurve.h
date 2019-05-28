#pragma once
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

struct point {
	float x;
	float y;
	point() {
		x = 0;
		y = 0;
	}
	point(float _x, float _y) {
		x = _x;
		y = _y;
	}
	point(const point &p) {
		x = p.x;
		y = p.y;
	}
};

class BezierCurve
{
public:
	BezierCurve();
	~BezierCurve();
	void drawBezier(vector<point> cpoint, int bsize);
	vector<point> getBpoint();
private:
	vector<point> bpoint;
};


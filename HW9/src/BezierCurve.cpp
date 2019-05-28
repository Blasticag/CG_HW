#include "BezierCurve.h"

BezierCurve::BezierCurve()
{
}


BezierCurve::~BezierCurve()
{
}

void BezierCurve::drawBezier(vector<point> cpoint, int bsize) {
	bpoint.clear();
	int n = cpoint.size();
	if (n < 0) {
		bpoint.clear();
		return;
	}
	if (n == 1 || bsize < 2) {
		bpoint = cpoint;
		return;
	}

	//Compute coeffitients
	vector<int> coeffitient;
	coeffitient.push_back(1);
	coeffitient.push_back(1);
	for (int i = 0; i < n - 2; i++) {
		for (int j = coeffitient.size() - 1; j > 0; j--) {
			coeffitient[j] = coeffitient[j - 1] + coeffitient[j];
		}
		coeffitient.push_back(1);
	}
	
	bpoint.push_back(cpoint[0]);
	for (int i = 1; i < bsize - 1; i++) {
		point b;
		float t = i / (float)(bsize - 1);
		for (int j = 0; j < n; j++) {
			float coef = pow(1 - t, n - 1 - j) * pow(t, j) * coeffitient[j];
			b.x += coef * cpoint[j].x;
			b.y += coef * cpoint[j].y;
		}
		bpoint.push_back(b);
	}
	bpoint.push_back(cpoint.back());
}

vector<point> BezierCurve::getBpoint()
{
	return bpoint;
}

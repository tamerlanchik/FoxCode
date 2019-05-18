#pragma once

#include <cmath>

struct Point {
	float x;
	float y;
	Point() {};
	Point(float _x, float _y);
	Point operator+ (const Point&);
	Point operator- (const Point&);
	Point operator* (const Point&);
	Point operator/ (const Point&);
	Point operator+ (float);
	Point operator/ (float);
	Point operator* (float);
	double Norm() const {
		return sqrt(x*x + y*y);
	}
};

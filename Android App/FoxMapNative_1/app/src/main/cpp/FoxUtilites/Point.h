#pragma once
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
};

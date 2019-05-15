#include "Point.h"

Point::Point(float _x, float _y) : x(_x), y(_y) {}

Point Point::operator+ (const Point& p2) {
	return Point(x + p2.x, y + p2.y);
}

Point Point::operator- (const Point& p2) {
	return Point(x - p2.x, y - p2.y);
}

Point Point::operator* (const Point& p2) {
	return Point(x * p2.x, y * p2.y);
}

Point Point::operator/ (const Point& p2) {
	return Point(x / p2.x, y / p2.y);
}

Point Point::operator+ (float a) {
	return Point(x + a, y + a);
}

Point Point::operator/ (float a) {
	return Point(x/a, y /a);
}

Point Point::operator* (float a) {
	return Point(x*a, y*a);
}
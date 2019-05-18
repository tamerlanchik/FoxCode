#pragma once

#include <cmath>

template<class T = float>
struct PointT {
	T x;
	T y;
	PointT() {};
	PointT(T _x, T _y);
	//explicit PointT(int _x, int _y) : PointT((float)_x, (float)_y) {};
	PointT<T> operator+ (const PointT<T>&);
	PointT<T> operator- (const PointT<T>&);
	PointT<T> operator* (const PointT<T>&);
	PointT<T> operator/ (const PointT<T>&);
	PointT<T> operator+ (T);
	PointT<T> operator/ (T);
	PointT<T> operator* (T);
	double Norm() const {
		return sqrt(x*x + y*y);
	}
};
typedef PointT<float> Point;


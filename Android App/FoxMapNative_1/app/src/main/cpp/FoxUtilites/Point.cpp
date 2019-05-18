#include "Point.h"

template<class T>
PointT<T>::PointT(T _x, T _y) : x(_x), y(_y) {}

template<class T>
PointT<T> PointT<T>::operator+ (const PointT<T>& p2) {
	return PointT<T>(x + p2.x, y + p2.y);
}

template<class T>
PointT<T> PointT<T>::operator- (const PointT<T>& p2) {
	return PointT<T>(x - p2.x, y - p2.y);
}

template<class T>
PointT<T> PointT<T>::operator* (const PointT<T>& p2) {
	return PointT<T>(x * p2.x, y * p2.y);
}

template<class T>
PointT<T> PointT<T>::operator/ (const PointT<T>& p2) {
	return PointT<T>(x / p2.x, y / p2.y);
}

template<class T>
PointT<T> PointT<T>::operator+ (T a) {
	return PointT<T>(x + a, y + a);
}

template<class T>
PointT<T> PointT<T>::operator/ (T a) {
	return PointT<T>(x/a, y /a);
}

template<class T>
PointT<T> PointT<T>::operator* (T a) {
	return PointT<T>(x*a, y*a);
}

template class PointT<float>;
template class PointT<size_t>;
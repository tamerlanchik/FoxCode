#include "pch.h"
#include "GraphConstructor.h"

template<class T>
GraphConstructor<T>::GraphConstructor(const std::vector<Hall> &halls, const std::vector<Room> &rooms){
	Halls = halls;
	Rooms = rooms;
}

template<class T>
GraphConstructor<T>::~GraphConstructor(){

}

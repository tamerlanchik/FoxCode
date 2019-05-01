#include "MapItem.h"


Room::Room(const float* vertices) {
	vertices_count_ = 12;
	vertices_ = new float[vertices_count_];
	memcpy(vertices_, vertices, vertices_count_*sizeof(float));
	int i = 0;
}
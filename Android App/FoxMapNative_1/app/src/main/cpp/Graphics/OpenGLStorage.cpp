#include "OpenGLStorage.h"
#include "MapItem.h"
#include <unistd.h>
#include <array>

typedef PointT<> Point;
const char OpenGLStorage::TAG[] = "OpenGLStorage";
OpenGLStorage::OpenGLStorage() : screen_dimensions_(Point(0,0))
{
	normalizing_matrix_ = glm::mat4(1.0);
	moving_matrix_ = glm::mat4(1.0f);
	scaling_matrix_ = glm::mat4(1.0f);
	updateTransformMatrix();
	Log::debug(TAG, "OpenGLStorage created!\n");
}
OpenGLStorage* OpenGLStorage::Get() {
	static OpenGLStorage instance_;
	return &instance_;
 }

bool OpenGLStorage::InflateStorage(DBAdapter& adapter) {
    if(!MapItemStorage::InflateStorage(adapter)){
        Log::debug(TAG, "Cannot inflate MapItemStorage");
        return false;
    }
    double max_val = MapItemStorage::getMaxCoordinateValue();
    Log::debug(TAG, (std::string("Max val: ") + std::to_string(max_val)).c_str());
    //map_dimensions_ = database_->GetMapDimensions();
	map_dimensions_ = Point(1000, 1000);
    // Переворот по Y
    normalizing_matrix_ = glm::scale(normalizing_matrix_, glm::vec3(1, -1, 1));
    // Сдвиг вправо вверх
    normalizing_matrix_ = glm::translate(normalizing_matrix_, glm::vec3(-0.5, -0.5, 0));
    //	Приведение масштаба к нормализованному
    normalizing_matrix_ = glm::scale(normalizing_matrix_,
                                     glm::vec3(1/map_dimensions_.x, 1/map_dimensions_.y, 1));

    /*glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO_);
    glGenVertexArrays(1, &VAO_room_);*/
    is_inflated_ = true;
    return true;
}

bool OpenGLStorage::InflateStorage() {
	if (!MapItemStorage::InflateStorage())
		return false;
	//map_dimensions_ = database_->GetMapDimensions();
	map_dimensions_ = Point(1000, 1000);
	// Переворот по Y
	normalizing_matrix_ = glm::scale(normalizing_matrix_, glm::vec3(1, -1, 1));
	// Сдвиг вправо вверх
	normalizing_matrix_ = glm::translate(normalizing_matrix_, glm::vec3(-0.5, -0.5, 0));
	//	Приведение масштаба к нормализованному
	normalizing_matrix_ = glm::scale(normalizing_matrix_,
					glm::vec3(1/map_dimensions_.x, 1/map_dimensions_.y, 1));

	return true;
}

void OpenGLStorage::UpdateScreenDimensions(size_t w, size_t h){
    screen_dimensions_ = Point(w,h);
    float screen_sides_ratio = ((float)w)/h;
    scaling_matrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(1, screen_sides_ratio, 1));
	updateTransformMatrix();
}

void OpenGLStorage::CommitMapMovement(int x, int y) {
	Point delta(x, -y);
	delta = delta*2;
	delta = delta/screen_dimensions_;
	moving_matrix_ = glm::translate(moving_matrix_, glm::vec3(delta.x, delta.y, 0));
	updateTransformMatrix();
}

void OpenGLStorage::CommitMapZoom(float d_z) {
	scaling_matrix_ = glm::scale(scaling_matrix_, glm::vec3(d_z, d_z, 1));
	updateTransformMatrix();
}

float* OpenGLStorage::GetRooms() {
	buffer_.clear();
	return getRooms();
}
float* OpenGLStorage::GetPassages() {
	buffer_.clear();
	return getPassages();
}

float* OpenGLStorage::GetObjects() {
	buffer_.clear();
	getPassages();
	getRooms();
	getPatches();
	return buffer_.data();
}

const glm::f32* OpenGLStorage::GetTransformMatrix() const {
	return glm::value_ptr(result_transform_matrix_);
}


void OpenGLStorage::NotifyStartWorking() {
	m_.lock();
}

void OpenGLStorage::NotifyStopWorking() {
	m_.unlock();
}
//--------Private-----------

float* OpenGLStorage::getRooms() {
	//rooms_buf_size_ = gls::Room::GetCount() * 8;
	buffer_map_.SetRooms(gls::Room::GetCount() * 8);
	buffer_.reserve(buffer_map_.GetTotal());
	std::vector<size_t> ind = { 0, 1,   2, 1,   2, 3,   0, 3 };

	for (gls::Room* obj : room_storage_) {
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
		    buffer_.push_back(obj->GetVertices()[i]);
		});
	}
	return buffer_.data();
}

float* OpenGLStorage::getPassages() {
	//passages_buf_size_ = gls::Passage::GetCount()*2*2*3;
	buffer_map_.SetPassages(gls::Passage::GetCount()*2*2*3);
	buffer_.reserve(buffer_map_.GetTotal());
    std::vector<size_t> ind = { 0, 1,   2, 1,   2, 3,
                                0, 1,   2, 3,   0, 3 };

	for (gls::Passage* obj : passage_storage_) {
        std::for_each(ind.begin(), ind.end(), [&](size_t& i){
            buffer_.push_back(obj->GetVertices()[i]);
        });
	}
	return buffer_.data();
}

float* OpenGLStorage::getPatches() {
	const float dW = map_dimensions_.x * 0.02;	// значение ширины квадрата заглушки
	buffer_map_.SetPatches(gls::Room::GetCount()*12);
	buffer_.reserve(buffer_map_.GetTotal());

	std::array<size_t, 12> ind = { 0, 1,   2, 1,   2, 3,
                                   0, 1,   2, 3,   0, 3 };
	std::array<float, 4> rect;
    float eps = 10;
    float dWx, dWy;
	for (gls::Room* obj : room_storage_) {
        dWx = eps/2; dWy = eps/2;
	    Point e = obj->GetEntry();
	    auto verts = obj->GetVertices();
	    if(abs(verts[0] - e.x) < eps || abs(verts[2] - e.x) < eps)
            dWy = dW;
        else
            dWx = dW;
		generateCenteredRectangle< std::array<float, 4> >(rect, e, dWx, dWy);
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
			buffer_.push_back(rect[i]);
		});
	}
	return buffer_.data();
}

void OpenGLStorage::updateTransformMatrix() {
	result_transform_matrix_ = moving_matrix_*scaling_matrix_*normalizing_matrix_;
}

template<class T>
void OpenGLStorage::generateCenteredRectangle(T& dest, const Point &center, const float width,
											  const float height) {
	if(dest.size() < 4) return;

	dest[0] = center.x - width;
	dest[1] = center.y - height;

	dest[2] = center.x + width;
	dest[3] = center.y + height;
}



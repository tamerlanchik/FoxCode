#include "OpenGLStorage.h"
#include "MapItem.h"
#include <unistd.h>
#include <array>
#include "config.h"
#include <cmath>

typedef PointT<> Point;
const char OpenGLStorage::TAG[] = "OpenGLStorage";
OpenGLStorage::OpenGLStorage() : screen_dimensions_(Point(0,0)), current_floor_(3)
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

    Log::debug(TAG, "Matrix inited");
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

float* OpenGLStorage::GetObjects() {
	return GetObjects(3);
}
float* OpenGLStorage::GetObjects(size_t floor) {
	current_floor_ = floor;
	buffer_.clear();
	getPassages();
	getRooms();
	getLifts();
	getSteps();
	getPatches();
	getPath();
	getMarks();
	return buffer_.data();
}

const glm::f32* OpenGLStorage::GetTransformMatrix() const {
	return glm::value_ptr(result_transform_matrix_);
}

bool OpenGLStorage::SetObjectMark(const std::string& name) {
	std::vector<std::string> names;
	if(name.find("Room") == std::string::npos)
	    names.push_back("Room " + name);
    else
        names.push_back(name);
	bool res = GetObjectsByNames(names, marks_, 0);
	return res;
}
void OpenGLStorage::SetRoute(std::vector<std::string> path){
    MapItemStorage::SetRoute(path);
    try {
        SetObjectMark(*path.begin());
        SetObjectMark(*(path.end() - 1));
    }catch(std::exception& e){
        Log::error(TAG, "Cannot find object " + *(path.begin()));
        throw(e);
    }
}
void OpenGLStorage::SetCurrentFloor(size_t new_floor) {
    current_floor_ = new_floor;
}

void OpenGLStorage::NotifyStartWorking() {
	m_.lock();
}

void OpenGLStorage::NotifyStopWorking() {
	m_.unlock();
}
//--------Private-----------

float* OpenGLStorage::getRooms() {
	buffer_map_.SetLocation(BufMap::R,
	        (storage_[current_floor_-1][(size_t)Type::R].size() +
                    storage_[current_floor_-1][(size_t)Type::L].size() +
                    storage_[current_floor_-1][(size_t)Type::S].size())*8);
	buffer_.reserve(buffer_map_.GetTotal());
	std::array<size_t, 8> ind = { 0, 1,   2, 1,   2, 3,   0, 3 };
	std::array<Type, 3> types = {Type::R, Type::L, Type::S};
	for(Type type : types){
		for(const gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)type]){
			std::for_each(ind.begin(), ind.end(), [&](size_t& i){
				buffer_.push_back(obj->GetVertices()[i]);
			});
		}
	}
	return buffer_.data();
}

float* OpenGLStorage::getPassages() {
    buffer_map_.SetLocation(BufMap::P, storage_[current_floor_-1][(size_t)Type::P].size() * 2*2*3);
	buffer_.reserve(buffer_map_.GetTotal());
    std::vector<size_t> ind = { 0, 1,   2, 1,   2, 3,
                                0, 1,   2, 3,   0, 3 };

	for (gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)Type::P]) {
        std::for_each(ind.begin(), ind.end(), [&](size_t& i){
            buffer_.push_back(obj->GetVertices()[i]);
        });
	}
	return buffer_.data();
}

float* OpenGLStorage::getPatches() {
    buffer_map_.SetLocation(BufMap::PT,
    		storage_[current_floor_-1][(size_t)Type::R].size()*conf::rect_vertex_count*conf::dims_count);

    buffer_.reserve(buffer_map_.GetTotal());

	std::array<size_t, 12> ind = { 0, 1,   2, 1,   2, 3,
                                   0, 1,   2, 3,   0, 3 };
	std::array<float, 4> rect;
	Point entry;
    float dWx, dWy;
	for (gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)Type::R]) {
        dWx = conf::door_dims[1]; dWy = dWx;
	    entry = static_cast<gls::Room*>(obj)->GetEntry();
	    auto verts = obj->GetVertices();
	    if(abs(verts[0] - entry.x) < conf::eps || abs(verts[2] - entry.x) < conf::eps)
            dWy = conf::door_dims[0];
        else
            dWx = conf::door_dims[0];

		generateCenteredRectangle< std::array<float, 4> >(rect, entry, dWx, dWy);
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
			buffer_.push_back(rect[i]);
		});
	}
	return buffer_.data();
}

float* OpenGLStorage::getLifts() {
	const size_t lines_count = 2;
    buffer_map_.SetLocation(BufMap::L,
    		storage_[current_floor_-1][(size_t)Type::L].size()*conf::dims_count*lines_count*2);
	buffer_.reserve(buffer_map_.GetTotal());
	std::array<size_t, 8> ind = {0, 1, 2, 3, 2, 1, 0, 3};	//перекрестье

	for (gls::MapItem *obj : storage_[current_floor_ - 1][(size_t)Type::L]) {
		std::for_each(ind.begin(), ind.end(), [&](size_t &i) {
			buffer_.push_back(obj->GetVertices()[i]);
		});
	}
	return buffer_.data();
}

float* OpenGLStorage::getSteps() {
	const size_t lines_count = 2;

    buffer_map_.SetLocation(BufMap::S,
    		storage_[current_floor_-1][(size_t)Type::S].size()
    		*conf::dims_count*conf::dims_count*lines_count*2);

	buffer_.reserve(buffer_map_.GetTotal());

	for (gls::MapItem *obj : storage_[current_floor_ - 1][(size_t)Type::S]) {
		const std::vector<float>& v = obj->GetVertices();
		Point l_t(v[0], v[1]);	//left-top corner
		Point r_b(v[2], v[3]);	//right-bottom corner
		Point step = (r_b - l_t)/(lines_count+1);
		for(Point i = l_t+step; i < r_b; i = i + step){
			buffer_.push_back(i.x);
			buffer_.push_back(l_t.y);
			buffer_.push_back(i.x);
			buffer_.push_back(r_b.y);

			buffer_.push_back(l_t.x);
			buffer_.push_back(i.y);
			buffer_.push_back(r_b.x);
			buffer_.push_back(i.y);
		}
	}
	return buffer_.data();
}

float* OpenGLStorage::getPath() {
	const size_t estimated_dash_count = 10;
	if(route_.size() == 0) return nullptr;
	buffer_.reserve(buffer_map_.GetTotal() +
	                    route_.size()*conf::dims_count*estimated_dash_count);
	std::vector<gls::MapItem*> path;

	if(!GetObjectsByNames(route_, path, current_floor_)){
	    Log::error(TAG, "Cannot find path objects");
	    return nullptr;
	};
	if(path.size() == 0) return nullptr;
	Point center1 = path[0]->GetCenter(), center2;
	const size_t step = conf::path_dash_step;
	std::function<size_t(Point&, Point&, size_t)> generate_line =
	        [&buffer_ = buffer_] (Point& center1, Point& center2, size_t step){
                Point st = (center2-center1) / ((center2-center1).Norm()/step);
                size_t counter = 0;
                for(Point p = center1; (p-(center2+st)).Norm() > step; p = p + st){
                    buffer_.push_back(p.x);
                    buffer_.push_back(p.y);
                    counter += 2;
                }
                return counter;
	        };
	size_t counter = 0;
	for(int i = 1; i < path.size(); ++i){
	    Log::debug(TAG, "Gen");
		center2 = path[i]->GetCenter();
		if(IDConverter::GetType(route_[i], conf::map_name_divider) == gls::MapKeys::Pass){
			// ищем точку коридора напротив комнаты
			const auto verts = path[i] ->GetVertices();
			if(path[i]->IsVertical()){
				center2.y = center1.y;
				counter+=generate_line(center1, center2, step);
				center1 = center2;
				if(i < path.size() - 1) {
					center2.y = path[i + 1]->GetCenter().y;
					counter += generate_line(center1, center2, step);
				}
			}else{
				center2.x = center1.x;
				counter += generate_line(center1, center2, step);
				center1 = center2;
				if(i < path.size() - 1) {
					center2.x = path[i + 1]->GetCenter().x;
					counter += generate_line(center1, center2, step);
				}
			}
		}
		else
			generate_line(center1, center2, step);	//для обычного обьекта
	    center1 = center2;
	}
    buffer_map_.SetLocation(BufMap::PATH, counter);
	return buffer_.data();
}

float* OpenGLStorage::getMarks() {
	buffer_map_.SetLocation(BufMap::MARK, marks_.size()*conf::dims_count*3);
	std::array<size_t, 6> ind = {0, 1, 2, 3, 4, 5};
	if(marks_.size() == 0) return nullptr;
	gls::MapItem* first = marks_[0];

	std::array<float, 6> triangle;
	for (gls::MapItem *obj : marks_) {
	    size_t floor = obj->GetId()[0] - '0';
	    if(floor != current_floor_) continue;
		generateCenteredTriangle(triangle, obj->GetCenter(), obj->GetDiagonal()/3);
		std::for_each(ind.begin(), ind.end(), [&](size_t &i) {
			buffer_.push_back(triangle[i]);
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

template<class T>
void OpenGLStorage::generateCenteredTriangle(T& dest, const Point &center, const float r) {
	const float K = 3.1415/180;
	if(dest.size() < 6) return;

	dest[0] = center.x;
	dest[1] = center.y + r;

	dest[2] = center.x + r*cos(30*K);
	dest[3] = center.y - r*sin(30*K);

	dest[4] = center.x - r*cos(30*K);
	dest[5] = center.y - r*sin(30*K);
}



#include "OpenGLStorage.h"
#include "MapItem.h"
#include <unistd.h>
#include <array>

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
	return buffer_.data();
}

const glm::f32* OpenGLStorage::GetTransformMatrix() const {
	return glm::value_ptr(result_transform_matrix_);
}


void OpenGLStorage::SetObjectMark(const int id) {

}

bool OpenGLStorage::SetObjectMark(const std::string& name) {
	return false;
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
	//rooms_buf_size_ = gls::Room::GetCount() * 8;
	//buffer_map_.SetRooms((gls::Room::GetCount() + gls::Lift::GetCount() + gls::Steps::GetCount()) * 8);
	//buffer_map_.SetRooms((room_storage_.size() + lift_storage_.size() + steps_storage_.size())*8);
	buffer_map_.SetLocation(BufMap::R,
	        (storage_[current_floor_-1][(size_t)Type::R].size() +
                    storage_[current_floor_-1][(size_t)Type::L].size() +
                    storage_[current_floor_-1][(size_t)Type::S].size())*8);
	buffer_.reserve(buffer_map_.GetTotal());
	std::vector<size_t> ind = { 0, 1,   2, 1,   2, 3,   0, 3 };
	std::vector<std::vector<int> > e;
	for(const gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)Type::R]){
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
		    buffer_.push_back(obj->GetVertices()[i]);
		});
	}
	for (gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)Type::L]) {
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
			buffer_.push_back(obj->GetVertices()[i]);
		});
	}
	for (gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)Type::S]) {
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
			buffer_.push_back(obj->GetVertices()[i]);
		});
	}
	return buffer_.data();
}

float* OpenGLStorage::getPassages() {
	//passages_buf_size_ = gls::Passage::GetCount()*2*2*3;
	//buffer_map_.SetPassages(gls::Passage::GetCount()*2*2*3);
	//buffer_map_.SetPassages(passage_storage_.size() * 2*2*3);
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
	const float dW = 5;	// значение ширины квадрата заглушки
	//buffer_map_.SetPatches(gls::Room::GetCount()*12);
    buffer_map_.SetLocation(BufMap::PT, storage_[current_floor_-1][(size_t)Type::R].size()*12);
	buffer_.reserve(buffer_map_.GetTotal());

	std::array<size_t, 12> ind = { 0, 1,   2, 1,   2, 3,
                                   0, 1,   2, 3,   0, 3 };
	std::array<float, 4> rect;
    float eps = 3;
    float dWx, dWy;
	for (gls::MapItem* obj : storage_[current_floor_ - 1][(size_t)Type::R]) {
        dWx = eps/2; dWy = eps/2;
	    Point e = static_cast<gls::Room*>(obj)->GetEntry();
	    auto verts = obj->GetVertices();
	    if(abs(verts[0] - e.x) < eps || abs(verts[2] - e.x) < eps) {
            dWy = dW;
        }
        else{
            dWx = dW;

        }
		generateCenteredRectangle< std::array<float, 4> >(rect, e, dWx, dWy);
		std::for_each(ind.begin(), ind.end(), [&](size_t& i){
			buffer_.push_back(rect[i]);
		});
	}
	return buffer_.data();
}

float* OpenGLStorage::getLifts() {
	//buffer_map_.SetLifts(gls::Lift::GetCount() * 8);
    buffer_map_.SetLocation(BufMap::L, storage_[current_floor_-1][(size_t)Type::L].size()*8);
	buffer_.reserve(buffer_map_.GetTotal());
	std::array<size_t, 8> ind = {0, 1, 2, 3, 2, 1, 0, 3};

	for (gls::MapItem *obj : storage_[current_floor_ - 1][(size_t)Type::L]) {
		std::for_each(ind.begin(), ind.end(), [&](size_t &i) {
			buffer_.push_back(obj->GetVertices()[i]);
		});
	}
	return buffer_.data();
}

float* OpenGLStorage::getSteps() {
    buffer_map_.SetLocation(BufMap::S, storage_[current_floor_-1][(size_t)Type::S].size() * 4 * 4);
	//buffer_map_.SetSteps(gls::Steps::GetCount() * 4 * 4);
	buffer_.reserve(buffer_map_.GetTotal());
	std::array<size_t, 8> ind = {0, 1, 2, 3, 2, 1, 0, 3};
	const size_t lines_count = 2;

	for (gls::MapItem *obj : storage_[current_floor_ - 1][(size_t)Type::S]) {
		const std::vector<float>& v = obj->GetVertices();
		Point l_t(v[0], v[1]);
		Point r_b(v[2], v[3]);
		Point d = (r_b - l_t)/(lines_count+1);
		for(Point i = l_t+d; i < r_b; i = i + d){
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
	//buffer_map_.SetLocation(BufMap::PATH, route_.size() * 2);
	if(route_.size() == 0) return nullptr;
	buffer_.reserve(buffer_map_.GetTotal() + route_.size()*10);
	const std::array<size_t, 4> ind = {0, 1, 2, 3};
	gls::MapItem search_item;
	std::vector<gls::MapItem*> path;
	path.reserve(route_.size());
	std::vector<Point> path_p;
	// "Room 312"
	for(const std::string& id : route_){
		int space = id.find(" ");
		if(space == std::string::npos){
		    Log::error(TAG, "Wrong id: " + id);
		    return nullptr;
		}
		size_t floor = id[space+1] - '0';
		search_item.SetID(id.substr(space+1, 5));
		size_t obj_type = 0;
		if(id.find("Room") != std::string::npos) obj_type = (size_t)Type::R;
        else if(id.find("Passage") != std::string::npos)obj_type = (size_t)Type::P;
        else if(id.find("Lift") != std::string::npos) obj_type = (size_t)Type::L;
        else if(id.find("Steps") != std::string::npos) obj_type = (size_t)Type::S;
        else continue;

		auto res = storage_[floor - 1][obj_type].find(&search_item);
		if(res!= storage_[current_floor_ - 1][(size_t)Type::R].end()) {
            Log::debug(TAG, "Found " + id);
            path.push_back(*(res));
            /*if(obj_type == Type::R){
                gls::Room* room = dynamic_cast<gls::Room*>(obj);
                if(!room){
                    Log::error(TAG, "Cannot cast to room: " + id);
                    return nullptr;
                }
                path_p.emplace_back(room->GetCenter());
            }else if(obj_type == Type::P){

            }*/
        }
		else{
		    Log::error(TAG, "Cannot found " + id);
		    return nullptr;
		}
        /*const gls::MapItem* obj = *(res);
		Point center = obj->GetCenter();
		buffer_.push_back(center.x);
        buffer_.push_back(center.y);*/
	}
	Point center1 = path[0]->GetCenter(), center2;
	const size_t step = 5;
	size_t counter = 0;
	std::function<void(Point&, Point&, size_t)> generate_line =
	        [&buffer_ = buffer_, &counter] (Point& center1, Point& center2, size_t step){
                Point st = (center2-center1) / ((center2-center1).Norm()/step);
                for(Point p = center1; (p-(center2+st)).Norm() > step; p = p + st){
                    buffer_.push_back(p.x);
                    buffer_.push_back(p.y);
                    counter += 2;
                }
	        };
	for(int i = 1; i < path.size(); ++i){
		center2 = path[i]->GetCenter();
		if(route_[i].find("Passage") != std::string::npos){
			// ищем точку коридора напротив комнаты
			const auto verts = path[i] ->GetVertices();
			if(path[i]->IsVertical()){
				center2.y = center1.y;
				generate_line(center1, center2, step);
				center1 = center2;
				if(i < path.size() - 1) {
					center2.y = path[i + 1]->GetCenter().y;
					generate_line(center1, center2, step);
				}
			}else{
				center2.x = center1.x;
				generate_line(center1, center2, step);
				center1 = center2;
				if(i < path.size() - 1) {
					center2.x = path[i + 1]->GetCenter().x;
					generate_line(center1, center2, step);
				}
			}
		}else{
			generate_line(center1, center2, step);
		}
	    /*Point st = (center2-center1) / ((center2-center1).Norm()/step);
	    for(Point p = center1; (p-(center2+st)).Norm() > step; p = p + st){
	        buffer_.push_back(p.x);
	        buffer_.push_back(p.y);
	        counter += 2;
	    }*/
	    center1 = center2;
	}
    buffer_map_.SetLocation(BufMap::PATH, counter);
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



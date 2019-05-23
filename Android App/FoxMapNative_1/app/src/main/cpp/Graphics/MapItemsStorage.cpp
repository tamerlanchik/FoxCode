#include "MapItemsStorage.h"

const char MapItemStorage::TAG[] = "MapItemsStorage";

MapItemStorage::MapItemStorage() : is_inflated_(false) {
	 Log::debug(TAG, "MapItemStorage created\n");
};

MapItemStorage* MapItemStorage::Get() {
	static MapItemStorage instance_;
	/*if (instance_ == NULL) {
		cerr << "instance is NULL\n";
		instance_ = new MapItemStorage();
	}*/
	return &instance_;
}

void MapItemStorage::SetDatabase(DataBase* database) {
	/*if (database)
		database_ = database;
	else
		Log::error(TAG, "Null database ptr!");*/
}

void MapItemStorage::SetDatabase(DBMaster* database) {
	/*if (database)
		db_ = database;
	else
		Log::error(TAG, "Null database ptr!");*/
}


bool MapItemStorage::InflateStorage() {
	return 1;
}
bool MapItemStorage::InflateStorage(MapItemStorage::DBAdapter & db) {
	class Converter{
	public:
		Point operator()(const Coordinate& coord){
			return Point(coord.x, coord.y);
		}
	} c;

	std::vector<Hall> passages = db.GetPassages();
	std::vector<Room> rooms = db.GetRooms();
	if(passages.size() == 0 || rooms.size() == 0){
		return false;
	}
	try {
        for (Hall h : passages) {
            size_t storey = IDConverter::GetFloor(h.ID, conf::map_name_divider);
            gls::Passage *pass = new gls::Passage(
                    c(h.LeftTop), c(h.RightBottom), IDConverter::GetId(h.ID, conf::map_name_divider));
            storage_[storey - 1][(size_t) Type::P].insert(pass);
            passage_storage_.push_back(pass);
        }
    }catch (std::exception& e){
	    Log::error(TAG, "Cannot inflate DB: Halls");
	    return false;
	}
	//storage_.emplace_back(passage_storage_);

	try {
        for (Room r : rooms) {
            size_t storey = IDConverter::GetFloor(r.ID, conf::map_name_divider);
            if (storey < 3 || storey > 4) {
                Log::error(TAG,
                           (r.ID + std::string(" Wrong storey ") + std::to_string(storey)).c_str());
                throw std::exception();
            }
            if (r.Type == "Room") {
                Log::debug(TAG, "Room");
                gls::Room *room = new gls::Room(c(r.LeftTop), c(r.RightBottom), c(r.Input[0]),
                                                IDConverter::GetId(r.ID, conf::map_name_divider));
                storage_[storey - 1][(int) Type::R].insert(room);
                room_storage_.push_back(room);
            } else {
                if (r.Type == "Lift") {
                    Log::debug(TAG, "Lift");
                    gls::Lift *room = new gls::Lift(c(r.LeftTop), c(r.RightBottom), IDConverter::GetId(r.ID, conf::map_name_divider));
                    storage_[storey - 1][(int) Type::L].insert(room);
                    lift_storage_.push_back(room);
                } else if (r.Type == "Steps") {
                    Log::debug(TAG, "Steps");
                    gls::Steps *room = new gls::Steps(c(r.LeftTop), c(r.RightBottom), IDConverter::GetId(r.ID, conf::map_name_divider));
                    storage_[storey - 1][(size_t) Type::S].insert(room);
                    steps_storage_.push_back(room);
                }
            }
        }
    }catch (std::exception& e){
	    Log::error(TAG, "Cannot inflate database: Other");
	    return false;
	}
	//storage_.emplace_back(room_storage_);

	is_inflated_ = true;
	Log::debug(TAG, "Inflated");
	return true;
}

bool MapItemStorage::GetObjectsByNames(const std::vector<std::string>& ids, std::vector<gls::MapItem*>& dest,
						size_t floor_condition) const{
	gls::MapItem search_item;
	dest.reserve(dest.size() + ids.size());
	for(const std::string& id : ids){
		size_t floor = IDConverter::GetFloor(id);
		if(floor != floor_condition) continue;
		search_item.SetID(IDConverter::GetId(id));
		size_t obj_type = 0;
		if(id.find("Room") != std::string::npos) obj_type = (size_t)Type::R;
		else if(id.find("Passage") != std::string::npos)obj_type = (size_t)Type::P;
		else if(id.find("Lift") != std::string::npos) obj_type = (size_t)Type::L;
		else if(id.find("Steps") != std::string::npos) obj_type = (size_t)Type::S;
		else continue;
		auto res = storage_[floor - 1][obj_type].find(&search_item);
		if(res!= storage_[floor - 1][obj_type].end()) {
			dest.push_back(*(res));
		}
		else{
			return false;
		}
	}
	return true;
}

void MapItemStorage::SetRoute(std::vector<std::string> r) {
    route_ = r;
}
bool MapItemStorage::IsInflated() const {
	return is_inflated_;
}

void MapItemStorage::convertPassages() {

}

double MapItemStorage::getMaxCoordinateValue() const {
	double max_val = 1000;
	/*for(std::vector& i : storage_){
		for(gls::MapItem* item : i)
			max_val = std::max(max_val, item->GetMaxValue());
	}*/
	return max_val;
}
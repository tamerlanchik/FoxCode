#include "MapItemsStorage.h"

const char MapItemStorage::TAG[] = "MapItemsStorage";

MapItemStorage::MapItemStorage() : is_inflated_(false){
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
	/*if (!database_)
		return 0;
	std::vector<DataBase::RoomParcel> data = database_->GetRooms();
	for (DataBase::RoomParcel c : data) {
		gls::Room* room = new gls::Room(c.lines);
		room_storage_.push_back(room);
	}
	data.~vector();
	std::vector<DataBase::PassageParcel> d = database_->GetPassages();
	for (DataBase::PassageParcel c : d) {
		gls::Passage* room = new gls::Passage(c.lines);
		passage_storage_.push_back(room);
	}
	is_inflated_ = true;*/
	return 1;
}
bool MapItemStorage::InflateStorage(MapItemStorage::DBAdapter & db) {
	class Converter{
	public:
		Point operator()(const Coordinate& coord){
			return Point(coord.x, coord.y);
		}
	} c;

	//storage_.reserve(2);

	std::vector<Hall> passages = db.GetPassages();
	std::vector<Room> rooms = db.GetRooms();
	for(Hall h : passages){
		gls::Passage* pass = new gls::Passage(c(h.LeftTop), c(h.RightBottom));
		passage_storage_.push_back(pass);
	}
	//storage_.emplace_back(passage_storage_);

	for(Room r : rooms){
		gls::Room* room = new gls::Room(c(r.LeftTop), c(r.RightBottom), c(r.Input[0]));
		room_storage_.push_back(room);
	}
	//storage_.emplace_back(room_storage_);

	is_inflated_ = true;
	Log::debug(TAG, "Inflated");
	return true;
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
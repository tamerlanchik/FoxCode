#include "MapItemsStorage.h"

const char MapItemStorage::TAG[] = "MapItemsStorage";

MapItemStorage::MapItemStorage() {
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
	if (database)
		database_ = database;
	else
		Log::error(TAG, "Null database ptr!");
}

bool MapItemStorage::InflateStorage() {
	if (!database_)
		return 0;
	std::vector<DataBase::RoomParcel> data = database_->GetRooms();
	for (DataBase::RoomParcel c : data) {
		gls::Room* room = new gls::Room(c.lines);
		room_storage_.push_back(room);
	}
	data.~vector();
	std::vector<DataBase::PassageParcel> d = database_->GetPassages();
	for (DataBase::PassageParcel c : d) {
		Passage* room = new Passage(c.lines);
		passage_storage_.push_back(room);
	}
	return 1;
}

bool MapItemStorage::InflateStorage(MapItemStorage::DBAdapter & db) {
	std::vector<Hall> passages = db.GetHalls();
	std::vector<Room> rooms = db.GetRooms();
	for(Hall h : passages){
		//Passage* room = new Passage(h.lines);
		Passage* pass = new Passage(h.LeftTop, h.RightBottom);
		passage_storage_.push_back(pass);
	}
	for(Room r : rooms){
		gls::Room* room;
		room_storage_.push_back(room);
	}
}
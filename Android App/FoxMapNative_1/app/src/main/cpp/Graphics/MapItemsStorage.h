//
// Created by Andrey on 21.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define FOXMAPNATIVE_1_MAPITEMSSTORAGE_H

#include <vector>
#include "Database/DataBase.h"
#include "FoxUtilites/Point.h"
#include "FoxUtilites/Log.h"
#include <system_error>
#include <Database/DBMaster.h>
#include <set>
#include <array>
#include <config.h>
#include "Database/Entity.h"
#include "MapItem.h"
using std::vector;

class MapItemStorage {
protected:
    //vector<std::vector<gls::MapItem*>& > storage_;
	vector<gls::Room*> room_storage_;
	vector<gls::Passage*> passage_storage_;
	vector<gls::Lift*> lift_storage_;
	std::vector<gls::Steps*> steps_storage_;
	std::array< std::array<
	                  std::set< gls::MapItem*, gls::MapItem::Compare>, 4>, 6> storage_;
	std::vector<std::string> route_;
	enum class Type { P = 0, R, S, L };
	//DataBase* database_;
	//DBMaster* db_;
	MapItemStorage();
	bool is_inflated_;
	double getMaxCoordinateValue() const;
	bool GetObjectsByNames(const std::vector<std::string>& ids, std::vector<gls::MapItem*>& dest,
						   size_t) const;
public:
	class IDConverter{
	public:
		static std::string GetId(const std::string& src, const char divider = ' ') throw(std::exception) {
			int space = src.find(divider);
			if(space == std::string::npos)
				throw std::exception();
			else
				return src.substr(space+1, conf::max_id_length);
		}
		static std::string GetType(const std::string& src, const char divider = ' ') throw(std::exception){
			int space = src.find(divider);
			if(space == std::string::npos)
				throw std::exception();
			else
				return src.substr(0, space);
		}
		static size_t GetFloor(const std::string& src, const char divider = ' ') throw(std::exception) {
			int space = src.find(divider);
			if(space == std::string::npos)
				throw std::exception();
			else
				return src[space+1] - '0';
		}
		static void ConvertString(std::string& src, const char divider = ' ') throw(std::exception){
			if(src.find(gls::MapKeys::Room) == std::string::npos){
				src = gls::MapKeys::Room + "_" + src;
			}
			int ind = src.find("ю");
			if(ind != std::string::npos){
				src.replace(ind, 2, "u");   //"ю" жирная и занимает 2 байта
			}

		}
	};
	class DBAdapter{
	public:
		virtual std::vector<Room> GetRooms() = 0;
		virtual std::vector<Hall> GetPassages() = 0;
	};
	virtual bool InflateStorage(DBAdapter&);
	static MapItemStorage* Get();

	void SetDatabase(DataBase*);
	void SetDatabase(DBMaster*);

	virtual bool InflateStorage();

	virtual void SetRoute(std::vector<std::string>);

	bool IsInflated() const;
private:
	static const char TAG[];
	void convertPassages();
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H

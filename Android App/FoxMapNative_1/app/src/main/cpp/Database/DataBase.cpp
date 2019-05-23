#include "DataBase.h"

//char DataBase::filename_[] = "map.txt";
const std::string DataBase::filename_ = "map.txt";

DataBase::DataBase() throw(std::system_error) : DBMaster(" ")
{
	std::ifstream file(filename_);
	if (!file.is_open())
		throw(std::system_error(ENOENT, std::iostream_category()));
	//assert(file.is_open());

	std::string s;
	char c = '\0';
	std::getline(file, s, c);
	file.close();
	//https://stackoverflow.com/questions/132358/how-to-read-file-content-into-istringstream
	try {
		parseFile(s);
	}
	catch (const std::logic_error& e) {
		throw;
	}
	//assert(parseFile(s));
}

#ifdef __ANDROID__
DataBase::DataBase(AAssetManager* asset_manager) : DataBase(asset_manager, filename_){}

DataBase::DataBase(AAssetManager* asset_manager, const std::string& db_name) : DBMaster(" ") {
	AAsset *shader_asset = AAssetManager_open(asset_manager, db_name.c_str(),
											  AASSET_MODE_BUFFER);
	assert(shader_asset);
	const void *shader_buf = AAsset_getBuffer(shader_asset);
	assert(shader_buf);
	off_t shader_length = AAsset_getLength(shader_asset);
	std::string res = std::string((const char*)shader_buf,
								  (size_t)shader_length);
	AAsset_close(shader_asset);
	assert(res.size() > 0);

	assert(parseFile(res));
}

DataBase::DataBase(AAssetManager* asset_manager, const std::vector<std::string>& db_names) : DBMaster(" ") {
    asset_manager_ = asset_manager;
    db_names_ = db_names;
	for(const std::string& db_name : db_names){
		AAsset *shader_asset = AAssetManager_open(asset_manager, db_name.c_str(),
												  AASSET_MODE_BUFFER);
		assert(shader_asset);
		const void *shader_buf = AAsset_getBuffer(shader_asset);
		assert(shader_buf);
		off_t shader_length = AAsset_getLength(shader_asset);
		std::string res = std::string((const char*)shader_buf,
									  (size_t)shader_length);
		AAsset_close(shader_asset);
		assert(res.size() > 0);

		assert(parseFile(res));
    }
}
#endif
DataBase::~DataBase()
{
}

size_t DataBase::GetRoomNumber() {
	return rooms_.size();
}
size_t DataBase::GetPassageNumber() {
	return passages_.size();
}
std::vector<Room> DataBase::GetRooms() {
	return rooms_;
}

std::vector<Hall> DataBase::GetHalls() {

	return passages_;
}
Point DataBase::GetMapDimensions() const {
	return dimensions_;
}

bool DataBase::parseFile(std::string& s) throw(std::logic_error){
    std::stringstream f;
    f << s;
	//------ReadHeader-----
	f >> dimensions_.x >> dimensions_.y;
	if (!dimensions_.x || !dimensions_.y) {
		//std::cout << "Cannot get dimensions\n";
		throw(std::logic_error("Cannot read dimensions"));
		return false;
	}

	std::string type;
	size_t size;
	float data;
    int i = 0;
	while (f >> type) {

	    ++i;
		char t = '0';
		size_t data_len = 4;
		if (type.find("Room")!= std::string::npos) {
			//std::cout << "\nRead room\n";
			t = 'r';
			room_types_.push_back('r');
			data_len = 6;
		}
		else if (type.find("Passage") != std::string::npos){
		    t = 'p';
		}else if(type.find("Lift") != std::string::npos){
		    t = 'l';
			room_types_.push_back('l');
		}else if(type.find("Steps") != std::string::npos){
		    t = 's';
			room_types_.push_back('s');
		}
		else {
			throw std::logic_error(std::string("Wrong object type ") + type + " i=" + std::to_string(i));
			return false;
		}
		f >> size;
		std::vector<float> coords;
		coords.reserve(data_len * size);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < data_len; j++) {
				f >> data;
				coords.push_back(data);
			}
		}
		if(t == 'p'){
            Hall parcel;
            parcel.LeftTop.x = coords[0];
            parcel.LeftTop.y = coords[1];
            parcel.RightBottom.x = coords[2];
            parcel.RightBottom.y = coords[3];
            parcel.ID = type.substr(8, type.size());
            passages_.push_back(parcel);
		}else{
		    Room parcel;
            parcel.LeftTop.x = coords[0];
            parcel.LeftTop.y = coords[1];
            parcel.RightBottom.x = coords[2];
            parcel.RightBottom.y = coords[3];

            switch(t){
                case 'r': {
                    parcel.ID = type.substr(5, type.size());
                    parcel.Type = "Room";
                    std::vector<Coordinate> input(1);
                    input[0].x = coords[4];
                    input[0].y = coords[5];
                    parcel.Input = input;
                    break;
                }
                case 'l': {
                    parcel.ID = type.substr(5, type.size());
                    parcel.Type = "Lift";
                    break;
                }
                case 's': {
                    parcel.ID = type.substr(6, type.size());
                    parcel.Type = "Steps";
                    break;
                }
            }
            rooms_.push_back(parcel);
		}

	}
	return true;
}
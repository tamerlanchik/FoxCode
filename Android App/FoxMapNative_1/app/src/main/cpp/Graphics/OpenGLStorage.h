#pragma once
#include "MapItemsStorage.h"
#include "FoxUtilites/Point.h"
#ifdef __ANDROID__
    #include <GLES3/gl3.h>
#else
	#define GLEW_STATIC
    #include <GL/glew.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <system_error>
#include <mutex>
#include <unistd.h>
#include <array>
#include "Database/Entity.h"
#include "RouteSearch/RouteSearch.h"
#include "vector"

class OpenGLStorage : public MapItemStorage
{
public:
	class BufMap{
		static const size_t types_count = 6;
	    // passages rooms lifts steps patches
		std::array<int, 7> map_ = {0};
	public:
		enum types {P = 0, R, L, S, PT, PATH};
		void SetLocation(int param, size_t count){ insert(param, count);}
		PointT<size_t> GetSectorRange(int param) const { return get(param); }
		bool IsFilled(size_t param) const  {
			return param < types_count && map_[param] != map_[param+1];
		}
		// вводим количество, храним позиции
		/*void SetPassages(size_t end){ insert(P, end); }	// нумерация с нуля
		void SetRooms(size_t end) { insert(R, end);}
		void SetLifts(size_t end) { insert(L, end); }
		void SetSteps(size_t end) { insert(S, end); }
		void SetPatches(size_t end) { insert(PT, end); }
        PointT<int> GetPassagesRange() const { return get(P);}
		PointT<int> GetRoomsRange() const { return get(R);}
		PointT<int> GetLiftsRange() const { return get(L);}
		PointT<int> GetStepsRange() const { return get(S);}
        PointT<int> GetPatchesRange() const { return get(PT);}*/
		int GetTotal() const { return map_[PT] + 1;}	// количество
	private:
	    void insert(size_t index, size_t val){
			int d = map_[index+1] - map_[index];
		    int delta = val - d;
		    std::for_each(map_.begin()+index+1, map_.end(), [&delta](int& elem){
		        elem += delta;
		    });
		    int i = 0;
		}
        PointT<size_t> get(size_t index) const {
		    return PointT<size_t>(map_[index], map_[index+1]-1);
		}
	};
	//const size_t BufMap::types_count = 5;
protected:
	OpenGLStorage();
	Point map_dimensions_;
	Point screen_dimensions_;
	glm::mat4 normalizing_matrix_;
	glm::mat4 moving_matrix_;
	glm::mat4 scaling_matrix_;
	glm::mat4 result_transform_matrix_;

	vector<float> buffer_;
	size_t rooms_buf_size_;
	size_t passages_buf_size_;
	size_t current_floor_;
	BufMap buffer_map_;
	float* getRooms();
	float* getPassages();
	float* getPatches();
	float* getLifts();
	float* getSteps();
	float* getPath();
	int data;
	std::mutex m_;
public:

	static OpenGLStorage* Get();
	virtual bool InflateStorage(DBAdapter&) override;
	virtual bool InflateStorage() override;
	const glm::f32* GetTransformMatrix() const;
	void UpdateScreenDimensions(size_t w, size_t h);
	float* GetRooms();
	float* GetPassages();
	float* GetObjects();
	float* GetObjects(size_t floor);
//	size_t GetPassagesBufSize() const { return passages_buf_size_; }
	const size_t GetBufferSize() const { return buffer_.size(); }
	const BufMap& GetBufferMap() const { return buffer_map_; }

	void CommitMapMovement(int x, int y);
	void CommitMapZoom(float delta);
	void SetCurrentFloor(size_t);

	//void SetRoute(const std::vector<int>& path);
	void SetObjectMark(const int id);
	bool SetObjectMark(const std::string& name);

	void NotifyStartWorking();
	void NotifyStopWorking();
private:
	void updateTransformMatrix();
	template<class T>
	void generateCenteredRectangle(T& dest, const Point& center,
	        const float width, const float height);
	static const char TAG[];
};


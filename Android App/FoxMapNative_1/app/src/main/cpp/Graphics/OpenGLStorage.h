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
#include "Database/Entity.h"

class OpenGLStorage : public MapItemStorage
{
public:
	class BufMap{
		size_t passages;
		size_t rooms;
		size_t patches;
	public:
		// вводим количество, храним позиции
		void SetPassages(size_t end){ passages = end - 1; }	// нумерация с нуля
		void SetRooms(size_t end) { rooms = passages + end;}
		void SetPatches(size_t end) { patches = rooms + end; }
        PointT<size_t> GetPassagesRange() const { return PointT<size_t>(0, passages);}
		PointT<size_t> GetRoomsRange() const { return PointT<size_t>(passages+1, rooms);}
        PointT<size_t> GetPatchesRange() const { return PointT<size_t>(rooms+1, patches);}
		size_t GetTotal() const { return patches + 1;}	// количество
	};
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
	BufMap buffer_map_;
	float* getRooms();
	float* getPassages();
	float* getPatches();
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
//	size_t GetPassagesBufSize() const { return passages_buf_size_; }
	const size_t GetBufferSize() const { return buffer_.size(); }
	const BufMap& GetBufferMap() const { return buffer_map_; }

	void CommitMapMovement(int x, int y);
	void CommitMapZoom(float delta);

	void NotifyStartWorking();
	void NotifyStopWorking();
private:
	void updateTransformMatrix();
	template<class T>
	void generateCenteredRectangle(T& dest, const Point& center,
	        const float width, const float height);
	static const char TAG[];
};


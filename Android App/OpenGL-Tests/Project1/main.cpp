#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <assert.h>

#include "MapDrawer.h"
#include "OpenGLStorage.h"

class GLMapView {
public:
	static void WindowChangeSizeCallback(GLFWwindow*, int, int);
	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
private:
	static float prev_x_;
	static float prev_y_;
	static bool is_dragging_;
};
float GLMapView::prev_x_ = 0;
float GLMapView::prev_y_ = 0;
bool GLMapView::is_dragging_ = false;

MapDrawer drawer;
OpenGLStorage* storage;
GLFWwindow* init(int w,int h);
int main()
{

	GLFWwindow* window = init(800, 600);	
	if (!window) {
		std::cerr << "Null window\n";
		glfwTerminate();
		return 0;
	}
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);

	GLMapView view;
	//MapDrawer drawer;
	drawer.Init();
	drawer.SurfaceCreated();
	drawer.SurfaceChanged(width, height);
	glfwSetWindowSizeCallback(window, GLMapView::WindowChangeSizeCallback);
	glfwSetMouseButtonCallback(window, GLMapView::MouseClickCallback);
	glfwSetCursorPosCallback(window, GLMapView::CursorPositionCallback);

	storage = OpenGLStorage::Get();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		drawer.Render();
		glfwSwapBuffers(window);
	}
	return 0;
}

GLFWwindow* init(int w, int h) {

	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(w, h, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
	return window;
}

void GLMapView::WindowChangeSizeCallback(GLFWwindow* window, int w, int h) {
	drawer.SurfaceChanged(w, h);
}

void GLMapView::CursorPositionCallback(GLFWwindow* window, double x, double y)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (storage)
			storage->CommitMapMovement(x - prev_x_, y - prev_y_);
		prev_x_ = x;
		prev_y_ = y;
	}
}

void GLMapView::MouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action) {
			if (!is_dragging_) {
				is_dragging_ = true;
				prev_x_ = (float) x;
				prev_y_ = (float) y;
				std::cout << "Start moving map\n";
			}
		}
		else if (GLFW_RELEASE == action){
			is_dragging_ = false;
			std::cout << "Stop moving map\n";
			}
	}
}
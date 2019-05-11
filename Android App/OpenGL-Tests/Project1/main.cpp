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

MapDrawer drawer;

GLFWwindow* init(int w,int h);
void WindowChangeSizeCallback(GLFWwindow*, int, int);
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

	//MapDrawer drawer;
	drawer.Init();
	drawer.SurfaceCreated();
	drawer.SurfaceChanged(width, height);
	glfwSetWindowSizeCallback(window, WindowChangeSizeCallback);

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

void WindowChangeSizeCallback(GLFWwindow* window, int w, int h) {
	drawer.SurfaceChanged(w, h);
}

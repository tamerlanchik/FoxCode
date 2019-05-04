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


std::string ReadFile(const char* name);
GLFWwindow* init();
void DrawTriangle();
int main()
{

	GLFWwindow* window = init();	
	if (!window) {
		std::cerr << "Null window\n";
		glfwTerminate();
		return 0;
	}
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);

	//glViewport(0, 0, width, height);

	//DrawTriangle();
	MapDrawer drawer;
	drawer.Init();
	drawer.SurfaceCreated();
	drawer.SurfaceChanged(width, height);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		drawer.Render();


		glfwSwapBuffers(window);
	}


	return 0;
}

void DrawTriangle() {
	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	std::string vertexShaderSourceString = ReadFile("vertex_shader.glsl");
	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	std::string fragmentShaderSourceString = ReadFile("fragment_shader.glsl");
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	assert(success);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	assert(success);

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. Затем установим указатели на вершинные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// 2. Используем нашу шейдерную программу
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::string ReadFile(const char* name) {
	std::ifstream file(name, std::ios::binary);
	assert(file.is_open());
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	std::string s(size, ' ');
	file.seekg(0);
	file.read(&s[0], size);
	file.close();
	return s;
}

GLFWwindow* init() {

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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
//
// Created by Andrey on 06.04.2019.
//

#include "ShaderMaster.h"
#include "Log.h"
#include <assert.h>
#include <string>

const char ShaderMaster::TAG[] = "ShaderMaster";

GLuint ShaderMaster::LoadShader(GLenum shader_type, const std::string& shader_raw){
    //Создаём пустой объект шейдера
    int shader_id = glCreateShader(shader_type);
    if(shader_id == 0){
        return 0;
    }
    //Получаем значение строки
    const char* shader_raw_buf = shader_raw.c_str();

    //  Принимает на вход ShaderID, количество шейдеров, массив указателей
    //  на строки с их исходным кодом и указатель на массив длин этих строк
    // (если NULL, то предполагается, что строки имеют терминальный символ '0\'
    glShaderSource(shader_id, 1, &shader_raw_buf, NULL);

    glCompileShader(shader_id);

    GLint shader_compile_status = 0;
    // Позволяет получить значение параметра свежесозданного шейдера
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compile_status);

    if( shader_compile_status == 0 ){
		printError(shader_compile_status, "Shader compile");
        glDeleteShader(shader_id);
        shader_id = 0;
    }
    return shader_id;

}

//  Два шейдера - вершинный и фрагментный - работают в паре
//  и потому объединяются в общий объект - "программу"
GLuint ShaderMaster::CreateProgram(const std::string& vertex_src,
        const std::string& fragment_src){
    GLuint vertex_id = LoadShader(GL_VERTEX_SHADER, vertex_src);
    GLuint fragment_id = LoadShader(GL_FRAGMENT_SHADER, fragment_src);
    if(vertex_id == 0 || fragment_id == 0){
        return 0;
    }

    return CreateProgram(vertex_id, fragment_id);
}

GLuint ShaderMaster::CreateProgram(const GLuint vertex_shader_id,
                                const GLuint fragment_shader_id){
    //  Создаём пустую программу
    GLuint program_id = glCreateProgram();
    if(program_id == 0){
        return 0;
    }

    //  Добавляем шейдеры
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    //  Собираем программу
    glLinkProgram(program_id);
    //  Проверяем статус линковки
    int link_status = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if(link_status == 0){
		printError(program_id, "Program compilaton");
        glDeleteProgram(program_id);
        program_id = 0;
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

std::string ShaderMaster::readFile(const char* name) {
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

void ShaderMaster::printError(GLuint item, const char* tag) {
	const size_t MESSAGE_MAX_LEN = 512;
	GLchar* infoLog = new GLchar[MESSAGE_MAX_LEN];
	glGetProgramInfoLog(item, MESSAGE_MAX_LEN, NULL, infoLog);
	//glGetProgramInfoLog(...);
	std::cout << "ERROR: " << tag << " failed\n" << infoLog << std::endl;
}
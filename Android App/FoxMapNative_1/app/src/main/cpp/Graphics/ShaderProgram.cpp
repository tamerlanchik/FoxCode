#include "ShaderProgram.h"

const char ShaderProgram::border_color_key[] = "u_color";
const char ShaderProgram::transform_matrix_key[] = "transform";


ShaderProgram::ShaderProgram(const char* vertex_shader_name,
							const char* fragment_shader_name)
{
	std::string vert_sh_src_ = ShaderMaster::GetShaderRaw(vertex_shader_name);
	assert(vert_sh_src_.size() > 0);
	vertex = vert_sh_src_;

	std::string frag_sh_src_ = ShaderMaster::GetShaderRaw(fragment_shader_name);
	assert(frag_sh_src_.size() > 0);
	frag = frag_sh_src_;
}

#ifdef __ANDROID__
ShaderProgram::ShaderProgram(AAssetManager* assert_manager, const char* vertex_shader_name,
							 const char* fragment_shader_name)
{
	std::string vert_sh_src_ = ShaderMaster::GetShaderRaw(assert_manager, vertex_shader_name);
	assert(vert_sh_src_.size() > 0);
	vertex = vert_sh_src_;

	std::string frag_sh_src_ = ShaderMaster::GetShaderRaw(assert_manager, fragment_shader_name);
	assert(frag_sh_src_.size() > 0);
	frag = frag_sh_src_;
}
#endif
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program_id_);
}

GLuint ShaderProgram::GetTransformParamLocation() const {
	return transform_param_location_;
}

GLuint ShaderProgram::GetColorParamLocation() const {
	return vertex_color_param_location_;
}

void ShaderProgram::Use() {
	glUseProgram(program_id_);
}

void ShaderProgram::SetTransformMatrix(const GLfloat* ptr) {
	glUniformMatrix4fv(transform_param_location_, 1, GL_FALSE, ptr);
}

void ShaderProgram::SetVertexColor(const float r, const float g, const float b) {
	glUniform3f(vertex_color_param_location_, r, g, b);
}

void ShaderProgram::SetVertexColor(const Colour& c) {
	SetVertexColor(c.r_, c.g_, c.b_);
}

void ShaderProgram::Generate() {
	program_id_ =  ShaderMaster::CreateProgram(this->vertex, this->frag);
	assert(program_id_ > 0);
	vertex_color_param_location_ = glGetUniformLocation(program_id_, border_color_key);
	transform_param_location_ = glGetUniformLocation(program_id_, transform_matrix_key);
}

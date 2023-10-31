#include "fprogram.hpp"
#include "futils.hpp"
#include "ftexture.hpp"
#include <glm/gtc/type_ptr.hpp>

FShader::FShader(const char* code, GLenum type)
    : _id(compile_shader(code, type), type) {}

FShader::FShader(std::filesystem::path file, GLenum type)
    : _id(compile_shader(readFile(file).c_str(), type), type) {}

FShader::operator GLuint() const {
    return _id;
}

FProgram::FProgram() {

}

FProgram::FProgram(std::vector<GLuint> shaders) {
    load(shaders);
}

FProgram::FProgram(std::filesystem::path vertex_shader_src, std::filesystem::path fragment_shader_src) {
    load({ compile_shader(readFile(fetch(vertex_shader_src)).c_str(), GL_VERTEX_SHADER),
    compile_shader(readFile(fetch(fragment_shader_src)).c_str(), GL_FRAGMENT_SHADER) });
}

void FProgram::load(std::vector<GLuint> shaders) {
    _id = FId(link_program(shaders), GL_PROGRAM);
    glUseProgram(_id);
}

FProgram::operator GLuint() const {
    return _id;
}

GLuint FProgram::location(std::string uniformName) const {
    return glGetUniformLocation(_id, uniformName.c_str());
}

template <>
void FProgram::set<glm::vec3>(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(location(name), 1, glm::value_ptr(value));
}
template <>
void FProgram::set<glm::vec4>(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(location(name), 1, glm::value_ptr(value));
}

template <>
void FProgram::set<glm::mat3>(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
}
template <>
void FProgram::set<glm::mat4>(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
}
template <>
void FProgram::set<GLuint>(const std::string& name, const GLuint& value) const {
    glUniform1ui(location(name), value);
}
template <>
void FProgram::set<FTexture>(const std::string& name, const FTexture& value) const {
    glUniform1ui(location(name), value);
}
template <>
void FProgram::set<GLint>(const std::string& name, const GLint& value) const {
    glUniform1i(location(name), value);
}
template <>
void FProgram::set<bool>(const std::string& name, const bool& value) const {
    glUniform1i(location(name), value);
}
template <>
void FProgram::set<GLfloat>(const std::string& name, const GLfloat& value) const {
    glUniform1f(location(name), value);
}
template <>
void FProgram::set<GLdouble>(const std::string& name, const GLdouble& value) const {
    glUniform1d(location(name), value);
}

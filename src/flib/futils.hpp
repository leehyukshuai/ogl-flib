#pragma once
#include "fheaders.hpp"
#include "fprogram.hpp"

GLFWwindow* initialize_glfw(int width, int height, const char* title);
GLuint compile_shader(const char* text, GLenum type);
GLuint link_program(const std::vector<GLuint>& shaders);
GLuint generate_texture(std::filesystem::path path, bool flip = false);
GLuint generate_cubeMap(std::filesystem::path path);
glm::mat3 calc_normal_trans(glm::mat4 model_trans);
std::string readFile(const std::filesystem::path& path);
std::filesystem::path fetch(std::filesystem::path);

#ifdef DEBUG
void print(const glm::mat4& m);
void print(const glm::vec4& v);
#endif

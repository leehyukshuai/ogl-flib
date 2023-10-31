#include "futils.hpp"
#include "fimage.hpp"
#include "config.h"
#include <fstream>
#include <sstream>

GLFWwindow* initialize_glfw(int width, int height, const char* title) {
    if (glfwInit() == GL_FALSE) {
        throw std::runtime_error("Failed to initialize glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    return window;
}

GLuint compile_shader(const char* text, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &text, NULL);
    glCompileShader(shader);

    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
        std::vector<GLchar> buffer(max_length);
        glGetShaderInfoLog(shader, max_length, &max_length, &buffer[0]);

        std::string error_log(buffer.data());

        glDeleteShader(shader); // Don't leak the shader.
        throw std::runtime_error("Failed to compile shader:\n" + error_log);
    }
    return shader;
}

GLuint link_program(const std::vector<GLuint>& shaders) {
    GLuint program = glCreateProgram();
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    int success;
    // check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
        std::vector<GLchar> buffer(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, &buffer[0]);

        std::string error_log(buffer.data());

        glDeleteProgram(program);
        throw std::runtime_error("Failed to link program:\n" + error_log);
    }
    return program;
}

GLuint generate_texture(std::filesystem::path path, bool flip) {
    GLuint texture;
    FImage image(path, flip);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, image.format(), image.width(), image.height(),
        0, image.format(), GL_UNSIGNED_BYTE, image.data());
    glGenerateTextureMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return texture;
}

GLuint generate_cubeMap(std::filesystem::path path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    std::vector<std::string> names{"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
    for (int i = 0; i < names.size(); ++i) {
        FImage image(path / names[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, image.format(), image.width(), image.height(), 0, image.format(), GL_UNSIGNED_BYTE, image.data());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateTextureMipmap(GL_TEXTURE_2D);
    return texture;
}

glm::mat3 calc_normal_trans(glm::mat4 model_trans) {
    return glm::mat3(glm::transpose(glm::inverse(model_trans)));
}

std::string readFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Can't find file " + path.string());
    }
    std::ifstream input(path);
    while (!input.is_open()) {
        throw std::runtime_error("Can't open file " + path.string());
    }
    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string code = buffer.str();
    input.close();

    return code;
}

std::filesystem::path fetch(std::filesystem::path path) {
    std::filesystem::path assets(ASSETS_PATH);
    return assets / path;
}

#ifdef DEBUG
void print(const glm::mat4& m) {
    for (int i = 0; i < 4; ++i) {
        print(m[i]);
    }
}

void print(const glm::vec4& v) {
    for (int i = 0; i < 4; ++i) {
        std::cout << std::showpos << std::left << std::setw(6) << std::setprecision(3) << v[i] << " ";
    }
    std::cout << std::noshowpos << std::right << "\n";
}

#endif
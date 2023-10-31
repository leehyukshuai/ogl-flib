#pragma once
#include "fheaders.hpp"
#include "fid.hpp"

class FShader {
    FId _id;
public:
    FShader(const char* code, GLenum type);
    FShader(std::filesystem::path file, GLenum type);
    operator GLuint() const;
};

class FProgram {
    FId _id;
    GLuint location(std::string uniformName) const;
public:
    FProgram();
    FProgram(std::vector<GLuint> shaders);
    FProgram(std::filesystem::path vertex_shader_src, std::filesystem::path fragment_shader_src);
    void load(std::vector<GLuint> shaders);
    operator GLuint() const;
    template<typename T>
    void set(const std::string& name, const T& value) const;
};

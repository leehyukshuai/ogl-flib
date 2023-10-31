#include "fid.hpp"

FId::FIdData::FIdData(GLenum type) : _type(type) {
    switch (_type) {
    case GL_PROGRAM:
        _id = glCreateProgram();
        break;
    case GL_VERTEX_SHADER:
    case GL_FRAGMENT_SHADER:
        _id = glCreateShader(type);
        break;
    case GL_VERTEX_ARRAY:
        glCreateVertexArrays(1, &_id);
        break;
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
        glCreateBuffers(1, &_id);
        break;
    case GL_TEXTURE_2D:
        glCreateTextures(GL_TEXTURE_2D, 1, &_id);
        break;
    case GL_TEXTURE_CUBE_MAP:
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);
        break;
    case GL_RENDERBUFFER:
        glGenRenderbuffers(1, &_id);
        break;
    case GL_FRAMEBUFFER:
        glGenFramebuffers(1, &_id);
        break;
    default:
        throw std::runtime_error("Unknown FId data type");
    }
}

FId::FIdData::FIdData(GLuint id, GLenum type) : _id(id), _type(type) {}

FId::FIdData::~FIdData() {
    switch (_type) {
    case GL_PROGRAM:
        glDeleteProgram(_id);
        break;
    case GL_VERTEX_SHADER:
    case GL_FRAGMENT_SHADER:
        glDeleteShader(_id);
        break;
    case GL_VERTEX_ARRAY:
        glDeleteVertexArrays(1, &_id);
        break;
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
        glDeleteBuffers(1, &_id);
        break;
    case GL_TEXTURE_2D:
    case GL_TEXTURE_CUBE_MAP:
        glDeleteTextures(1, &_id);
        break;
    case GL_RENDERBUFFER:
        glDeleteRenderbuffers(1, &_id);
        break;
    case GL_FRAMEBUFFER:
        glDeleteFramebuffers(1, &_id);
        break;
    }
}

FId::FIdData::operator GLuint() const {
    return _id;
}

FId::FId() {}

FId::FId(GLenum type) : _pid(std::make_shared<FIdData>(type)) {}

FId::FId(GLuint id, GLenum type) : _pid(std::make_shared<FIdData>(id, type)) {}

FId::operator GLuint() const {
    if (_pid) {
        return (unsigned)*_pid;
    } else {
        throw std::runtime_error("Failed to access FId: FId is uninitialized");
    }
}

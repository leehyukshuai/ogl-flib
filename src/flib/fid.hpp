#pragma once
#include "fheaders.hpp"

// 可以自动创建以及释放的智能ID
class FId {
    class FIdData {
        GLuint _id;
        GLenum _type;
    public:
        FIdData(GLenum type);
        FIdData(GLuint id, GLenum type);
        ~FIdData();
        operator GLuint() const;
    };
    std::shared_ptr<FIdData> _pid;
public:
    FId();
    FId(GLenum type);
    FId(GLuint id, GLenum type);
    operator GLuint() const;
};

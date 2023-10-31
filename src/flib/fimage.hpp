#pragma once
#include "fheaders.hpp"

class FImage {
    std::filesystem::path _path;
    int _width, _height;
    int _channels;
    void* _data;
public:
    FImage(std::filesystem::path path, bool flip = false);
    ~FImage();
    int width() const;
    int height() const;
    GLenum format() const;
    const void* data() const;
};

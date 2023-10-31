#include "fimage.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

FImage::FImage(std::filesystem::path path, bool flip) : _path(path) {
    if (std::filesystem::exists(path)) {
        std::cout << "Loading file: " + path.string() << "\n";
        stbi_set_flip_vertically_on_load(flip);
        _data = stbi_load(path.string().c_str(), &_width, &_height, &_channels, 0);
        if (_data == nullptr) {
            throw std::runtime_error("Failed to load " + path.string());
        }
    } else {
        throw std::runtime_error("Failed to find " + path.string());
    }
}

FImage::~FImage() {
    stbi_image_free(_data);
}

int FImage::width() const {
    return _width;
}

int FImage::height() const {
    return _height;
}

GLenum FImage::format() const {
    switch (_channels) {
    case 1:
        return GL_R;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        throw std::runtime_error("Unsupported image format: " + _path.extension().string());
    }
}

const void* FImage::data() const {
    return _data;
}

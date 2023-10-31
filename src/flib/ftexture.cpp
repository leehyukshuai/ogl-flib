#include "ftexture.hpp"
#include "futils.hpp"

std::unordered_map<std::string, FId> FTexture::visited;

FTexture::FTexture(std::filesystem::path path, bool flip) {
    if (visited.find(path.string()) == visited.end()) {
        _id = FId(generate_texture(path, flip), GL_TEXTURE_2D);
        visited[path.string()] = _id;
    } else {
        _id = visited[path.string()];
    }
}

FTexture::operator GLuint() const {
    return _id;
}

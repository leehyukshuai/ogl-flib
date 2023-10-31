#pragma once
#include "fid.hpp"

class FTexture {
    FId _id;
    static std::unordered_map<std::string, FId> visited;
public:
    FTexture(std::filesystem::path path, bool flip = false);
    operator GLuint()const;
};

#pragma once
#include "fheaders.hpp"
#include "fprogram.hpp"
#include "ftexture.hpp"
#include "fid.hpp"

struct FVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct FMaterial {
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float _shininess;
    std::optional<FTexture> _reflect_map;
    std::optional<FTexture> _diffuse_map;
    std::optional<FTexture> _specular_map;
    std::optional<FTexture> _emissive_map;
    bool _constant;
    glm::vec3 _const_color;
public:
    FMaterial();
    // 生成const材质(不受光照影响)
    FMaterial(glm::vec3 const_color);
    FMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess = 32.0f);
};

class FMesh {
    FId _vao, _vbo, _ebo;
    unsigned _verticesNum;

    FMaterial _material;
public:
    FMesh();
    FMesh(std::vector<FVertex> vertices, std::vector<unsigned> indices, FMaterial material);
    void render(const FProgram&program);
    FMaterial getMaterial() const;
    FMaterial setMaterial(const FMaterial &material);
};

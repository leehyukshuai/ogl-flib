#pragma once
#include "fheaders.hpp"
#include "fmesh.hpp"
#include "fnode.hpp"
#include <assimp/scene.h>

class FModel: public FNode {
    static std::unordered_map<std::string, std::vector<FMesh>> visited;
    std::vector<FMesh> _meshes;
    bool _blender;

    // directory is all texture maps' parent directory
    std::filesystem::path directory;
    // loadModel将会把模型内容写到_meshes所指向的空间
    void loadModel(std::filesystem::path path);
    void processNode(aiNode* node, const aiScene* scene);
    FMesh processMesh(aiMesh* mesh, const aiScene* scene);
    void render(FScene *scene) override;

public:
    FModel(std::filesystem::path path, FNode* parent = nullptr);
    void setMaterial(const FMaterial& material);
    void setBlender(bool blender);
    bool getBlender() const;
};

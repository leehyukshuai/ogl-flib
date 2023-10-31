#include "fmodel.hpp"
#include "futils.hpp"
#include "fscene.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

std::unordered_map<std::string, std::vector<FMesh>> FModel::visited;

FModel::FModel(std::filesystem::path path, FNode* parent) : FNode(parent), _blender(false) {
    if (visited.find(path.string()) == visited.end()) {
        loadModel(path);
        visited[path.string()] = _meshes;
    } else {
        _meshes = visited[path.string()];
    }
}

void FModel::loadModel(std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Can't find file " + path.string());
    }
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(std::string("Failed to load model: ") + import.GetErrorString());
    }
    directory = path.parent_path().make_preferred();

    processNode(scene->mRootNode, scene);
}

void FModel::processNode(aiNode* node, const aiScene* scene) {
    // 处理节点所有的网格（如果有的话）
    for (unsigned i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

FMesh FModel::processMesh(aiMesh* mesh, const aiScene* scene) {
    // 处理顶点位置、法线和纹理坐标
    std::vector<FVertex> vertices;
    for (unsigned i = 0; i < mesh->mNumVertices; i++) {
        FVertex vertex;

        auto position = mesh->mVertices[i];
        vertex.position = { position.x, position.y, position.z };
        if (mesh->HasNormals()) {
            auto normal = mesh->mNormals[i];
            vertex.normal = { normal.x, normal.y, normal.z };
        }
        if (mesh->HasTextureCoords(0)) {
            auto texCoord = mesh->mTextureCoords[0][i];
            vertex.texCoord = { texCoord.x, texCoord.y };
        }

        vertices.push_back(vertex);
    }
    // 处理索引
    std::vector<unsigned> indices;
    for (unsigned i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // 处理材质
    FMaterial material;
    // 无论如何，每一个mesh都会有对应的材质，即便没有声明，也会有默认材质
    aiMaterial* amaterial = scene->mMaterials[mesh->mMaterialIndex];
    // 获取材质的颜色信息
    if (std::string(amaterial->GetName().C_Str()) == "DefaultMaterial") {
        material._ambient = { 0.2, 0.2, 0.2 };
        material._diffuse = { 0.6, 0.6, 0.6 };
        material._specular = { 0.3, 0.3, 0.3 };
        material._shininess = 32.0f;
    } else {
        aiColor3D color; float shiness;
        amaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material._ambient = { color.r, color.g, color.b };
        amaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material._diffuse = { color.r, color.g, color.b };
        amaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material._specular = { color.r, color.g, color.b };
        amaterial->Get(AI_MATKEY_SHININESS, shiness);
        material._shininess = shiness;
        // 获取材质的纹理信息
        // 同一种类型的材质可能有多个，然后之后可以通过混合方式和混合因子等属性混合，
        // 在此我们只考虑一种材质的情况（并忽略更多的材质以及混合方式）
        aiString textureName; const char* name = textureName.C_Str();
        if (amaterial->GetTextureCount(aiTextureType_AMBIENT) > 0) {
            amaterial->GetTexture(aiTextureType_AMBIENT, 0, &textureName);
            material._reflect_map = FTexture(directory / name);
        }
        if (amaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            amaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
            material._diffuse_map = FTexture(directory / name);
        }
        if (amaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            amaterial->GetTexture(aiTextureType_SPECULAR, 0, &textureName);
            material._specular_map = FTexture(directory / name);
        }
        if (amaterial->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
            amaterial->GetTexture(aiTextureType_EMISSIVE, 0, &textureName);
            material._emissive_map = FTexture(directory / name);
        }
    }

    return FMesh(vertices, indices, material);
}

void FModel::render(FScene *scene) {
    glm::mat4 model_trans = scene->trans(this) * _trans;
    glm::mat3 normal_trans = calc_normal_trans(model_trans);
    scene->program().set("model_trans", model_trans);
    scene->program().set("normal_trans", normal_trans);
    for (FMesh& mesh : _meshes) {
        mesh.render(scene->program());
    }
}

void FModel::setMaterial(const FMaterial& material) {
    for (FMesh& mesh : _meshes) {
        mesh.setMaterial(material);
    }
}

void FModel::setBlender(bool blender) {
    _blender = blender;
}

bool FModel::getBlender() const {
    return _blender;
}

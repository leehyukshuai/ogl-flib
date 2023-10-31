#include "flib.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using std::cout;

void processMesh(aiMesh* mesh, const aiScene* scene) {
    cout << mesh->mName.C_Str() << "\n";
    for (int i = 0; i < mesh->mNumVertices; ++i) {
        aiVector3D pos = mesh->mVertices[i];
        cout << "v " << pos.x << " " << pos.y << " " << pos.z << "\n";
    }
    if (mesh->HasNormals()) {
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D norm = mesh->mNormals[i];
            cout << "vn " << norm.x << " " << norm.y << " " << norm.z << "\n";
        }
    }
    if (mesh->HasTextureCoords(0)) {
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D texCoord = mesh->mTextureCoords[0][i];
            cout << "vt " << texCoord.x << " " << texCoord.y << " " << texCoord.z << "\n";
        }
    }
    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        cout << "f ";
        for (int i = 0; i < face.mNumIndices; ++i) {
            cout << face.mIndices[i] << " ";
        }
        cout << "\n";
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 获取材质的名称
    aiString name;
    material->Get(AI_MATKEY_NAME, name);
    std::cout << "Material name: " << name.C_Str() << std::endl;

    // 获取材质的颜色信息
    aiColor3D ambientColor;
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
    std::cout << "Ambient color: (" << ambientColor.r << ", " << ambientColor.g << ", " << ambientColor.b << ")" << std::endl;

    aiColor3D diffuseColor;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    std::cout << "Diffuse color: (" << diffuseColor.r << ", " << diffuseColor.g << ", " << diffuseColor.b << ")" << std::endl;

    aiColor3D specularColor;
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
    std::cout << "Specular color: (" << specularColor.r << ", " << specularColor.g << ", " << specularColor.b << ")" << std::endl;

    int shininess;
    material->Get(AI_MATKEY_SHININESS, shininess);
    std::cout << "Shininess: " << shininess << std::endl;

    // 获取材质的纹理信息
    // 同一种类型的材质可能有多个，然后之后可以通过混合方式和混合因子等属性混合，
    // 在此我们只考虑一种材质的情况（并忽略更多的材质以及混合方式）
    aiString texturePath;
    if (material->GetTextureCount(aiTextureType_AMBIENT) > 0) {
        material->GetTexture(aiTextureType_AMBIENT, 0, &texturePath);
        std::cout << "Ambient texture path: " << texturePath.C_Str() << std::endl;
    }

    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
        std::cout << "Diffuse texture path: " << texturePath.C_Str() << std::endl;
    }

    if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
        material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
        std::cout << "Specular texture path: " << texturePath.C_Str() << std::endl;
    }
    if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
        material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath);
        std::cout << "Emissive texture path: " << texturePath.C_Str() << std::endl;
    }
    if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
        material->GetTexture(aiTextureType_NORMALS, 0, &texturePath);
        std::cout << "Normals texture path: " << texturePath.C_Str() << std::endl;
    }
    if (material->GetTextureCount(aiTextureType_UNKNOWN) > 0) {
        material->GetTexture(aiTextureType_UNKNOWN, 0, &texturePath);
        std::cout << "Unknown texture path: " << texturePath.C_Str() << std::endl;
    }
}

void processNode(aiNode* node, const aiScene* scene, int tab = 0) {
    cout << tab << " " << node->mName.C_Str() << "\n";
    for (int i = 0; i < node->mNumMeshes; ++i) {
        processMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    for (int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, tab + 1);
    }
}


void load(std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Can't find " + path.string());
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(std::string("Error: ") + importer.GetErrorString());
    }

    processNode(scene->mRootNode, scene);
}

int main() {
    std::string path;
    std::cin >> path;
    load(fetch(path));
}

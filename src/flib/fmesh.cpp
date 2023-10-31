#include "fmesh.hpp"

FMaterial::FMaterial() : _constant(false) {}

FMaterial::FMaterial(glm::vec3 const_color)
    : _const_color(const_color), _constant(true) {}

FMaterial::FMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess), _constant(false) {}

FMesh::FMesh() {}

FMesh::FMesh(std::vector<FVertex> vertices, std::vector<unsigned> indices, FMaterial material)
    : _verticesNum(indices.size()), _material(material), _vao(GL_VERTEX_ARRAY),
    _vbo(GL_ARRAY_BUFFER), _ebo(GL_ELEMENT_ARRAY_BUFFER) {

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FVertex), (void*)offsetof(FVertex, texCoord));
    glBindVertexArray(0);
}

void FMesh::render(const FProgram& program) {
    program.set("material.constant", _material._constant);
    program.set("material.const_color", _material._const_color);
    program.set("material.ambient", _material._ambient);
    program.set("material.diffuse", _material._diffuse);
    program.set("material.specular", _material._specular);
    program.set("material.shininess", _material._shininess);
    // GL_TEXTURE0 is blank!(resevered to potential skybox)
    if (_material._diffuse_map.has_value()) {
        program.set("material.diffuse_map_exists", 1);
        glActiveTexture(GL_TEXTURE1);
        program.set("material.diffuse_map", 1);
        glBindTexture(GL_TEXTURE_2D, _material._diffuse_map.value());
    } else {
        program.set("material.diffuse_map_exists", 0);
    }
    if (_material._specular_map.has_value()) {
        program.set("material.specular_map_exists", 1);
        glActiveTexture(GL_TEXTURE2);
        program.set("material.specular_map", 2);
        glBindTexture(GL_TEXTURE_2D, _material._specular_map.value());
    } else {
        program.set("material.specular_map_exists", 0);
    }
    if (_material._emissive_map.has_value()) {
        program.set("material.emissive_map_exists", 1);
        glActiveTexture(GL_TEXTURE3);
        program.set("material.emissive_map", 3);
        glBindTexture(GL_TEXTURE_2D, _material._emissive_map.value());
    } else {
        program.set("material.emissive_map_exists", 0);
    }
    if (_material._reflect_map.has_value()) {
        program.set("material.reflect_map_exists", 1);
        glActiveTexture(GL_TEXTURE4);
        program.set("material.reflect_map", 4);
        glBindTexture(GL_TEXTURE_2D, _material._reflect_map.value());
    } else {
        program.set("material.reflect_map_exists", 0);
    }
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _verticesNum, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

FMaterial FMesh::getMaterial() const {
    return _material;
}

FMaterial FMesh::setMaterial(const FMaterial& material) {
    FMaterial old_material = _material;
    _material = material;
    return old_material;
}

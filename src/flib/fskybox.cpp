#include "fskybox.hpp"
#include "futils.hpp"
#include "fscene.hpp"

FProgram FSkyBox::_program;
FId FSkyBox::_vao;
FId FSkyBox::_vbo;

// path: the parent directory of all six faces of skybox
FSkyBox::FSkyBox(const std::filesystem::path& path) {
    _id = FId(generate_cubeMap(path), GL_TEXTURE_CUBE_MAP);
    static bool initialized = false;
    if (!initialized) {
        initialize();
        initialized = true;
    }
}

void FSkyBox::update(float delta) {}

void FSkyBox::render(FScene* scene) {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(_program);
    _program.set("camera_view", scene->camera().view());
    _program.set("camera_projection", scene->camera().projection());
    _program.set("cubemap", (GLuint)_id);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(scene->program());
    glDepthFunc(GL_LESS);
}

void FSkyBox::initialize() {
    _vao = FId(GL_VERTEX_ARRAY);
    _vbo = FId(GL_ARRAY_BUFFER);
    _program = FProgram(fetch("shaders/skybox.vert"), fetch("shaders/skybox.frag"));

    float skyboxVertices[] = {
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
          1.0f, -1.0f,  1.0f
    };
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
#pragma once
#include "fnode.hpp"

class FSkyBox : public FNode {
    static void initialize();
    static FProgram _program;
    static FId _vao, _vbo;
    FId _id;
protected:
    void update(float delta) override;
    void render(FScene *scene) override;
public:
    FSkyBox(const std::filesystem::path &path);
};

#pragma once
#include "fapplication.hpp"
#include "fscene.hpp"

class FSceneApp: public FApplication {
protected:
    FScene scene;
public:
    FSceneApp(int width, int height, const std::string& title);

protected:
    void loop() override;
    void key_callback(int key, int scankey, int action, int mods) override;
    void mouse_move_callback(double x, double y) override;
    void scroll_callback(double x, double y) override;
    void framebuffer_size_callback(int w, int h) override;
};

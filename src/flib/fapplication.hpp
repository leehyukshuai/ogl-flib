#pragma once
#include "fheaders.hpp"

class FApplication {
private:
    GLFWwindow* window;
    float _lastTime, _deltaTime;
    // for better performance, false by default
    bool _disable_imgui;
public:
    FApplication(int width, int height, const std::string& title);
    ~FApplication();
    void run();
    float deltaTime() const;
    float currentTime() const;
    void disableImGui(bool flag);
    float aspect() const;
    int width() const;
    int height() const;
    void setWindowTitle(const std::string& title);
    void setWindowSize(int width, int height);
protected:
    virtual void loop();
    virtual void key_callback(int key, int scankey, int action, int mods);
    virtual void mouse_move_callback(double x, double y);
    virtual void scroll_callback(double x, double y);
    virtual void framebuffer_size_callback(int w, int h);
private:
    void initGLEW();
    void initImGui();
    void initGLFW(int width, int height, const std::string& title);
    static void window_key_callback(GLFWwindow* window, int key, int scankey, int action, int mods);
    static void window_mouse_move_callback(GLFWwindow* window, double x, double y);
    static void window_scroll_callback(GLFWwindow* window, double x, double y);
    static void window_framebuffer_size_callback(GLFWwindow* window, int w, int h);
};

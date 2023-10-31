#include "fapplication.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

FApplication::FApplication(int width, int height, const std::string& title) {
    initGLFW(width, height, title);
    initGLEW();
    initImGui();
}

FApplication::~FApplication() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void FApplication::run() {
    while (glfwWindowShouldClose(window) == false) {
        float currentTime = glfwGetTime();
        _deltaTime = currentTime - _lastTime;
        _lastTime = currentTime;
        if (_disable_imgui) {
            this->loop();
        } else {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            this->loop();
            GLint polygonMode[2];
            glGetIntegerv(GL_POLYGON_MODE, polygonMode);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (polygonMode[0] == GL_LINE) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void FApplication::initGLEW() {
    GLenum e = glewInit();
    if (e != GLEW_OK) {
        std::cerr << glewGetErrorString(e) << std::endl;
        throw std::runtime_error("Failed to init glew");
    }
}

void FApplication::initImGui() {
    ImGui::CreateContext();
    // 在高分屏上字体太小了，所以需要放大一些
    ImGui::GetIO().FontGlobalScale = 1.2f;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    _disable_imgui = false;
}

void FApplication::initGLFW(int width, int height, const std::string& title) {
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to init glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Failed to create glfwwindow");
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, window_key_callback);
    glfwSetCursorPosCallback(window, window_mouse_move_callback);
    glfwSetScrollCallback(window, window_scroll_callback);
    glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
}

void FApplication::window_key_callback(GLFWwindow* window, int key, int scankey, int action, int mods) {
    auto* app = static_cast<FApplication*>(glfwGetWindowUserPointer(window));
    app->key_callback(key, scankey, action, mods);
}

void FApplication::window_mouse_move_callback(GLFWwindow* window, double x, double y) {
    auto* app = static_cast<FApplication*>(glfwGetWindowUserPointer(window));
    app->mouse_move_callback(x, y);
}

void FApplication::window_scroll_callback(GLFWwindow* window, double x, double y) {
    auto* app = static_cast<FApplication*>(glfwGetWindowUserPointer(window));
    app->scroll_callback(x, y);
}

void FApplication::window_framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    auto* app = static_cast<FApplication*>(glfwGetWindowUserPointer(window));
    app->framebuffer_size_callback(w, h);
}

int FApplication::width() const {
    int width;
    glfwGetWindowSize(window, &width, nullptr);
    return width;
}

int FApplication::height() const {
    int height;
    glfwGetWindowSize(window, nullptr, &height);
    return height;
}

void FApplication::setWindowTitle(const std::string& title) {
    glfwSetWindowTitle(window, title.c_str());
}

void FApplication::setWindowSize(int width, int height) {
    glfwSetWindowSize(window, width, height);
}

float FApplication::deltaTime() const {
    return _deltaTime;
}

float FApplication::currentTime() const {
    return glfwGetTime();
}

void FApplication::disableImGui(bool flag) {
    _disable_imgui = flag;
}

float FApplication::aspect() const {
    return (float)width() / height();
}

void FApplication::loop() {}
void FApplication::key_callback(int key, int scankey, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void FApplication::mouse_move_callback(double x, double y) {}
void FApplication::scroll_callback(double x, double y) {}
void FApplication::framebuffer_size_callback(int w, int h) {
    glViewport(0, 0, w, h);
}

#include "flib.hpp"
using namespace std;

struct vertex {
    float x, y, z;
};

int main() {
    GLFWwindow* window = initialize_glfw(1200, 1000, "00-test");
    GLenum e = glewInit();
    if (e != GLEW_OK) {
        std::cerr << glewGetErrorString(e) << std::endl;
        throw std::runtime_error("Failed to init glew");
    }

    FShader vertex_shader(fetch("shaders/simple.vert"), GL_VERTEX_SHADER);
    FShader fragment_shader(fetch("shaders/simple.frag"), GL_FRAGMENT_SHADER);
    FProgram program({vertex_shader, fragment_shader});

    vector<vertex> vertices {
        {-1, -1, 0}, { -1, 1, 0 }, { 1, 1, 0 }
    };

    FId vao(GL_VERTEX_ARRAY), vbo(GL_ARRAY_BUFFER);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glEnableVertexAttribArray(0);

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClearColor(0.3, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
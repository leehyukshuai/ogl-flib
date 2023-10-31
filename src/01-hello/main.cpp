#include "flib.hpp"

struct vertex {
    float x, y, z;
};

class App: public FApplication {
    FProgram program;
    FId vao, vbo;
public:
    App() : FApplication(1200, 1000, "01-hello") {
        FShader vertex_shader(fetch("shaders/simple.vert"), GL_VERTEX_SHADER);
        FShader fragment_shader(fetch("shaders/simple.frag"), GL_FRAGMENT_SHADER);
        program.load({ vertex_shader, fragment_shader });

        std::vector<vertex> vertices {
            {-1, -1, 0}, { -1, 1, 0 }, { 1, 1, 0 }
        };

        vao = FId(GL_VERTEX_ARRAY);
        vbo = FId(GL_ARRAY_BUFFER);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
        glEnableVertexAttribArray(0);

    }
protected:
    void loop() override {
        glClearColor(0.5, 0.1, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

int main() {
    try {
        App app;
        app.run();
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
    }
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }

    GLCall(std::cout << glGetString(GL_VERSION) << std::endl);

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray* va = new VertexArray;

    IndexBuffer* ib = new IndexBuffer(indices, 6);
    VertexBuffer* vb = new VertexBuffer(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout* layout = new VertexBufferLayout();

    layout->Push<float>(2);
    va->AddBuffer(*vb, *layout, *ib);

    Shader* shader = new Shader("./res/shaders/Basic.shader");
    shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    Renderer* renderer = new Renderer();

    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        renderer->Clear();

        shader->Bind();
        shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        renderer->Draw(*va, *ib, *shader);

        if (r >= 1.0f)
            increment = -0.05f;
        else if (r <= 0.0f)
            increment = 0.05f;

        r += increment;

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    delete va, layout, vb, ib, shader, renderer;

    glfwTerminate();
    return 0;
}
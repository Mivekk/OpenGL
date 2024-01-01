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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray* va = new VertexArray;

    IndexBuffer* ib = new IndexBuffer(indices, 6);
    VertexBuffer* vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push<float>(2);
    layout->Push<float>(2);
    va->AddBuffer(*vb, *layout, *ib);

    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    Shader* shader = new Shader("./res/shaders/Basic.shader");
    shader->SetUniformMat4f("u_MVP", proj);

    Texture* texture = new Texture("res/textures/ShaggyRogers.png");
    texture->Bind();
    shader->SetUniform1i("u_Texture", 0);

    Renderer* renderer = new Renderer();

    while (!glfwWindowShouldClose(window))
    {
        renderer->Clear();

        renderer->Draw(*va, *shader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    delete va, layout, vb, ib, shader, texture, renderer;

    glfwTerminate();
    return 0;
}
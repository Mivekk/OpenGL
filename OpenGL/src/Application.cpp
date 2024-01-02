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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    Shader* shader = new Shader("./res/shaders/Basic.shader");

    Texture* texture = new Texture("res/textures/ShaggyRogers.png");
    texture->Bind();
    shader->SetUniform1i("u_Texture", 0);

    Renderer* renderer = new Renderer();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glm::vec3 translationA(0.0f, 0.0f, 0.0f);
    glm::vec3 translationB(0.5f, 0.0f, 0.0f);
    while (!glfwWindowShouldClose(window))
    {
        renderer->Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = model * view * proj;
            shader->SetUniformMat4f("u_MVP", mvp);
            renderer->Draw(*va, *shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = model * view * proj;
            shader->SetUniformMat4f("u_MVP", mvp);
            renderer->Draw(*va, *shader);
        }

        {
            ImGui::Begin("ImGui");

            ImGui::SliderFloat3("Translation A", &translationA.x, -1.0f, 1.0f);
            ImGui::SliderFloat3("Translation B", &translationB.x, -1.0f, 1.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }   

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    delete va, layout, vb, ib, shader, texture, renderer;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
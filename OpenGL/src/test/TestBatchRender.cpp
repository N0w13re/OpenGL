#include "TestBatchRender.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    test::TestBatchRender::TestBatchRender()
        : m_Proj(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
    {
        float positions[] = {
            100.0f, 100.0f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
            200.0f, 100.0f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
            200.0f, 200.0f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
            100.0f, 200.0f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

            300.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
            400.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
            400.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
            300.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 11 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(4);  //postion
        layout.Push<float>(4);  //color
        layout.Push<float>(2);  //texture coordinate
        layout.Push<float>(1);  //texture slot
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/Batch.shader");
        m_Shader->Bind();

        m_Texture[0] = std::make_unique<Texture>("res/textures/ChernoLogo.png");
        m_Texture[1] = std::make_unique<Texture>("res/textures/HazelLogo.png");
        for (size_t i = 0; i < 2; i++)
        {
            m_Texture[i]->Bind();
        }
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);
    }

    test::TestBatchRender::~TestBatchRender()
    {
    }

    void test::TestBatchRender::OnUpdate(float deltatime)
    {
    }

    void test::TestBatchRender::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture[0]->Bind(0);
        m_Texture[1]->Bind(1);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
        glm::mat4 mvp = m_Proj * m_View * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void test::TestBatchRender::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}

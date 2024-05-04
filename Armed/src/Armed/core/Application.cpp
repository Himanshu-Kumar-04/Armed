#include "ArmPCH.h"
#include "Application.h"
#include "glad.h"
#include "input.h"

#include "Armed/renderer/OpenGL/openGLBuffer.h"
#include "Armed/renderer/renderer.h"


namespace Arm {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
        :m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->setEventCallBack(BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new ImGuiLayer;
        pushOverlay(m_ImGuiLayer);

        float squareVertices[3 * 4] = {
            -0.5f ,-0.5f, 0.0f,
             0.5f ,-0.5f, 0.0f,
             0.5f , 0.5f, 0.0f,
            -0.5f , 0.5f, 0.0f
        };

        m_SquareVA.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->setLayout({
                {ShaderDataType::Float3 , "position"}
            });
        m_SquareVA->addVertexBuffer(squareVB);
        uint32_t squareIndices[6] = { 0,1,2 ,2 ,3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->setIndexBuffer(squareIB);

        m_Shader.reset(Shader::Create());
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while (m_Running) {

            RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            RendererCommand::clearColor();

            m_Camera.setPosition({ 0.5f,0.0f,0.0f });

            Renderer::beginScene(m_Camera);

            m_Shader->bind();
            Renderer::submit(m_Shader, m_SquareVA);

            Renderer::endScene();

            for (Layer* layer : m_LayerStack)
                layer->onUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->onImGuiRender();
            m_ImGuiLayer->End();

            m_Window->onUpdate();
        }
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->onEvent(e);
            if (e.handled)
                break;
        }

    }

    void Application::pushLayer(Layer* layer)
    {
        m_LayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer)
    {
        m_LayerStack.pushOverlay(layer);
        layer->onAttach();
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
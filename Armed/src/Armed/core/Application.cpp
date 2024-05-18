#include "ArmPCH.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Armed/renderer/renderer.h"

#include "input.h"

namespace Arm {
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;
        m_Window = Scope<Window>(Window::Create());
        m_Window->setEventCallBack(ARM_BIND_EVENT_FN(Application::onEvent));

        Renderer::init();

        m_ImGuiLayer = new ImGuiLayer;
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while (m_Running) {

            float time = (float)glfwGetTime();
            Timestep timestep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            if (!m_Minimized) {
                for (Layer* layer : m_LayerStack)
                layer->onUpdate(timestep);
            }

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
        dispatcher.dispatch<WindowCloseEvent>(ARM_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(ARM_BIND_EVENT_FN(Application::onWindowResize));

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

    void Application::close()
    {
        m_Running = false;
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
    bool Application::onWindowResize(WindowResizeEvent& e)
    {
        if (e.getWidth() == 0 || e.getHeight() == 0) m_Minimized = true;
        else m_Minimized = false;
        Renderer::onWindowResize(e.getWidth(), e.getHeight());
        return false;
    }
}
#include "ArmPCH.h"
#include "Application.h"
#include "glad.h"
#include "input.h"

namespace Arm {
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->setEventCallBack(ARM_BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new ImGuiLayer;
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while (m_Running) {

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
        dispatcher.dispatch<WindowCloseEvent>(ARM_BIND_EVENT_FN(Application::onWindowClose));

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
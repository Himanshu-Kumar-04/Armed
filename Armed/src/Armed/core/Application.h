#pragma once
#include<memory>
#include"appWindow.h"
#include"layerStack.h"
#include"Armed/imGui/imGuiLayer.h"

#include "Armed/core/timestep.h"

namespace Arm {

    class Application {
    public:
        Application(const std::string& name = "Armed App");
        ~Application();

        void run();

        void onEvent(Event& e);
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        void close();

        inline static Application& get() { return *s_Instance; }
        inline Window& getWindow() { return *m_Window; }
        
        ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }
    private:
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);
    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        bool m_Running = true, m_Minimized = false;
        static Application* s_Instance;
        LayerStack m_LayerStack;
        float m_lastFrameTime = 0.0f;
    };

    //To be defined in client
    Application* createApplication();

}
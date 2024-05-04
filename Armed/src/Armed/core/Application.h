#pragma once
#include<memory>
#include"appWindow.h"
#include"layerStack.h"
#include"Armed/imGui/imGuiLayer.h"
#include "Armed/renderer/shader.h"
#include "Armed/renderer/vertexArray.h"
#include "Armed/renderer/orthographicCamera.h"


namespace Arm {

    class Application {
    public:
        Application();
        ~Application();

        void run();

        void onEvent(Event& e);
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);
        inline static Application& get() { return *s_Instance; }
        inline Window& getWindow() { return *m_Window; }

    private:
        bool onWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        bool m_Running = true;
        static Application* s_Instance;
        LayerStack m_LayerStack;

        std::shared_ptr<VertexArray> m_SquareVA;

        std::shared_ptr<Shader> m_Shader;

        OrthographicCamera m_Camera;
    };

    //To be defined in client
    Application* createApplication();

}
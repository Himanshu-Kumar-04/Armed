#pragma once
#include<memory>
#include"appWindow.h"
#include"layerStack.h"

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
		bool m_Running = true;
		static Application* s_Instance;
		LayerStack m_LayerStack;
	};

	//To be defined in client
	Application* createApplication();

}
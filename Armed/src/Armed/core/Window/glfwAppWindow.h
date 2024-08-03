#pragma once
#include"Armed/renderer/graphicsContext.h"
#include "Armed/core/appWindow.h"

#include"Armed/event/applicationEvent.h"
#include"Armed/event/keyEvent.h"
#include"Armed/event/mouseEvent.h"


struct GLFWwindow;

namespace Arm {

	class GLFW_Window : public Window {
	public:
		GLFW_Window(const WindowProperties& props);
		virtual ~GLFW_Window();

		void onUpdate() override;

		unsigned int getWidth() const override { return m_Data.Width; }
		unsigned int getHeight() const override { return m_Data.Height; }

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		virtual void* getNativeWindow() const { return m_Window; }
	private:
		virtual void init(const WindowProperties& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
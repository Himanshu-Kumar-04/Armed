#pragma once
#include<string>
#include<memory>
#include<iostream>

#include"Armed/renderer/graphicsContext.h"


#include"Armed/event/applicationEvent.h"
#include"Armed/event/keyEvent.h"
#include"Armed/event/mouseEvent.h"

struct GLFWwindow;

namespace Arm {
	struct WindowProperties {
		std::string Title;
		uint16_t Width;
		uint16_t Height;

		WindowProperties(const std::string& title = "Armed Engine",
			uint16_t width = 1600,
			uint16_t height = 900)
			:Title(title),Width(width),Height(height)
		{
		}
	};
	class Window {
	public:
		Window(const WindowProperties& properties);
		~Window();

		void onUpdate();
		inline uint16_t getWidth() const { return m_Data.Width; }
		inline uint16_t getHeight() const { return m_Data.Height; }

		using EventCallbackFunction = std::function<void(Event&)>;
		inline void* getNativeWindow() const { return m_Window; }
		inline void setEventCallBack(const EventCallbackFunction& callback) { m_Data.EventCallback = callback; }
		void setVSync(bool enabled);
		bool isVSync() const;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	private:
		void init(const WindowProperties& properties);
		void shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		struct WindowData
		{
			std::string Title = "Title";
			uint16_t Width = 1280;
			uint16_t Height = 720;
			bool VSync = true;

			Window::EventCallbackFunction EventCallback;
		};


		WindowData m_Data;
	};
}
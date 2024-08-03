#pragma once
#include"Armed/event/event.h"
#include"Armed/core/core.h"

struct GLFWwindow;

namespace Arm {
	struct WindowProperties {
		std::string Title;
		uint16_t Width;
		uint16_t Height;

		WindowProperties(const std::string& title = "Armed Engine",
			uint16_t width = 1600,
			uint16_t height = 900)
			:Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Scope<Window> create(const WindowProperties& props = WindowProperties());
	};
}
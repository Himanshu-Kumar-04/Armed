#include"ArmPCH.h"
#include "appWindow.h"

#include"glad.h"
#include<GLFW/glfw3.h>

namespace Arm {

	static bool s_GLFWInitialized = false;

	static void GLFWGetErrorCallback(int error, const char* description) {
		std::cout << "GLFW ERROR : (" << error << ") : " << description << '\n';
	}

	Window::Window(const WindowProperties& properties)
	{
		init(properties);
	}

	Window::~Window()
	{
		shutdown();
	}

	void Window::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		enabled = true;
	}

	bool Window::isVSync() const
	{
		return false;
	}

	Window* Window::Create(const WindowProperties& properties)
	{
		return new Window(properties);
	}

	void Window::init(const WindowProperties& properties)
	{
		m_Data.Title = properties.Title;
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;

		if (!s_GLFWInitialized) {
			if (!glfwInit()) {
				std::cout << "Cannot initialized GLFW\n";
				__debugbreak();
			}
			glfwSetErrorCallback(GLFWGetErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
			std::cout << "failed to initialized glad\n";
		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);

		//set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			});
		
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanCode, int action, int modes) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
			});

	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}

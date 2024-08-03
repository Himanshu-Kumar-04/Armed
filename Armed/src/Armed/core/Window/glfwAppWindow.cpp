#include "ArmPCH.h"
#include "glfwAppWindow.h"
#include<GLFW/glfw3.h>

#include "Armed/renderer/OpenGL/openGLContext.h"
#include "Armed/renderer/vk/vulkanContext.h"
#include"Armed/renderer/renderer.h"

namespace Arm {

    static bool s_GLFWInitialized = false;

    static void GLFWGetErrorCallback(int error, const char* description) {
        std::cout << "GLFW ERROR : (" << error << ") : " << description << '\n';
    }

    GLFW_Window::GLFW_Window(const WindowProperties& properties)
    {
        init(properties);
    }

    GLFW_Window::~GLFW_Window()
    {
        shutdown();
    }

    void GLFW_Window::onUpdate()
    {
        glfwPollEvents();
        m_Context->swapBuffers();
    }

    void GLFW_Window::setVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        enabled = true;
    }

    bool GLFW_Window::isVSync() const
    {
        return false;
    }

    void GLFW_Window::init(const WindowProperties& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!s_GLFWInitialized) {
            if (!glfwInit()) {
                std::cout << "Cannot initialized GLFW\n";
                __debugbreak();
            }
            glfwSetErrorCallback(GLFWGetErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        if (Renderer::getAPI() == RendererAPI::API::OpenGL) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

        m_Context = GraphicsContext::Create(m_Window);
        m_Context->init();

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

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode) {
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
            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
            });
    }

    void GLFW_Window::shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
}
#pragma once
#include "Armed/renderer/graphicsContext.h"
#define GLFW_INCLUDE_VULKAN
#include<vulkan/vulkan.h>
#include<GLFW/glfw3.h>

struct GLFWwindow;

namespace Arm {
    class VulkanContext : public GraphicsContext {
    public:
        VulkanContext(const char* appName);
        virtual void init();
        virtual void swapBuffers();
    private:
        GLFWwindow* m_windowHandle;
        const char* m_AppName;
        VkInstance m_Instance = nullptr;
    };
}
#pragma once
#include"Armed/renderer/graphicsContext.h"

struct GLFWwindow;

namespace Arm {
    class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        virtual void init();
        virtual void swapBuffers();
    private:
        GLFWwindow* m_windowHandle;
    };
}
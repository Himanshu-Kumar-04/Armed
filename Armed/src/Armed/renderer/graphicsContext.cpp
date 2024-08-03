#include "ArmPCH.h"
#include "graphicsContext.h"
#include "renderer.h"
#include "OpenGL/openGLContext.h"

namespace Arm {
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    ARM_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		ARM_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}
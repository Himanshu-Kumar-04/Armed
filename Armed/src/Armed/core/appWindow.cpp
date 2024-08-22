#include"ArmPCH.h"
#include "appWindow.h"
#include "Window/glfwAppWindow.h"
#include "Armed/renderer/renderer.h"
namespace Arm {

	Scope<Window> Window::create(const WindowProperties& props)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:	return nullptr;
		case RendererAPI::API::OpenGL:	return CreateScope<GLFW_Window>(props);
		case RendererAPI::API::Vulkan:  return CreateScope<GLFW_Window>(props);
		default:
			return nullptr;
		}
	}

}

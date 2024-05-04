#include "ArmPCH.h"
#include "vertexArray.h"
#include "renderer.h"
#include "OpenGL/openGLVertexArray.h"

namespace Arm {
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLVertexArray();
        }

        ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
}
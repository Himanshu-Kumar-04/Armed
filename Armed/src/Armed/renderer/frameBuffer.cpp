#include "ArmPCH.h"
#include "frameBuffer.h"
#include "renderer.h"
#include "OpenGL/openGLFrameBuffer.h"

namespace Arm {
    Ref<FrameBuffer> FrameBuffer::create(const FrameBufferProperties& properties)
    {
        ARM_PROFILE_FUNCTION();

        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLFrameBuffer>(properties);
        }

        std::cout << "UNKNOWN RENDERER API\n";
        return nullptr;
    }
}
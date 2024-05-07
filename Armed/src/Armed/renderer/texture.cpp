#include "ArmPCH.h"
#include "Armed/renderer/renderer.h"
#include "texture.h"
#include "OpenGL/openGLTexture.h"

namespace Arm {
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(path);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
}
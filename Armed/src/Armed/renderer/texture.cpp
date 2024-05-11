#include "ArmPCH.h"
#include "Armed/renderer/renderer.h"
#include "texture.h"
#include "OpenGL/openGLTexture.h"

namespace Arm {
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(width,height);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(path);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
}
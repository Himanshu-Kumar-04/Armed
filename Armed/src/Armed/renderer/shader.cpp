#include"ArmPCH.h"
#include "shader.h"
#include "renderer.h"
#include "OpenGL/openGLShader.h"

namespace Arm {
    Shader* Shader::Create(const std::string& filepath)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLShader(filepath);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
    Shader* Shader::Create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return new OpenGLShader(vertexShaderSource, fragmentShaderSource);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
}
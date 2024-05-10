#include"ArmPCH.h"
#include "shader.h"
#include "renderer.h"
#include "OpenGL/openGLShader.h"

namespace Arm {
    Ref<Shader> Shader::create(const std::string& filepath)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLShader>(filepath);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
    Ref<Shader> Shader::create(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
    {
        switch (Renderer::getAPI()) {
        case RendererAPI::API::None:     std::cout << "RendererAPI: None is not supported.\n"; return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLShader>(name, vertexShaderSource, fragmentShaderSource);
        }

        ARM_ASSERT(false, "Unknown Renderer API");
        return nullptr;
    }
    

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
	{
		ARM_ASSERT(!exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name)
	{
		ARM_ASSERT(exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
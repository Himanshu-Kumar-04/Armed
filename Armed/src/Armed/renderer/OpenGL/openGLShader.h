#pragma once
#include"Armed/renderer/shader.h"
#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Arm {

	class  OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;
		//Set Uniform
		void setUniform1i(const std::string& name, int32_t v1);

		void setUniform1f(const std::string& name, float vec);
		void setUniform2f(const std::string& name, glm::vec2& vec);
		void setUniform3f(const std::string& name, glm::vec3& vec);
		void setUniform4f(const std::string& name, glm::vec4& vec);

		void setUniformMat3(const std::string name, const glm::mat3& matrix);
		void setUniformMat4(const std::string name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int32_t> m_UniformLocationCache;
		const std::string m_FilePath;
		
		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
		void  compileShader(const std::unordered_map<GLenum,std::string>& shaderSource);
		int32_t getUniformLocation(const std::string& name);
	};
}
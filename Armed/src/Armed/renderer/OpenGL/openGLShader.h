#pragma once
#include "Armed/renderer/shader.h"
#include "Armed/core/core.h"
#include <glm/glm.hpp>


typedef unsigned int GLenum;

namespace Arm {

	class  OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;
		virtual const std::string getName() const { return m_Name; }
		//Set Uniform
		virtual void setInt(const std::string& name, int value) override;
		virtual void setIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void setFloat(const std::string& name, float value) override;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;
		//Upload Uniform
		void uploadUniformInt1(const std::string& name, int32_t v1);
		void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void uploadUniformFloat1(const std::string& name, float vec);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& vec);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& vec);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& vec);

		void uploadUniformMat3(const std::string name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string name, const glm::mat4& matrix);

	private:
		RendererID m_RendererID;
		std::string m_Name;
		std::unordered_map<std::string, int32_t> m_UniformLocationCache;
		const std::string m_FilePath;
		
		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
		void  compileShader(const std::unordered_map<GLenum,std::string>& shaderSource);
		int32_t getUniformLocation(const std::string& name);
	};
}
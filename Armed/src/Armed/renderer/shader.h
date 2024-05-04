#pragma once
#include"ArmPCH.h"

#include <glm/glm.hpp>
namespace Arm {

	class Shader {
	public:
		Shader();
		~Shader();

		static Shader* Create();

		void bind();
		void unbind();
		//Set Uniform
		void setUniform1i(const std::string& name, int32_t v1);
		void setUniform1f(const std::string& name, float v1);
		void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
		void setUniformMat4(const std::string name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int32_t> m_UniformLocationCache;

		std::string readShader(const std::string& filePath);
		uint32_t  compileShader(uint32_t type, const std::string& source);
		uint32_t createShader(const std::string& vertexShader, const std::string& fragmentShader);
		int32_t getUniformLocation(const std::string& name);
	};
}
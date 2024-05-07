#pragma once
#include"ArmPCH.h"

namespace Arm {

	class Shader {
	public:
		virtual ~Shader() = default;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);


		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}
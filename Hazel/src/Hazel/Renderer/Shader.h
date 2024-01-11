#pragma once

#include <string>

namespace Hazel {
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() {}
		virtual void UnBind() {}

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}
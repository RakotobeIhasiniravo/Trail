#pragma once
#include <string>
#include <glm/glm.hpp>
#include <fstream>

namespace Trail {

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;
		void SetUniformMat4f(const std::string& name, const glm::mat4& projection_matrix);
		void SetUniformFloat4(const std::string& name, const glm::vec4& color); 
		void SetUniformFloat2(const std::string& name, const glm::vec2& value);
		void SetUniformFloat3(const std::string& name, const glm::vec3& value);
		void SetUniformInt(const std::string& name, const int slot);
	private: 
		ShaderProgramSource PreProcess(const std::string& filepath);
		void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
	private: 
		uint32_t m_RendererID;
	};
}
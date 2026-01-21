#include "trlpch.h"
#include "Shader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

//CODE I GOT FROM https://www.khronos.org/opengl/wiki/Shader_Compilation 26/03/2025  2:00 
//This page was last edited on 11 October 2022, at 03:35.


namespace Trail {
	static int getTypeCode(const std::string& type) {
		if (type == "vertex")
			return 0;
		else if (type == "fragment" || type == "pixel")
			return 1;
		else
			TRL_CORE_ERROR("Invalid shader type {0}", type);
		return -1;
	}

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Shader::Compile(vertexSrc, fragmentSrc);
	}
	Shader::Shader(const std::string& filepath)
	{
		ShaderProgramSource programSource = Shader::PreProcess(filepath);
		Shader::Compile(programSource.VertexSource, programSource.FragmentSource);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}
	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& m_ProjectionView)
	{
		GLint  location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE,glm::value_ptr(m_ProjectionView));
	}
	
	void Shader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint  location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint  location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	void Shader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint  location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void Shader::SetUniformInt(const std::string& name, const int value)
	{
		GLint  location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	ShaderProgramSource Shader::PreProcess(const std::string& filepath)
	{
		std::string shaderSource;
		std::ifstream stream(filepath);
		if (!stream) 
			TRL_CORE_ERROR("Could not open shader file {0} ", filepath);
		
		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};
		std::string line;
		std::stringstream ss[2];
		ShaderType shadertype = ShaderType::NONE;
		while (getline(stream, line)) {
			if (line.find("#type") != std::string::npos) {//if we find "#type" in a line
				if (line.find("vertex") != std::string::npos) {
					//set the mode to vertex shader
					shadertype = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					//set the mode to fragment shader
					shadertype = ShaderType::FRAGMENT;
				}
			}//if we don't find the shader type line, we put the line in the corresponding stringstream
			else {
				ss[(int)shadertype] << line << '\n';
			}
		}
		return { ss[0].str(), ss[1].str() };
	}
	void Shader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			TRL_CORE_ERROR("{0}", infoLog.data());
			TRL_CORE_ASSERT(false, "Compilation failure of Vertex shader");

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			TRL_CORE_ERROR("{0}", infoLog.data());
			TRL_CORE_ASSERT(false, "Compilation failure of fragment shader");

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.

		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}
}
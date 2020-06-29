#include "Shader.h"
#include "Engine.h"

#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace Tristeon
{
	Shader::Shader()
	{
		empty = true;
	}

	Shader::Shader(std::string const& vertexShader, std::string const& fragmentShader) : vertexPath(vertexShader), fragmentPath(fragmentShader)
	{
		initialize();
		Renderer::registerPrePassShader(this);
	}

	Shader::~Shader()
	{
		if (!empty)
			Renderer::deregisterPrePassShader(this);

		glDeleteProgram(program);
	}

	bool Shader::isReady() const
	{
		return !failed && ready;
	}

	void Shader::bind() const
	{
		glUseProgram(program);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	bool Shader::isEmpty() const
	{
		return empty;
	}

	void Shader::reload()
	{
		glDeleteProgram(program);
		
		ready = false;
		failed = false;
		initialize();
	}

	void Shader::setUniformValue(std::string const& name, int const& v0)
	{
		bind();
		glUniform1i(glGetUniformLocation(program, name.data()), v0);
	}

	void Shader::setUniformValue(std::string const& name, int const& v0, int const& v1)
	{
		bind();
		glUniform2i(glGetUniformLocation(program, name.data()), v0, v1);
	}

	void Shader::setUniformValue(std::string const& name, int const& v0, int const& v1, int const& v2)
	{
		bind();
		glUniform3i(glGetUniformLocation(program, name.data()), v0, v1, v2);
	}

	void Shader::setUniformValue(std::string const& name, int const& v0, int const& v1, int const& v2, int const& v3)
	{
		bind();
		glUniform4i(glGetUniformLocation(program, name.data()), v0, v1, v2, v3);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0)
	{
		bind();
		glUniform1ui(glGetUniformLocation(program, name.data()), v0);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0, unsigned const& v1)
	{
		bind();
		glUniform2ui(glGetUniformLocation(program, name.data()), v0, v1);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0, unsigned const& v1, unsigned const& v2)
	{
		bind();
		glUniform3ui(glGetUniformLocation(program, name.data()), v0, v1, v2);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0, unsigned const& v1, unsigned const& v2, unsigned const& v3)
	{
		bind();
		glUniform4ui(glGetUniformLocation(program, name.data()), v0, v1, v2, v3);
	}

	void Shader::setUniformValue(std::string name, float v0)
	{
		bind();
		glUniform1f(glGetUniformLocation(program, name.data()), v0);
	}

	void Shader::setUniformValue(std::string name, float v0, float v1)
	{
		bind();
		glUniform2f(glGetUniformLocation(program, name.data()), v0, v1);
	}

	void Shader::setUniformValue(std::string name, float v0, float v1, float v2)
	{
		bind();
		glUniform3f(glGetUniformLocation(program, name.data()), v0, v1, v2);
	}

	void Shader::setUniformValue(std::string name, float v0, float v1, float v2, float v3)
	{
		bind();
		glUniform4f(glGetUniformLocation(program, name.data()), v0, v1, v2, v3);
	}

	void Shader::initialize()
	{
		std::ifstream vertexFile(vertexPath.c_str());
		std::ifstream fragmentFile(fragmentPath.c_str());

		if (!vertexFile.is_open() || !fragmentFile.is_open())
		{
			std::cout << "Failed to create shader program with files: [Vertex] " + vertexPath + ", [Fragment] " +
				fragmentPath << std::endl;
			failed = true;
			return;
		}

		vertexData = std::string(std::istreambuf_iterator<char>(vertexFile), std::istreambuf_iterator<char>());
		fragmentData = std::string(std::istreambuf_iterator<char>(fragmentFile), std::istreambuf_iterator<char>());

		//Compile and check vertex shader
		auto vertexString = vertexData.data();
		const unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexString, nullptr);
		glCompileShader(vertex);

#ifdef TRISTEON_LOGENABLED
		int compiledVertex = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiledVertex);
		if (compiledVertex == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(vertex, maxLength, &maxLength, &errorLog[0]);
				std::cout << "Failed to compile vertex shader " << vertexPath << ": \n" << errorLog.data() << "\n";
				glDeleteShader(vertex);
			}
			else
				std::cout << "Failed to compile vertex shader " << vertexPath << ". No log.\n";
			return;
		}
#endif
		
		//Compile and check fragment shader
		auto fragmentString = fragmentData.data();
		const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentString, nullptr);
		glCompileShader(fragment);

#ifdef TRISTEON_LOGENABLED
		GLint compiledFragment = 0;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiledFragment);
		if (compiledFragment == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);
				std::cout << "Failed to compile fragment shader " << fragmentPath << ": \n" << errorLog.data() << "\n";
				glDeleteShader(fragment);
			}
			else
				std::cout << "Failed to compile fragment shader " << fragmentPath << ". No log.\n";
			return;
		}
#endif
		
		//Attach and link
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		std::cout << "Created shader program " << program << " with vertexpath " << vertexPath << " and fragmentpath "
			<< fragmentPath << std::endl;
		ready = true;

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
}

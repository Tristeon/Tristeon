#include "Shader.h"
#include "Engine.h"

#include "Utils/Console.h"
#include <fstream>
#include <glad/glad.h>

namespace Tristeon
{
	Shader::Shader()
	{
		_empty = true;
		Collector<Shader>::add(this);
	}

	Shader::Shader(const String& vertexShader, const String& fragmentShader) : _vertexPath(vertexShader), _fragmentPath(fragmentShader)
	{
		load();
		Collector<Shader>::add(this);
	}

	Shader::~Shader()
	{
		Collector<Shader>::remove(this);

		glDeleteProgram(_program);
	}

	bool Shader::ready() const
	{
		return !_failed && _ready;
	}

	void Shader::bind() const
	{
		glUseProgram(_program);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	bool Shader::empty() const
	{
		return _empty;
	}

	void Shader::reload()
	{
		glDeleteProgram(_program);

		_ready = false;
		_failed = false;
		load();
	}

	void Shader::setUniformValue(const String& name, const int& v0)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform1i(loc, v0);
	}

	void Shader::setUniformValue(const String& name, const int& v0, const int& v1)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);
		
		glUniform2i(loc, v0, v1);
	}

	void Shader::setUniformValue(const String& name, const int& v0, const int& v1, const int& v2)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);
		
		glUniform3i(loc, v0, v1, v2);
	}

	void Shader::setUniformValue(const String& name, const int& v0, const int& v1, const int& v2, const int& v3)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform4i(loc, v0, v1, v2, v3);
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform1ui(loc, v0);
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform2ui(loc, v0, v1);
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1, const unsigned int& v2)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform3ui(loc, v0, v1, v2);
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1, const unsigned int& v2, const unsigned int& v3)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform4ui(loc, v0, v1, v2, v3);
	}

	void Shader::setUniformValue(const String& name, const float& v0)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform1f(loc, v0);
	}

	void Shader::setUniformValue(const String& name, const float& v0, const float& v1)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform2f(loc, v0, v1);
	}

	void Shader::setUniformValue(const String& name, const float& v0, const float& v1, const float& v2)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform3f(loc, v0, v1, v2);
	}

	void Shader::setUniformValue(const String& name, const float& v0, const float& v1, const float& v2, const float& v3)
	{
		const auto loc = glGetUniformLocation(_program, name.data());
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform4f(loc, v0, v1, v2, v3);
	}

	void Shader::load()
	{
		std::ifstream vertexFile(_vertexPath.c_str());
		std::ifstream fragmentFile(_fragmentPath.c_str());

		if (!vertexFile.is_open() || !fragmentFile.is_open())
		{
			TRISTEON_WARNING("Failed to create shader program with files: [Vertex] " + _vertexPath + ", [Fragment] " + _fragmentPath);
			_failed = true;
			return;
		}

		_vertexData = String(std::istreambuf_iterator<char>(vertexFile), std::istreambuf_iterator<char>());
		_fragmentData = String(std::istreambuf_iterator<char>(fragmentFile), std::istreambuf_iterator<char>());

		//Compile and check vertex shader
		auto vertexString = _vertexData.data();
		const auto vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexString, nullptr);
		glCompileShader(vertex);

#ifdef TRISTEON_LOGENABLED
		int compiledVertex = 0;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiledVertex);
		if (compiledVertex == GL_FALSE)
		{
			auto maxLength = 0;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				List<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(vertex, maxLength, &maxLength, &errorLog[0]);
				TRISTEON_WARNING("Failed to compile vertex shader " + _vertexPath + ": " + std::string(errorLog.ptr()));
				glDeleteShader(vertex);
			}
			else
			{
				TRISTEON_WARNING("Failed to compile vertex shader " + _vertexPath + ". No log.");
			}
			return;
		}
#endif

		//Compile and check fragment shader
		auto fragmentString = _fragmentData.data();
		const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentString, nullptr);
		glCompileShader(fragment);

#ifdef TRISTEON_LOGENABLED
		GLint compiledFragment = 0;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiledFragment);
		if (compiledFragment == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				List<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);
				TRISTEON_WARNING("Failed to compile fragment shader " + _fragmentPath + ": " + std::string(errorLog.ptr()));
				glDeleteShader(fragment);
			}
			else
			{
				TRISTEON_WARNING("Failed to compile fragment shader " + _fragmentPath + ". No log.");
			}
			return;
		}
#endif

		//Attach and link
		_program = glCreateProgram();
		glAttachShader(_program, vertex);
		glAttachShader(_program, fragment);
		
		glLinkProgram(_program);

		TRISTEON_LOG("Successfully created shader program " + std::to_string(_program) + " with vertexpath " + _vertexPath + " and fragmentpath " + _fragmentPath);
		_ready = true;

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
}
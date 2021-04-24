#include "Shader.h"

#include <filesystem>
#include <glad/glad.h>
#include <Shadinclude.hpp>

#include "DebugGL.h"
#include "Engine.h"
#include "Utils/Console.h"

namespace Tristeon
{
	Shader::Shader()
	{
		_empty = true;
		Collector<Shader>::add(this);
	}

	Shader::Shader(const String& pVertexShader, const String& pFragmentShader) : _vertexPath(pVertexShader), _fragmentPath(pFragmentShader)
	{
		load();
		Collector<Shader>::add(this);
	}

	Shader::~Shader()
	{
		Collector<Shader>::remove(this);

		glDeleteProgram(_program);
		TRISTEON_DEBUG_GL();
	}

	bool Shader::ready() const
	{
		return !_failed && _ready;
	}

	void Shader::bind() const
	{
		TRISTEON_DEBUG_GL();
		glUseProgram(_program);
		TRISTEON_DEBUG_GL();
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
		TRISTEON_DEBUG_GL();
	}

	bool Shader::empty() const
	{
		return _empty;
	}

	void Shader::reload()
	{
		glDeleteProgram(_program);
		TRISTEON_DEBUG_GL();

		_ready = false;
		_failed = false;
		load();
	}

	void Shader::setUniformValue(const String& name, const int& v0)
	{
		const auto loc = uniformIndex(name);
		if (loc == -1)
			return;
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform1i(loc, v0);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const int& v0, const int& v1)
	{
		const auto loc = uniformIndex(name);
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);
		
		glUniform2i(loc, v0, v1);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const int& v0, const int& v1, const int& v2)
	{
		const auto loc = uniformIndex(name);
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);
		
		glUniform3i(loc, v0, v1, v2);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const int& v0, const int& v1, const int& v2, const int& v3)
	{
		const auto loc = uniformIndex(name);
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform4i(loc, v0, v1, v2, v3);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0)
	{
		const auto loc = uniformIndex(name);
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform1ui(loc, v0);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1)
	{
		const auto loc = uniformIndex(name);
		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform2ui(loc, v0, v1);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1, const unsigned int& v2)
	{
		const auto loc = uniformIndex(name);

		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform3ui(loc, v0, v1, v2);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1, const unsigned int& v2, const unsigned int& v3)
	{
		const auto loc = uniformIndex(name);

		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform4ui(loc, v0, v1, v2, v3);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const float& v0)
	{
		const auto loc = uniformIndex(name);

		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform1f(loc, v0);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const float& v0, const float& v1)
	{
		const auto loc = uniformIndex(name);

		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform2f(loc, v0, v1);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const float& v0, const float& v1, const float& v2)
	{
		const auto loc = uniformIndex(name);

		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform3f(loc, v0, v1, v2);
		TRISTEON_DEBUG_GL();
	}

	void Shader::setUniformValue(const String& name, const float& v0, const float& v1, const float& v2, const float& v3)
	{
		const auto loc = uniformIndex(name);

		//TRISTEON_ASSERT(loc != -1, "Couldn't find shader uniform with name " + name + ". Shader: " + _vertexPath + ", " + _fragmentPath, AssertSeverity::Warning);

		glUniform4f(loc, v0, v1, v2, v3);
		TRISTEON_DEBUG_GL();
	}

	void Shader::load()
	{
		if (!std::filesystem::exists(_vertexPath))
		{
			TRISTEON_WARNING("Failed to create shader program because vertex file " + _vertexPath + " doesn't exist");
			_failed = true;
			return;
		}
		if (!std::filesystem::exists(_vertexPath))
		{
			TRISTEON_WARNING("Failed to create shader program because fragment file " + _fragmentPath + " doesn't exist");
			_failed = true;
			return;
		}

		_vertexData = Shadinclude::load(_vertexPath);
		_fragmentData = Shadinclude::load(_fragmentPath);

		//Compile and check vertex shader
		auto* vertexString = _vertexData.data();
		const auto vertex = glCreateShader(GL_VERTEX_SHADER);
		TRISTEON_DEBUG_GL();
		glShaderSource(vertex, 1, &vertexString, nullptr);
		TRISTEON_DEBUG_GL();
		glCompileShader(vertex);
		TRISTEON_DEBUG_GL();

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
		auto* fragmentString = _fragmentData.data();
		const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentString, nullptr);
		TRISTEON_DEBUG_GL();
		glCompileShader(fragment);
		TRISTEON_DEBUG_GL();

#ifdef TRISTEON_LOGENABLED
		auto compiledFragment = 0;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiledFragment);
		if (compiledFragment == GL_FALSE)
		{
			auto maxLength = 0;
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
		TRISTEON_DEBUG_GL();
		glAttachShader(_program, vertex);
		TRISTEON_DEBUG_GL();
		glAttachShader(_program, fragment);
		TRISTEON_DEBUG_GL();

		glLinkProgram(_program);
		TRISTEON_DEBUG_GL();

		TRISTEON_LOG("Successfully created shader program " + std::to_string(_program) + " with vertexpath " + _vertexPath + " and fragmentpath " + _fragmentPath);
		_ready = true;

		glDeleteShader(vertex);
		TRISTEON_DEBUG_GL();
		glDeleteShader(fragment);
		TRISTEON_DEBUG_GL();
	}

	int32_t Shader::uniformIndex(const String& pPath)
	{
		if (_uniformCache.find(pPath) != _uniformCache.end())
			return _uniformCache[pPath];

		const auto loc = glGetUniformLocation(_program, pPath.c_str());
		_uniformCache[pPath] = loc;
		return loc;
	}
}

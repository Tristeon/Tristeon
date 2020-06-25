#include "Shader.h"
#include "Engine.h"

#include <fstream>
#include <iostream>

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

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

		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glDeleteProgram(program);
	}

	bool Shader::isReady() const
	{
		return !failed && ready;
	}

	void Shader::bind() const
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glUseProgram(program);
	}

	void Shader::unbind() const
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glUseProgram(0);
	}

	bool Shader::isEmpty() const
	{
		return empty;
	}

	void Shader::reload()
	{
		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
		f->glDeleteProgram(program);
		
		ready = false;
		failed = false;
		initialize();
	}

	void Shader::setUniformValue(std::string const& name, int const& v0)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform1i(f->glGetUniformLocation(program, name.data()), v0);
	}

	void Shader::setUniformValue(std::string const& name, int const& v0, int const& v1)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform2i(f->glGetUniformLocation(program, name.data()), v0, v1);
	}

	void Shader::setUniformValue(std::string const& name, int const& v0, int const& v1, int const& v2)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform3i(f->glGetUniformLocation(program, name.data()), v0, v1, v2);
	}

	void Shader::setUniformValue(std::string const& name, int const& v0, int const& v1, int const& v2, int const& v3)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform4i(f->glGetUniformLocation(program, name.data()), v0, v1, v2, v3);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0)
	{
		auto* f = GameView::context()->functions();
		bind();
		GameView::context()->extraFunctions()->glUniform1ui(f->glGetUniformLocation(program, name.data()), v0);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0, unsigned const& v1)
	{
		auto* f = GameView::context()->functions();
		bind();
		GameView::context()->extraFunctions()->glUniform2ui(f->glGetUniformLocation(program, name.data()), v0, v1);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0, unsigned const& v1, unsigned const& v2)
	{
		auto* f = GameView::context()->functions();
		bind();
		GameView::context()->extraFunctions()->glUniform3ui(f->glGetUniformLocation(program, name.data()), v0, v1, v2);
	}

	void Shader::setUniformValue(std::string const& name, unsigned const& v0, unsigned const& v1, unsigned const& v2, unsigned const& v3)
	{
		auto* f = GameView::context()->functions();
		bind();
		GameView::context()->extraFunctions()->glUniform4ui(f->glGetUniformLocation(program, name.data()), v0, v1, v2, v3);
	}

	void Shader::setUniformValue(std::string name, float v0)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform1f(f->glGetUniformLocation(program, name.data()), v0);
	}

	void Shader::setUniformValue(std::string name, float v0, float v1)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform2f(f->glGetUniformLocation(program, name.data()), v0, v1);
	}

	void Shader::setUniformValue(std::string name, float v0, float v1, float v2)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform3f(f->glGetUniformLocation(program, name.data()), v0, v1, v2);
	}

	void Shader::setUniformValue(std::string name, float v0, float v1, float v2, float v3)
	{
		auto* f = GameView::context()->functions();
		bind();
		f->glUniform4f(f->glGetUniformLocation(program, name.data()), v0, v1, v2, v3);
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

		QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

		//Compile and check vertex shader
		auto vertexString = vertexData.data();
		const unsigned int vertex = f->glCreateShader(GL_VERTEX_SHADER);
		f->glShaderSource(vertex, 1, &vertexString, nullptr);
		f->glCompileShader(vertex);

#ifdef TRISTEON_LOGENABLED
		GLint compiledVertex = 0;
		f->glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiledVertex);
		if (compiledVertex == GL_FALSE)
		{
			GLint maxLength = 0;
			f->glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				std::vector<GLchar> errorLog(maxLength);
				f->glGetShaderInfoLog(vertex, maxLength, &maxLength, &errorLog[0]);
				std::cout << "Failed to compile vertex shader " << vertexPath << ": \n" << errorLog.data() << "\n";
				f->glDeleteShader(vertex);
			}
			else
				std::cout << "Failed to compile vertex shader " << vertexPath << ". No log.\n";
			return;
		}
#endif
		
		//Compile and check fragment shader
		auto fragmentString = fragmentData.data();
		const unsigned int fragment = f->glCreateShader(GL_FRAGMENT_SHADER);
		f->glShaderSource(fragment, 1, &fragmentString, nullptr);
		f->glCompileShader(fragment);

#ifdef TRISTEON_LOGENABLED
		GLint compiledFragment = 0;
		f->glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiledFragment);
		if (compiledFragment == GL_FALSE)
		{
			GLint maxLength = 0;
			f->glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				std::vector<GLchar> errorLog(maxLength);
				f->glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);
				std::cout << "Failed to compile fragment shader " << fragmentPath << ": \n" << errorLog.data() << "\n";
				f->glDeleteShader(fragment);
			}
			else
				std::cout << "Failed to compile fragment shader " << fragmentPath << ". No log.\n";
			return;
		}
#endif
		
		//Attach and link
		program = f->glCreateProgram();
		f->glAttachShader(program, vertex);
		f->glAttachShader(program, fragment);
		f->glLinkProgram(program);

		std::cout << "Created shader program " << program << " with vertexpath " << vertexPath << " and fragmentpath "
			<< fragmentPath << std::endl;
		ready = true;
	}
}

#include "Shader.h"
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include "Engine.h"
#include "Window.h"

namespace Tristeon
{
	Shader::Shader()
	{
		//Empty
	}

	Shader::Shader(std::string const& vertexShader, std::string const& fragmentShader) : vertexPath(vertexShader), fragmentPath(fragmentShader)
	{
		initialize();
	}

	Shader::~Shader()
	{
		delete program;
	}

	bool Shader::isReady()
	{
		return !failed && ready;
	}

	void Shader::bind() 
	{
		program->bind();
	}

	void Shader::reload()
	{
		delete program;
		ready = false;
		failed = false;
		initialize();
	}

	void Shader::initialize()
	{
		QFile vertexFile(vertexPath.c_str());
		vertexFile.open(QIODevice::OpenModeFlag::ReadOnly);
		QFile fragmentFile(fragmentPath.c_str());
		fragmentFile.open(QIODevice::OpenModeFlag::ReadOnly);

		if (!vertexFile.isOpen() || !fragmentFile.isOpen())
		{
			std::cout << "Failed to create shader program with files: [Vertex] " + vertexPath + ", [Fragment] " + fragmentPath << std::endl;
			failed = true;
			return;
		}

		vertexData = vertexFile.readAll().toStdString();
		fragmentData = fragmentFile.readAll().toStdString();

		program = new QOpenGLShaderProgram(Engine::instance()->gameView()->context());
		program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexData.c_str());
		program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentData.c_str());
		program->link();
		std::cout << "Created shader program " << program << " with vertexpath " << vertexPath << " and fragmentpath " << fragmentPath << std::endl;
		ready = true;
	}
}
#include "Shader.h"
#include <iostream>
#include <QFileSystemModel>
#include <QOpenGLContext>

namespace Tristeon
{
	Shader::Shader()
	{
		//Empty
	}

	Shader::Shader(std::string const& vertexShader, std::string const& fragmentShader) : vertexPath(vertexShader), fragmentPath(fragmentShader)
	{

	}

	Shader::~Shader()
	{
		delete program;
	}

	bool Shader::isReady()
	{
		if (!failed && !ready)
			initialize();

		return !failed && ready;
	}

	void Shader::bind() 
	{
		program->bind();
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

		program = new QOpenGLShaderProgram(QOpenGLContext::currentContext());
		program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexData.c_str());
		program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentData.c_str());
		program->link();

		ready = true;
	}
}

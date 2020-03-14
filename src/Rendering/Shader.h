#pragma once
#include <string>

class QOpenGLShaderProgram;

namespace Tristeon
{
	class Shader
	{
	public:
		explicit Shader();
		explicit Shader(std::string const& vertexShader, std::string const& fragmentShader);
		~Shader();

		bool isReady();
		void bind();

		bool isEmpty();

		QOpenGLShaderProgram* getShaderProgram() const { return program; }
		void reload();
	private:
		void initialize();
		
		std::string vertexPath = "";
		std::string fragmentPath = "";
		
		std::string vertexData = "";
		std::string fragmentData = "";

		bool ready = false;
		bool failed = false;
		bool empty = false;

		QOpenGLShaderProgram* program = nullptr;
	};
}
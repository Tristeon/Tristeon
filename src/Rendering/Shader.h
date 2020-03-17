#pragma once
#include <string>

class QOpenGLShaderProgram;

namespace Tristeon
{
	/**
	 * Shader is a wrapper around QOpenGLShaderProgram that simplifies its implementation.
	 *
	 * Graphic objects can use this class to create shader programs with vertex & fragment shaders.
	 */
	class Shader
	{
	public:
		/**
		 * Creates an empty Shader.
		 *
		 * Using this constructor, the shader is NOT ready for rendering.
		 * isEmpty() is true if this constructor is used.
		 */
		explicit Shader();

		/**
		 * Creates a shader program using the given vertex and fragment shader paths.
		 *
		 * If file loading & shader compilation succeeds, isRead() will be true.
		 */
		explicit Shader(std::string const& vertexShader, std::string const& fragmentShader);
		~Shader();

		/**
		 * True if the shaders have compiled successfully.
		 * False if the shaders have failed to compile/load, or if the program is empty (isEmpty())
		 */
		bool isReady() const;

		/**
		 * True if the class was created without any shader files.
		 * False if the class was created with shader files, even if the shaders failed to compile.
		 */
		bool isEmpty() const;

		/**
		 * Binds the shader program for rendering usage.
		 */
		void bind() const;

		/**
		 * Gets the QOpenGLShaderProgram for further rendering use.
		 */
		QOpenGLShaderProgram* getShaderProgram() const { return program; }

		/**
		 * Reloads the shader using the same vertex and fragment paths as before.
		 */
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
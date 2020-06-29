#pragma once
#include <string>
#include <TObject.h>

namespace Tristeon
{
	/**
	 * Shaders are what define the visual appearance of an object in the scene.
	 *
	 * Graphic objects can use this class to create shader programs with vertex & fragment shaders.
	 */
	class Shader : public TObject
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
		 * Unbinds the shader program.
		 */
		void unbind() const;
		
		/**
		 * Gets the shader program's ID for advanced rendering use.
		 */
		unsigned int getShaderProgram() const { return program; }

		/**
		 * Reloads the shader using the same vertex and fragment paths as before.
		 */
		void reload();

		void setUniformValue(std::string const& name, int const& v0);
		void setUniformValue(std::string const& name, int const& v0, int const& v1);
		void setUniformValue(std::string const& name, int const& v0, int const& v1, int const& v2);
		void setUniformValue(std::string const& name, int const& v0, int const& v1, int const& v2, int const& v3);

		void setUniformValue(std::string const& name, unsigned int const& v0);
		void setUniformValue(std::string const& name, unsigned int const& v0, unsigned int const& v1);
		void setUniformValue(std::string const& name, unsigned int const& v0, unsigned int const& v1, unsigned int const& v2);
		void setUniformValue(std::string const& name, unsigned int const& v0, unsigned int const& v1, unsigned int const& v2, unsigned int const& v3);

		void setUniformValue(std::string name, float v0);
		void setUniformValue(std::string name, float v0, float v1);
		void setUniformValue(std::string name, float v0, float v1, float v2);
		void setUniformValue(std::string name, float v0, float v1, float v2, float v3);

	private:
		void initialize();
		
		std::string vertexPath = "";
		std::string fragmentPath = "";
		
		std::string vertexData = "";
		std::string fragmentData = "";

		bool ready = false;
		bool failed = false;
		bool empty = false;

		unsigned int program = 0;
	};
}

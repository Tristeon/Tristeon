#pragma once
#include <TObject.h>
#include <Standard/String.h>
#include <Utils/ClassDefaults.h>

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
		explicit Shader(const String& vertexShader, const String& fragmentShader);
		~Shader();

		DELETE_COPY(Shader);
		DEFAULT_MOVE(Shader);

		/**
		 * True if the shaders have compiled successfully.
		 * False if the shaders have failed to compile/load, or if the program is empty (isEmpty())
		 */
		[[nodiscard]] bool ready() const;

		/**
		 * True if the class was created without any shader files.
		 * False if the class was created with shader files, even if the shaders failed to compile.
		 */
		[[nodiscard]] bool empty() const;

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
		[[nodiscard]] unsigned int shaderProgram() const { return _program; }

		/**
		 * Reloads the shader using the same vertex and fragment paths as before.
		 */
		void reload();

		void setUniformValue(const String& name, const int& v0);
		void setUniformValue(const String& name, const int& v0, const int& v1);
		void setUniformValue(const String& name, const int& v0, const int& v1, const int& v2);
		void setUniformValue(const String& name, const int& v0, const int& v1, const int& v2, const int& v3);

		void setUniformValue(const String& name, const unsigned int& v0);
		void setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1);
		void setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1, const unsigned int& v2);
		void setUniformValue(const String& name, const unsigned int& v0, const unsigned int& v1, const unsigned int& v2, const unsigned int& v3);

		void setUniformValue(const String& name, const float& v0);
		void setUniformValue(const String& name, const float& v0, const float& v1);
		void setUniformValue(const String& name, const float& v0, const float& v1, const float& v2);
		void setUniformValue(const String& name, const float& v0, const float& v1, const float& v2, const float& v3);

	private:
		void load();

		std::string _vertexPath = "";
		std::string _fragmentPath = "";

		std::string _vertexData = "";
		std::string _fragmentData = "";

		bool _ready = false;
		bool _failed = false;
		bool _empty = false;

		unsigned int _program = 0;
	};
}

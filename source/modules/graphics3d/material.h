#pragma once

#include <map>
#include <string>

#include "graphics/graphics.h"

// TODO handling of unavailable uniforms

namespace gl3d
{
	/**
	 * @brief Represents a material as used by a shader program.
	 * 
	 */
	class Material
	{
	public:
		Material() = default;

		/**
		 * @brief Adds a texture to be used as a shader sampler uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param tex the texture object
		 */
		void addTexture(std::string name, gl::Texture2D tex);

		/**
		 * @brief Adds a Vec3f to be used as a shader vec3 uniform.
		 * 
		 * @param name the shader uniform's name
		 * @param value the value to be set
		 */
		void addVec3(std::string name, gml::Vec3f value);

		/**
		 * @brief Adds a float value to be used as a shader float uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param value the value to be set
		 */
		void addFloat(std::string name, float value);

		/**
		 * @brief Adds an int value to be used as a shader int uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param value the value to be set
		 */
		void addInteger(std::string name, int value);

		/**
		 * @brief Updates the shader's material uniforms with local values.
		 */
		void updateShader();

		/**
		 * @brief Sets the shader to be ussed by this material.
		 * 
		 * @param shader the shader to set
		 */
		void setShader(gl::Shader shader);

		/**
		 * @brief Returns the shader used by this material.
		 * 
		 * @return the shader
		 */
		gl::Shader getShader() const;

	private:
		gl::Shader shader;
		std::map<std::string, gl::Texture2D> textures;
		std::map<std::string, gml::Vec3f> vec3Values;
		std::map<std::string, float> fValues;
		std::map<std::string, int> iValues;
	};
}
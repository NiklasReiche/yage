#pragma once

#include <memory>

#include "utils/utils.h"
#include "graphics/graphics.h"
#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneObject.h"
#include "light/light.h"
#include "light/dirLight.h"
#include "light/pointLight.h"
#include "resourceManager.h"
#include "material.h"
#include "mesh.h"

namespace gl3d
{
	struct Geom
	{
		Mesh mesh;
		Material material;
		gml::Mat4d transform;
	};

	struct ShaderUniformValues
	{
		gml::Vec3d viewPos;
		gml::Mat4d view;
		std::vector<std::shared_ptr<DirLight>> dirLights;
		std::vector<std::shared_ptr<PointLight>> pointLights;
	};

	class SceneRenderer
	{
	public:
		gl::Renderer renderer = nullptr;

		void renderGraph(const std::shared_ptr<SceneNode>& root);

	private:
		ShaderUniformValues uniformValues;

		void setLightColorsShader(gl::Shader shader, std::string lightType, unsigned int pos, const LightColor & color) const;
		void setLightConstantsShader(gl::Shader shader, std::string lightType, unsigned int pos, const LightConstants & constants) const;

		void setDirLightShader(gl::Shader shader, unsigned int pos, std::shared_ptr<DirLight> light) const;
		void setPointLightShader(gl::Shader shader, unsigned int pos, std::shared_ptr<PointLight> light) const;
	};
}

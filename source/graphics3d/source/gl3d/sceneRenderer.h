#pragma once

#include <memory>

#include <utils/strings.h>
#include <core/gl/graphics.h>
#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneObject.h"
#include "light/light.h"
#include "light/dirLight.h"
#include "light/pointLight.h"
#include "resourceManager.h"
#include "material.h"
#include "mesh.h"
#include "light/pbr/PointLight.h"

namespace gl3d
{
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
		SceneRenderer() = default;
		explicit SceneRenderer(std::shared_ptr<gl::IRenderer> renderer);

		void renderGraph(const std::shared_ptr<SceneNode>& root);

	private:
		std::shared_ptr<gl::IRenderer> renderer;
		ShaderUniformValues uniformValues;

		void setLightColorsShader(gl::IShader& shader, std::string lightType, unsigned int pos, const LightColor & color) const;
		void setLightConstantsShader(gl::IShader& shader, std::string lightType, unsigned int pos, const LightConstants & constants) const;

		void setDirLightShader(gl::IShader& shader, unsigned int pos, const std::shared_ptr<DirLight>& light) const;
		void setPointLightShader(gl::IShader& shader, unsigned int pos, const std::shared_ptr<PointLight>& light) const;

		void setPBRPointLightShader(gl::IShader& shader, unsigned pos, const pbr::PointLight& light) const;
	};
}

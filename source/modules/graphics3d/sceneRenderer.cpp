#include "sceneRenderer.h"

namespace gl3d
{
	void SceneRenderer::renderGraph(std::shared_ptr<SceneNode> root)
	{
		std::vector<Geom> drawablesLoop;
		uniformValues.dirLights.clear();
		uniformValues.pointLights.clear();

		auto collectGeometry = [this, &drawablesLoop](SceneObject* node, gml::Mat4d transform)
		{
			if (node->hasMaterial() && node->hasMesh())
			{
				drawablesLoop.push_back({ 
					*node->getMesh(), 
					*node->getMaterial(),
					transform});
			}
			if (node->hasLight())
			{
				std::shared_ptr<Light> light = node->getLight();
				switch(light->getType())
				{
				case LightType::DIR_LIGHT:
					{
					std::shared_ptr<DirLight> dirLight = std::static_pointer_cast<DirLight>(light);
					uniformValues.dirLights.push_back(dirLight);
					}
				case LightType::POINT_LIGHT:
					{
					std::shared_ptr<PointLight> pointLight = std::static_pointer_cast<PointLight>(light);
					pointLight->setPosition(gml::Vec3d(transform.getTranslation()));
					uniformValues.pointLights.push_back(pointLight);
					}
				default:
					break;
				}
			}
			if (node->hasCamera())
			{
				std::shared_ptr<Camera> camera = node->getCamera();
				camera->moveTo(transform.getTranslation());
				camera->rotateTo(transform.getRotation().toQuaternion());
			}
		};

		root->updateChildren(collectGeometry);

		for (Geom drawable : drawablesLoop)
		{
			gl::Shader shader = drawable.material.getShader();
			shader.setUniform("model", drawable.transform);

			shader.setUniform("number_dirLight", (int)uniformValues.dirLights.size());
			for (int i = 0; i < uniformValues.dirLights.size(); ++i)
			{
				setDirLightShader(shader, i, uniformValues.dirLights.at(i));
			}

			shader.setUniform("number_pointLight", (int)uniformValues.pointLights.size());
			for (int i = 0; i < uniformValues.pointLights.size(); ++i)
			{
				setPointLightShader(shader, i, uniformValues.pointLights.at(i));
			}

			drawable.material.updateShader();
			this->gl->draw(drawable.mesh.drawable);
		}
	}

	void SceneRenderer::setLightColorsShader(gl::Shader shader, std::string lightType, unsigned int pos, const LightColor& color) const
	{
		shader.setUniform(lightType + "[" + util::to_string(pos) + "].ambient", color.ambient);
		shader.setUniform(lightType + "[" + util::to_string(pos) + "].diffuse", color.diffuse);
		shader.setUniform(lightType + "[" + util::to_string(pos) + "].specular", color.specular);
	}

	void SceneRenderer::setLightConstantsShader(gl::Shader shader, std::string lightType, unsigned int pos, const LightConstants& constants) const
	{
		shader.setUniform(lightType + "[" + util::to_string(pos) + "].constant", constants.constant);
		shader.setUniform(lightType + "[" + util::to_string(pos) + "].linear", constants.linear);
		shader.setUniform(lightType + "[" + util::to_string(pos) + "].quadratic", constants.quadratic);
	}

	void SceneRenderer::setDirLightShader(gl::Shader shader, unsigned pos, std::shared_ptr<DirLight> light) const
	{
		setLightColorsShader(shader, "dirLights", pos, light->getColors());
		shader.setUniform("dirLights[" + util::to_string(pos) + "].direction", light->getDirection());
	}

	void SceneRenderer::setPointLightShader(gl::Shader shader, unsigned pos, std::shared_ptr<PointLight> light) const
	{
		setLightColorsShader(shader, "pointLights", pos, light->getColors());
		setLightConstantsShader(shader, "pointLights", pos, light->getConstants());
		shader.setUniform("pointLights[" + util::to_string(pos) + "].position", light->getPosition());
	}
}

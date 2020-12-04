#include "sceneRenderer.h"

namespace gl3d
{
	SceneRenderer::SceneRenderer(std::shared_ptr<gl::IRenderer> renderer)
		: renderer(std::move(renderer))
	{

	}

	void SceneRenderer::renderGraph(const std::shared_ptr<SceneNode>& root)
	{
		std::vector<Geom> drawablesLoop;
		uniformValues.dirLights.clear();
		uniformValues.pointLights.clear();

		auto collectGeometry = [this, &drawablesLoop](SceneObject* node, gml::Mat4d transform)
		{
			if (node->hasMaterial() && node->hasMesh()) {
				drawablesLoop.push_back({
					                        node->getMesh(),
					                        node->getMaterial(),
					                        transform });
			}
			if (node->hasLight()) {
				std::shared_ptr<Light> light = node->getLight();
				switch (light->getType()) {
					case LightType::DIR_LIGHT: {
						std::shared_ptr<DirLight> dirLight = std::static_pointer_cast<DirLight>(light);
						uniformValues.dirLights.push_back(dirLight);
					}
						break;
					case LightType::POINT_LIGHT: {
						std::shared_ptr<PointLight> pointLight = std::static_pointer_cast<PointLight>(light);
						pointLight->setPosition(gml::Vec3d(transform.getTranslation()));
						uniformValues.pointLights.push_back(pointLight);
					}
						break;
					default: break;
				}
			}
			if (node->hasCamera()) {
				std::shared_ptr<Camera> camera = node->getCamera();
				camera->moveTo(transform.getTranslation());
				camera->rotateTo(gml::quaternion::fromMatrix<double>(transform.getRotation()));
			}
		};

		root->updateChildren(collectGeometry);

		for (Geom drawable : drawablesLoop) {
			std::shared_ptr<gl::IShader> shader = drawable.material->getShader();
			shader->setUniform("model", drawable.transform);

			shader->setUniform("number_dirLight", (int)uniformValues.dirLights.size());
			for (int i = 0; i < (int)uniformValues.dirLights.size(); ++i) {
				setDirLightShader(*shader, i, uniformValues.dirLights.at(i));
			}

			shader->setUniform("number_pointLight", (int)uniformValues.pointLights.size());
			for (int i = 0; i < (int)uniformValues.pointLights.size(); ++i) {
				setPointLightShader(*shader, i, uniformValues.pointLights.at(i));
			}

			drawable.material->updateShader();
			this->renderer->draw(*(drawable.mesh->drawable));
		}
	}

	void SceneRenderer::setLightColorsShader(
		gl::IShader& shader,
		std::string lightType,
		unsigned int pos,
		const LightColor& color) const
	{
		shader.setUniform(lightType + "[" + utils::toString(pos) + "].ambient", color.ambient);
		shader.setUniform(lightType + "[" + utils::toString(pos) + "].diffuse", color.diffuse);
		shader.setUniform(lightType + "[" + utils::toString(pos) + "].specular", color.specular);
	}

	void SceneRenderer::setLightConstantsShader(
		gl::IShader& shader,
		std::string lightType,
		unsigned int pos,
		const LightConstants& constants) const
	{
		shader.setUniform(lightType + "[" + utils::toString(pos) + "].constant", constants.constant);
		shader.setUniform(lightType + "[" + utils::toString(pos) + "].linear", constants.linear);
		shader.setUniform(lightType + "[" + utils::toString(pos) + "].quadratic", constants.quadratic);
	}

	void
	SceneRenderer::setDirLightShader(gl::IShader& shader, unsigned pos, const std::shared_ptr<DirLight>& light) const
	{
		setLightColorsShader(shader, "dirLights", pos, light->getColors());
		shader.setUniform("dirLights[" + utils::toString(pos) + "].direction", light->getDirection());
	}

	void SceneRenderer::setPointLightShader(
		gl::IShader& shader,
		unsigned pos,
		const std::shared_ptr<PointLight>& light) const
	{
		setLightColorsShader(shader, "pointLights", pos, light->getColors());
		setLightConstantsShader(shader, "pointLights", pos, light->getConstants());
		shader.setUniform("pointLights[" + utils::toString(pos) + "].position", light->getPosition());
	}
}

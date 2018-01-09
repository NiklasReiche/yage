#include "sceneRenderer.h"

namespace graphics3d
{
	void SceneRenderer::renderGraph(SceneNode* root)
	{
		std::vector<Geom> drawablesLoop;
		std::vector<PointLight> lightsLoop;

		auto collectGeometry = [this, &drawablesLoop, &lightsLoop](SceneObject* node, gml::Matrix4D<float> transform)
		{
			if (drawables.count(node->getID()))
				drawablesLoop.push_back({ drawables.at(node->getID()), materials.at(node->getID()), transform });
			if (lights.count(node->getID())) {
				lightsLoop.push_back(lights.at(node->getID()));
				lightsLoop.back().position = gml::Vec3f(transform.getTranslation());
			}

		};

		auto applyTransform = [](SceneGroup* node, gml::Matrix4D<float> transform)
		{
			return node->applyTransform(transform);
		};

		root->updateChildren(collectGeometry, applyTransform, gml::Matrix4D<float>());

		this->shader.setUniform("number_pointLight", (int)lightsLoop.size());
		for (int i = 0; i < lightsLoop.size(); ++i)
		{
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].position", lightsLoop.at(i).position);
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].ambient", lightsLoop.at(i).color.ambient);
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].diffuse", lightsLoop.at(i).color.diffuse);
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].specular", lightsLoop.at(i).color.specular);
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].constant", lightsLoop.at(i).constant);
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].linear", lightsLoop.at(i).linear);
			this->shader.setUniform("pointLights[" + util::to_string(i) + "].quadratic", lightsLoop.at(i).quadratic);
		}
		for (Geom drawable : drawablesLoop)
		{
			this->shader.setUniform("model", drawable.transform);
			this->shader.setUniform("material.ambient", drawable.material.ambient);
			this->shader.setUniform("material.diffuse", drawable.material.diffuse);
			this->shader.setUniform("material.specular", drawable.material.specular);
			this->shader.setUniform("material.shininess", drawable.material.shininess);
			this->gl->draw(drawable.drawable);
		}
	}

	void SceneRenderer::addDrawable(int id, gl::Drawable drawable)
	{
		drawables[id] = drawable;
	}
	void SceneRenderer::addMaterial(int id, Mat drawable)
	{
		materials[id] = drawable;
	}
	void SceneRenderer::addLight(int id, PointLight drawable)
	{
		lights[id] = drawable;
	}
}

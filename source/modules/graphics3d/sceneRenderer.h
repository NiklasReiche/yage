#pragma once

#include "graphics/graphics.h"
#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneGeometry.h"

namespace graphics3d
{
	struct Geom
	{
		gl::Drawable drawable;
		gml::Matrix4D<float> transform;
	};

	class SceneRenderer
	{
	public:
		gl::GraphicsContext* gl;
		gl::Shader shader;

		void renderGraph(SceneNode* root)
		{
			std::vector<Geom> drawables;

			auto collectGeometry = [&drawables](SceneGeometry* node, gml::Matrix4D<float> transform)
			{
				Geom geom = { node->drawable, transform };
				drawables.push_back(geom);
			};

			auto applyTransform = [](SceneGroup* node, gml::Matrix4D<float> transform)
			{
				return node->applyTransform(transform);
			};

			root->updateChildren(collectGeometry, applyTransform, gml::Matrix4D<float>());

			for (Geom drawable : drawables)
			{
				this->shader.setUniform("model", drawable.transform);
				this->gl->draw(drawable.drawable);
			}
		}
	};
}

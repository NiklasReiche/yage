#pragma once

#include "graphics/graphics.h"
#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneGeometry.h"

namespace graphics3d
{
	class SceneRenderer
	{
	public:
		gl::GraphicsContext* gl;
		gl::Shader shader;

		void renderGraph(SceneNode* root, gml::Matrix4D<float> transform)
		{
			auto drawGeometry = [this](SceneGeometry* node, gml::Matrix4D<float> transform)
			{
				this->shader.setUniform("model", transform);
				this->gl->draw(node->drawable);
			};

			root->updateChildren(drawGeometry, transform);
		}
	};
}

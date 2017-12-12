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

		void renderGraph(SceneNode* node, gml::Matrix4D<float> transform)
		{
			for (int i = 0; i < node->children.size(); ++i) {
				if (node->children.at(i)->type == 1) {
					SceneGroup* group = (SceneGroup*)node->children.at(i);
					renderGraph(node->children.at(i), group->applyTransform(transform));
				}
				else if (node->children.at(i)->type == 2) {
					SceneGeometry* geometry = (SceneGeometry*)node->children.at(i);
					shader.setUniform("model", transform);
					gl->draw(geometry->drawable);
				}
			}
		}
	};
}

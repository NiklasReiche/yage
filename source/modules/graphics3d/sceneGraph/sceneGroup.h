#pragma once

#include "math/matrix.h"

#include "sceneNode.h"

namespace graphics3d
{
	class SceneGroup : public SceneNode
	{
	private:
		gml::Matrix4D<float> transform;

	public:
		SceneGroup()
		{
			this->type = NodeType::GROUP;
		}
		SceneGroup(gml::Matrix4D<float> transform)
			:transform(transform) 
		{
			this->type = NodeType::GROUP;
		}

		gml::Matrix4D<float> applyTransform(gml::Matrix4D<float> transform)
		{
			return this->transform * transform;
		}

		void setTransform(gml::Matrix4D<float> tranform)
		{
			this->transform = tranform;
		}
	};
}

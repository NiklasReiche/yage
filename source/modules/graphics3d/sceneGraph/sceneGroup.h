#pragma once

#include "sceneNode.h"

#include "math/matrix.h"

namespace graphics3d
{
	class SceneGroup : public SceneNode
	{
	private:
		gml::Matrix4D<float> transform;

	public:
		SceneGroup()
			{
				this->type = 1;
			}
		SceneGroup(gml::Matrix4D<float> transform)
			:transform(transform) 
			{
				this->type = 1;
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

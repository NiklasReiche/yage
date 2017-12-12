#pragma once

#include "sceneNode.h"

#include "math/matrix.h"
#include "../material.h"

namespace graphics3d
{
	struct SceneGroup : SceneNode
	{
	private:
		gml::Matrix4D<float> transform;

	public:
		SceneGroup()
		{
			type = 1;
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
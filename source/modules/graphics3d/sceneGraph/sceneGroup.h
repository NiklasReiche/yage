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
		SceneGroup();
		SceneGroup(gml::Matrix4D<float> transform);

		void addChild(SceneNode* node);

		gml::Matrix4D<float> applyTransform(gml::Matrix4D<float> transform);

		void setTransform(gml::Matrix4D<float> tranform);
	};
}

#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "math/matrix.h"

namespace graphics3d
{
	class SceneGeometry;

	class SceneNode
	{
	protected:
		int type = 0;

	public:
		std::vector<SceneNode*> children;

	public:
		void updateChildren(std::function<void(SceneGeometry*, gml::Matrix4D<float>)> func, gml::Matrix4D<float> transform);

		void addChild(SceneNode* node);

		int getType();
	};
}

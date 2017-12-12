#pragma once

#include <vector>
#include <memory>

namespace graphics3d
{
	struct SceneNode
	{
	public:
		int type = 0;
		std::vector<SceneNode*> children;

	public:
		void addChild(SceneNode* node)
		{
			children.push_back(node);
		}
	};
}
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "math/matrix.h"

namespace graphics3d
{
	class SceneObject;
	class SceneGroup;

	enum class NodeType
	{
		NODE,
		GROUP,
		OBJECT,
	};

	class SceneNode
	{
	private:
		std::string name;

	protected:
		std::vector<SceneNode*> children;
		NodeType type = NodeType::NODE;

	public:
		// Func1: void (SceneObject*, gml::Matrix4D<float>)
		// Func2: gml::Matrix4D<float> (SceneGroup*, gml::Matrix4D<float>)
		template <typename Func1, typename Func2>
		void updateChildren(Func1&& func, Func2&& func2, gml::Matrix4D<float> transform)
		{
			for (SceneNode* child : children)
			{
				switch (child->getType())
				{
				case NodeType::GROUP:
					child->updateChildren(func, func2, func2((SceneGroup*)child, transform));
					break;
				case NodeType::OBJECT:
					func(static_cast<SceneObject*>(child), transform);
					break;
				}
			}
		}

		NodeType getType()
		{
			return type;
		}
	};
}

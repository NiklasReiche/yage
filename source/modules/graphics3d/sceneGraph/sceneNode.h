#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "math/matrix.h"

namespace graphics3d
{
	class SceneGeometry;
	class SceneGroup;

	enum class NodeType
	{
		NODE,
		GROUP,
		GEOMETRY,
	};

	class SceneNode
	{
	private:
		std::vector<SceneNode*> children;

	protected:
		NodeType type = NodeType::NODE;

	public:
		// Func1: void (SceneGeometry*, gml::Matrix4D<float>)
		// Func2: gml::Matrix4D<float> (SceneGeometry*, gml::Matrix4D<float>)
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
				case NodeType::GEOMETRY:
					func((SceneGeometry*)child, transform);
					break;
				}
			}
		}

		void addChild(SceneNode* node)
		{
			children.push_back(node);
		}

		NodeType getType()
		{
			return type;
		}
	};
}

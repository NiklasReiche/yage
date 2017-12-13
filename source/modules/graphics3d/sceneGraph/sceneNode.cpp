#include "sceneNode.h"
#include "sceneGroup.h"
#include "sceneGeometry.h"

namespace graphics3d
{
    void SceneNode::updateChildren(std::function<void(SceneGeometry*, gml::Matrix4D<float>)> func, gml::Matrix4D<float> transform)
    {
        for (int i = 0; i < children.size(); ++i) {
            SceneNode *child = children.at(i);
            if (child->getType() == 1) {
                SceneGroup* group = (SceneGroup*)child;
                child->updateChildren(func, group->applyTransform(transform));
            }
            else if (child->getType() == 2) {
                func((SceneGeometry*)child, transform);
            }
        }
    }

    void SceneNode::addChild(SceneNode* node)
	{
		children.push_back(node);
	}

	int SceneNode::getType()
	{
        return type;
	}
}
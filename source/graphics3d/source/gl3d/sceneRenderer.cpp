#include "sceneRenderer.h"

namespace gl3d
{
    struct Geometry
    {
        std::shared_ptr<Mesh> mesh;
        gml::Mat4d transform;
    };

	SceneRenderer::SceneRenderer(std::shared_ptr<gl::IRenderer> renderer)
		: renderer(std::move(renderer))
	{
	}

	void SceneRenderer::renderGraph(const std::shared_ptr<SceneNode>& root)
	{
		std::vector<Geometry> drawablesLoop;
		uniformValues.lights.clear();

		auto collectGeometry = [this, &drawablesLoop](SceneObject* node, gml::Mat4d transform)
		{
			if (node->hasMesh()) {
				drawablesLoop.emplace_back(node->getMesh(), transform);
			}
			if (node->hasLight()) {
				std::shared_ptr<Light> light = node->getLight();
                light->update_from_transform(transform);
                uniformValues.lights.push_back(light);
			}
			if (node->hasCamera()) {
				std::shared_ptr<Camera> camera = node->getCamera();
				camera->moveTo(transform.getTranslation());
				camera->rotateTo(gml::quaternion::fromMatrix<double>(transform.getRotation()));
			}
		};

        // TODO: use found camera from scene graph to set shader camPos, like we do with lights

		root->updateChildren(collectGeometry);

		for (auto& geometry : drawablesLoop) {
            for (auto& sub_mesh : geometry.mesh->sub_meshes()) {
                for (std::size_t i = 0; i < uniformValues.lights.size(); ++i) {
                    uniformValues.lights[i]->update_uniforms(*sub_mesh->material().shader(), i);
                }

                sub_mesh->material().shader()->setUniform("model", geometry.transform);
                sub_mesh->material().update_shader_uniforms();
                sub_mesh->material().bind_textures(*renderer);

                renderer->draw(sub_mesh->drawable());
            }
		}
	}
}

#pragma once

#include <memory>

#include <core/gl/graphics.h>

#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneObject.h"
#include "light.h"
#include "ProjectionView.h"

namespace yage::gl3d
{
	struct ShaderUniformValues
	{
		math::Vec3d viewPos;
		math::Mat4d view;
        std::vector<std::shared_ptr<Light>> dir_lights;
		std::vector<std::shared_ptr<Light>> point_lights;
        std::shared_ptr<Camera> camera;
	};

	class SceneRenderer
	{
	public:
		explicit SceneRenderer(gl::IContext& context);

		void render_graph(const std::shared_ptr<SceneNode>& root, const Camera& target_camera);

		math::Mat4f& projection();

		math::Mat4f& view();

		const ProjectionView& projection_view();

		gl::IRenderer& base_renderer();

	private:
		std::shared_ptr<gl::IRenderer> m_renderer;
		ShaderUniformValues m_uniform_values;
		ProjectionView m_projection_view;
	};
}

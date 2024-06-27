#pragma once

#include <memory>

#include <core/gl/graphics.h>

#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneObject.h"
#include "light.h"
#include "ProjectionView.h"
#include "shaders.h"

namespace yage::gl3d
{
	struct ShaderUniformValues
	{
        std::vector<std::shared_ptr<Light>> dir_lights;
		std::vector<std::shared_ptr<Light>> point_lights;
	};

	class SceneRenderer
	{
	public:
	    std::shared_ptr<Camera> active_camera;
        std::shared_ptr<SceneGroup> active_scene;

		explicit SceneRenderer(gl::IContext& context);

		void render_active_scene();

		math::Mat4f& projection();

		math::Mat4f& view();

		const ProjectionView& projection_view();

		gl::IRenderer& base_renderer();

		ShaderMap shaders() const;

	private:
        std::shared_ptr<gl::IRenderer> m_renderer;
		ShaderUniformValues m_uniform_values;
		ProjectionView m_projection_view;
		ShaderMap m_shaders;

	    std::vector<std::reference_wrapper<SceneObject>> m_drawables;

	    void collect_entities(SceneObject& node);
	};
}

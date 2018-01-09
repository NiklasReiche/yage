#pragma once

#include "platform/util.h"
#include "graphics/graphics.h"
#include "sceneGraph/sceneNode.h"
#include "sceneGraph/sceneGroup.h"
#include "sceneGraph/sceneObject.h"
#include "light.h"

namespace graphics3d
{
	struct Mat
	{
		gml::Vec3f ambient;
		gml::Vec3f diffuse;
		gml::Vec3f specular;
		float shininess;
	};
	struct Geom
	{
		gl::Drawable drawable;
		Mat material;
		gml::Matrix4D<float> transform;
	};
	

	class SceneRenderer
	{
	private:
		std::map<int, gl::Drawable> drawables;
		std::map<int, Mat> materials;
		std::map<int, PointLight> lights;

	public:
		gl::GraphicsContext* gl;
		gl::Shader shader;

		void renderGraph(SceneNode* root);

		void addDrawable(int id, gl::Drawable drawable);
		void addMaterial(int id, Mat drawable);
		void addLight(int id, PointLight drawable);
	};
}

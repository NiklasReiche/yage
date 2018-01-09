#pragma once

#include "platform/platform.h"
#include "graphics/gl.h"
#include "input/InputController.h"
#include "math/MVector.h"

#include "graphics3d/camera.h"
#include "graphics3d/sceneRenderer.h"

class App
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* glContext;
	input::InputController* inputController;

	gl::Shader shader;
	graphics3d::Camera camera;
	graphics3d::SceneRenderer renderer;

	graphics3d::SceneGroup scene;
	graphics3d::SceneGroup group1;
	graphics3d::SceneObject cube1;
	graphics3d::SceneObject light1;
	graphics3d::SceneGroup group2;
	graphics3d::SceneObject cube2;
	graphics3d::SceneObject cube3;
	graphics3d::SceneGroup group3;

public:
	App()
	{
		platform = new sys::PlatformHandle();
		glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Sandbox");
		inputController = new input::InputController(platform);

		renderer.gl = glContext;

		camera = graphics3d::Camera(gml::Vec3f(0.0f, 0.0f, 2.0f), gml::axisAngle<float>(0.0f, gml::Vec3f(0.0f, 0.0f, -1.0f)));

		glContext->setActiveViewport(gl::Viewport(0, 0, 1500, 900));


		std::string vertexShader =
#include "graphics3d/shaders/singleColorShader.vert"
			;
		std::string fragmentShader =
#include "graphics3d/shaders/singleColorShader.frag"
			;
		gl::ShaderLoader shaderLoader(platform, glContext);
		shader = shaderLoader.loadFromString(vertexShader, fragmentShader);
		renderer.shader = shader;

		gml::Matrix4D<float> proj = gml::perspective<float>(45.0f, (float)(1500) / (float)(900), 0.1f, 1000.0f);
		shader.setUniform("projection", proj);
		shader.setUniform("viewPos", camera.getPosition());

		setupGraph();
		glContext->enableDepthTest();
	}

	void setupGraph()
	{
		std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		gl::Drawable cube = glContext->createDrawable(vertices, { 3, 3 }, gl::DrawMode::DRAW_STATIC);
		renderer.addMaterial(1, { gml::Vec3f(0.1f), gml::Vec3f(0.61f, 0.04f, 0.04f), gml::Vec3f(0.7f, 0.6f, 0.6), 0.6f });
		renderer.addDrawable(1, cube);
		renderer.addMaterial(2, { gml::Vec3f(0.1f), gml::Vec3f(0.61f, 0.04f, 0.04f), gml::Vec3f(0.7f, 0.6f, 0.6), 0.6f });
		renderer.addDrawable(2, cube);
		renderer.addMaterial(4, { gml::Vec3f(1.0f), gml::Vec3f(1.0f), gml::Vec3f(1.0f), 0.6f });
		renderer.addDrawable(4, cube);

		graphics3d::PointLight light;
		light.color.ambient = gml::Vec3f(0.2f);
		light.color.diffuse = gml::Vec3f(0.5f);
		light.color.specular = gml::Vec3f(1.0f);
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;
		renderer.addLight(3, light);

		cube1 = graphics3d::SceneObject(1);
		cube2 = graphics3d::SceneObject(2);
		light1 = graphics3d::SceneObject(3);
		cube3 = graphics3d::SceneObject(4);

		group1.setTransform(gml::translate<float>(gml::Vec3f(0.0f, 1.5f, 0.0f)));
		group1.addChild(&cube1);

		group3.addChild(&light1);
		group3.addChild(&cube3);

		group2.setTransform(gml::translate<float>(gml::Vec3f(0.0f, 0.0f, -3.0f)));
		group2.addChild(&cube2);

		group2.addChild(&group1);
		group2.addChild(&group3);

		scene.addChild(&group2);
	}

	void run()
	{
		float coord = -1.0f;
		glContext->showWindow();
		platform->getTimeStep();
		while (!platform->shouldDestroy())
		{
			double dt = platform->getTimeStep();
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glContext->useShader(shader);
			shader.setUniform("view", camera.getViewMatrix());

			coord += 0.1 * dt;
			gml::Matrix4D<float> globalTransform = gml::translate<float>(gml::Vec3f(0.0f, 0.0f, -3.0f)) * gml::rotate<float>(coord * 100, gml::Vec3f(0.0f, 0.0f, -1.0f));
			gml::Matrix4D<float> localTransform = gml::translate<float>(gml::Vec3f(0.0f, 1.5f, 0.0f)) * gml::rotate<float>(coord * 100, gml::Vec3f(0.0f, 0.0f, -1.0f));

			group2.setTransform(globalTransform);
			group1.setTransform(localTransform);
			localTransform = gml::rotate<float>(coord * 300, gml::Vec3f(1.0f, 0.0f, 1.0f)) * gml::translate<float>(gml::Vec3f(0.0f, -1.5f, 0.0f)) * gml::scale<float>(0.1f);
			group3.setTransform(localTransform);

			renderer.renderGraph(&scene);

			glContext->swapBuffers();
			inputController->poll();
		}
	}
};

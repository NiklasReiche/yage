#pragma once

#include <platform/platform.h>
#include <graphics/gl.h>
#include <input/InputController.h>
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

	graphics3d::SceneNode scene;
	graphics3d::SceneGroup group;
	graphics3d::SceneGeometry geom;

	graphics3d::SceneGroup group2;
	graphics3d::SceneGeometry geom2;

public:
	App()
	{
		platform = new sys::PlatformHandle();
		glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Sandbox");
		inputController = new input::InputController(platform);

		renderer.gl = glContext;

		camera = graphics3d::Camera(gml::Vec3f(0.0f, 0.0f, 0.0f), gml::axisAngle<float>(0.0f, gml::Vec3f(0.0f, 0.0f, -1.0f)));

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

		setupGraph();
	}

	void setupGraph()
	{
		std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		gl::Drawable cube = glContext->createDrawable(vertices, { 3 }, gl::DrawMode::DRAW_STATIC);
		geom.drawable = cube;
		geom2.drawable = cube;

		group.setTransform(gml::translate<float>(gml::Vec3f(0.0f, 1.5f, 0.0f)));
		group.addChild(&geom);
		
		group2.setTransform(gml::translate<float>(gml::Vec3f(0.0f, 0.0f, -3.0f)));
		group2.addChild(&geom2);

		group2.addChild(&group);

		scene.addChild(&group2);
	}

	void run()
	{
		float coord = -1.0f;
		gml::Matrix4D<float> globalTransform = gml::translate<float>(gml::Vec3f(0.0f, 0.0f, -3.0f));
		glContext->showWindow();
		platform->getTimeStep();
		while (!platform->shouldDestroy())
		{
			double dt = platform->getTimeStep();
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glContext->useShader(shader);
			shader.setUniform("view", camera.getViewMatrix());

			coord += 0.1 * dt;
			globalTransform = gml::translate<float>(gml::Vec3f(coord, 0.0f, -3.0f));
			
			group2.setTransform(globalTransform);
			renderer.renderGraph(&scene, gml::Matrix4D<float>());

			glContext->swapBuffers();
			inputController->poll();
		}
	}
};
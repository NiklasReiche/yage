#include "platform/Platform.h"
#include "graphics/gl.h"
#include "input/InputController.h"
#include "math/MVector.h"

#include "graphics3d/skybox.h"
#include "graphics3d/camera.h"

#include "gui.h"

int main()
{
	sys::PlatformHandle platform;
	gl::GraphicsContext glContext(&platform, 1000, 700);
	input::InputController inputController(&platform);

	Gui gui(&platform, &glContext, &inputController);



	std::array<std::string, 6> filenames = {
		"D:/DEV/Projects/YAGE/Tests/graphics3d/skybox/ThickCloudsWater/right.png",
		"D:/DEV/Projects/YAGE/Tests/graphics3d/skybox/ThickCloudsWater/left.png",
		"D:/DEV/Projects/YAGE/Tests/graphics3d/skybox/ThickCloudsWater/top.png",
		"D:/DEV/Projects/YAGE/Tests/graphics3d/skybox/ThickCloudsWater/bottom.png",
		"D:/DEV/Projects/YAGE/Tests/graphics3d/skybox/ThickCloudsWater/back.png",
		"D:/DEV/Projects/YAGE/Tests/graphics3d/skybox/ThickCloudsWater/front.png",
	};
	graphics3d::Skybox skybox = graphics3d::SkyboxLoader(&platform, &glContext).loadSkybox(filenames, 50);

	skybox.shader.setUniform("skybox", 0);
	gml::Matrix4D<float> proj = gml::perspective<float>(45.0f, (float)500 / (float)700, 0.1f, 1000.0f);
	skybox.shader.setUniform("projection", proj);



	glContext.showWindow();
	platform.getTimeStep();
	while (!platform.shouldDestroy()) 
	{
		double dt = platform.getTimeStep();
		glContext.setActiveViewport(gl::Viewport(0,0,1000,700));
		gui.update(dt);
		
		/*
		glContext.setActiveViewport(gl::Viewport(500, 0, 500, 700));
		glContext.useShader(skybox.shader);
		glContext.bindTexture(skybox.cubemap);
		glContext.draw(skybox.drawable);
		*/

		glContext.swapBuffers();
		inputController.poll();
	}

	return 0;
}
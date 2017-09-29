#include "platform/Platform.h"
#include "graphics/gl.h"
#include "graphics3d/skybox.h"
#include "graphics3d/camera.h"
#include "math/MVector.h"


int main() {

	sys::PlatformHandle platform;
	gl::GraphicsContext glContext(&platform, 500, 500);
	glContext.showWindow();

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
	gml::Matrix4D<float> proj = gml::perspective<float>(45.0f, (float)500 / (float)500, 0.1f, 1000.0f);
	skybox.shader.setUniform("projection", proj);

	
	while (!platform.shouldDestroy()) {
		platform.pollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glContext.useShader(skybox.shader);
		glContext.bindTexture(skybox.cubemap);
		glContext.draw(skybox.drawable);

		glContext.swapBuffers();
	}

	return 0;
}
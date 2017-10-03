#include "skybox.h"
#include <image/ImageReader.h>
#include <platform/Exception.h>

namespace graphics3d
{
	Skybox SkyboxLoader::loadSkybox(std::array<std::string, 6> paths, float boxSize)
	{
		img::ImageReader imageReader(platform);
		std::array<img::Image, 6> images;
		std::array<unsigned char*, 6> faces;
		
		for (unsigned int i = 0; i < paths.size(); ++i) {
			images[i] = imageReader.readFile(paths[i]);
			faces[i] = images[0].getRawData();
			if (images[i].getWidth() != images[0].getWidth() || images[i].getHeight() != images[0].getHeight()) {
				throw sys::FileException(sys::FileError::UNKNOWN, "images not same size", paths[i]);
			}
		}

		std::vector<GLfloat> skyboxVertices = {
			-boxSize,  boxSize, -boxSize,
			-boxSize, -boxSize, -boxSize,
			boxSize, -boxSize, -boxSize,
			boxSize, -boxSize, -boxSize,
			boxSize,  boxSize, -boxSize,
			-boxSize,  boxSize, -boxSize,

			-boxSize, -boxSize,  boxSize,
			-boxSize, -boxSize, -boxSize,
			-boxSize,  boxSize, -boxSize,
			-boxSize,  boxSize, -boxSize,
			-boxSize,  boxSize,  boxSize,
			-boxSize, -boxSize,  boxSize,

			boxSize, -boxSize, -boxSize,
			boxSize, -boxSize,  boxSize,
			boxSize,  boxSize,  boxSize,
			boxSize,  boxSize,  boxSize,
			boxSize,  boxSize, -boxSize,
			boxSize, -boxSize, -boxSize,

			-boxSize, -boxSize,  boxSize,
			-boxSize,  boxSize,  boxSize,
			boxSize,  boxSize,  boxSize,
			boxSize,  boxSize,  boxSize,
			boxSize, -boxSize,  boxSize,
			-boxSize, -boxSize,  boxSize,

			-boxSize,  boxSize, -boxSize,
			boxSize,  boxSize, -boxSize,
			boxSize,  boxSize,  boxSize,
			boxSize,  boxSize,  boxSize,
			-boxSize,  boxSize,  boxSize,
			-boxSize,  boxSize, -boxSize,

			-boxSize, -boxSize, -boxSize,
			-boxSize, -boxSize,  boxSize,
			boxSize, -boxSize, -boxSize,
			boxSize, -boxSize, -boxSize,
			-boxSize, -boxSize,  boxSize,
			boxSize, -boxSize,  boxSize
		};

		Skybox skybox;
		skybox.cubemap = glContext->createCubemapTexture(faces, images.at(0).getWidth(), images.at(0).getHeight(), gl::ImageFormat::RGB);
		skybox.drawable = glContext->createDrawable(skyboxVertices, std::vector<int>{3}, gl::DrawMode::DRAW_STATIC);

		std::string vertexShader =
#include "shaders/skyboxShader.vert"
			;
		std::string fragmentShader =
#include "shaders/skyboxShader.frag"
			;

		gl::ShaderLoader shaderLoader(platform, glContext);
		skybox.shader = shaderLoader.loadFromString(vertexShader, fragmentShader);

		return skybox;
	}
}
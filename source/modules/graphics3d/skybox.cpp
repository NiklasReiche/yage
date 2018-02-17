#include "skybox.h"
#include <image/imageReader.h>
#include <platform/exception.h>

namespace gl3d
{
	Skybox SkyboxLoader::loadSkybox(std::array<std::string, 6> paths, float boxSize, int textureSize)
	{
		img::ImageReader imageReader(platform);
		std::array<img::Image, 6> images;
		std::array<unsigned char*, 6> faces;
		gl::Texture temp;
		
		for (unsigned int i = 0; i < paths.size(); ++i) {
			if (paths.at(i).size() != 0) {
				images[i] = imageReader.readFile(paths[i], img::FORCE_CHANNELS::RGB);
				if (images.at(i).getWidth() != textureSize || images.at(i).getHeight() != textureSize) {
					temp = images.at(i).toTexture(glContext);
					temp.resize(textureSize, textureSize);
					images[i] = imageReader.readTexture(temp);
				}
				faces[i] = images[i].getRawData();
			}
			else {
				faces[i] = nullptr;
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
		skybox.cubemap = glContext->createCubemapTexture(faces, textureSize, textureSize, gl::ImageFormat::RGB);
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
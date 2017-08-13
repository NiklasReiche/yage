#include "skybox.h"
#include "image/imageLoader.h"
#include "platform/Exception.h"

namespace graphics3d
{
	Skybox SkyboxLoader::loadSkybox(std::string dirpath, float boxSize)
	{
		std::vector<std::string> paths;
		paths.push_back(dirpath + "\\right.bmp");
		paths.push_back(dirpath + "\\left.bmp");
		paths.push_back(dirpath + "\\top.bmp");
		paths.push_back(dirpath + "\\bottom.bmp");
		paths.push_back(dirpath + "\\back.bmp");
		paths.push_back(dirpath + "\\front.bmp");

		img::ImageReader imageReader(platform);
		std::array<img::Image, 6> images;
		std::array<unsigned char*, 6> faces;
		
		for (unsigned int i = 0; i < paths.size(); ++i) {
			images[i] = imageReader.readFile(paths[i]);
			faces[i] = images[0].getRawData();
			if (images[i].getWidth() != images[0].getWidth() || images[i].getHeight() != images[0].getHeight()) {
				throw FileException(FileError::UNKNOWN, dirpath);
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

		return skybox;
	}
}
#include "skybox.h"
#include <image/imageReader.h>

namespace yage::gl3d
{
	Skybox SkyboxLoader::loadSkybox(std::array<std::string, 6> paths, float boxSize, int textureSize)
	{
		std::array<img::Image, 6> images;
		std::array<std::vector<unsigned char>, 6> faces;
		gl::Texture2D temp;
		
		for (unsigned int i = 0; i < paths.size(); ++i) {
			if (paths.at(i).size() != 0) {
				images[i] = img::ImageReader::readFile(paths[i], img::FORCE_CHANNELS::RGB);
#if 0
				if (images.at(i).getWidth() != textureSize || images.at(i).getHeight() != textureSize) {
					temp = images.at(i).toTexture(glContext->newTextureCreator());
					temp->resize(textureSize, textureSize);
					images[i] = img::ImageReader::readTexture(temp);
				}
#endif
				faces[i] = std::vector<unsigned char>(images[i].getRawData(), images[i].getRawData()+images[i].getWidth()*images[i].getHeight()*images[i].getChannels());
			}
			else {
				faces[i] = std::vector<unsigned char>();
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
		skybox.cubemap = glContext->getTextureCreator()->createCubemap(faces, textureSize, textureSize, gl::ImageFormat::RGB);
		skybox.drawable = glContext->getDrawableCreator()->createDrawable(skyboxVertices, std::vector<int>{3}, gl::VertexFormat::BATCHED);

		std::string vertexShader =
#include "shaders/skyboxShader.vert"
			;
		std::string fragmentShader =
#include "shaders/skyboxShader.frag"
			;

		skybox.shader = glContext->getShaderCreator()->createShader(vertexShader, fragmentShader);

		return skybox;
	}
}
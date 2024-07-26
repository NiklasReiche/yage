#include "skybox.h"
#include <image/img.h>
#include <numeric>
#include "shaders.h"

namespace yage::gl3d
{
	Skybox loadSkybox(const platform::IFileReader& file_reader, gl::IContext& context, std::span<std::string, 6> paths,
	                  const float box_size, const int texture_size)
	{
		std::array<img::Image, 6> images;
		std::array<std::vector<unsigned char>, 6> faces;
		
		for (unsigned int i = 0; i < paths.size(); ++i) {
			if (!paths[i].empty()) {
				auto file = file_reader.open_binary_file(paths[i], platform::IFile::AccessMode::READ);
				images[i] = readFromFile(*file, img::FORCE_CHANNELS::RGB);
#if 0
				if (images.at(i).getWidth() != textureSize || images.at(i).getHeight() != textureSize) {
					gl::Texture2D temp = images.at(i).toTexture(glContext->newTextureCreator());
					temp->resize(textureSize, textureSize);
					images[i] = img::ImageReader::readTexture(temp);
				}
#endif
				faces[i] = std::vector<unsigned char>(images[i].data(), images[i].data() + images[i].getSize());
			}
			else {
				faces[i] = std::vector<unsigned char>();
			}
		}

		std::vector<float> skyboxVertices = {
			-box_size,  box_size, -box_size,
			-box_size, -box_size, -box_size,
			box_size, -box_size, -box_size,
			box_size, -box_size, -box_size,
			box_size,  box_size, -box_size,
			-box_size,  box_size, -box_size,

			-box_size, -box_size,  box_size,
			-box_size, -box_size, -box_size,
			-box_size,  box_size, -box_size,
			-box_size,  box_size, -box_size,
			-box_size,  box_size,  box_size,
			-box_size, -box_size,  box_size,

			box_size, -box_size, -box_size,
			box_size, -box_size,  box_size,
			box_size,  box_size,  box_size,
			box_size,  box_size,  box_size,
			box_size,  box_size, -box_size,
			box_size, -box_size, -box_size,

			-box_size, -box_size,  box_size,
			-box_size,  box_size,  box_size,
			box_size,  box_size,  box_size,
			box_size,  box_size,  box_size,
			box_size, -box_size,  box_size,
			-box_size, -box_size,  box_size,

			-box_size,  box_size, -box_size,
			box_size,  box_size, -box_size,
			box_size,  box_size,  box_size,
			box_size,  box_size,  box_size,
			-box_size,  box_size,  box_size,
			-box_size,  box_size, -box_size,

			-box_size, -box_size, -box_size,
			-box_size, -box_size,  box_size,
			box_size, -box_size, -box_size,
			box_size, -box_size, -box_size,
			-box_size, -box_size,  box_size,
			box_size, -box_size,  box_size
		};

		std::vector<unsigned int> indices;
		indices.reserve(36);
		std::iota(indices.begin(), indices.end(), 0);

		Skybox skybox;
		skybox.cubemap = context.getTextureCreator()->createCubemap(texture_size, texture_size, gl::ImageFormat::RGB, faces);
		skybox.drawable = context.getDrawableCreator()->createDrawable(skyboxVertices, indices, std::vector<unsigned int>{3}, gl::VertexFormat::BATCHED);
		skybox.shader = context.getShaderCreator()->createShader(shaders::Skybox::vert, shaders::Skybox::frag);
		return skybox;
	}
}
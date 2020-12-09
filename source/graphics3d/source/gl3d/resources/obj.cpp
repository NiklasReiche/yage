#include "obj.h"
#include <vector>
#include <gml/vector.h>
#include <utils/strings.h>

namespace gl3d::resources
{
	std::unique_ptr<gl::IDrawable>
	readObj(const platform::IFileReader& fileReader, const std::string& filename, gl::IDrawableCreator& drawableCreator)
	{
		auto file = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ);

		std::vector<gml::Vec3f> vertices;
		std::vector<gml::Vec3f> normals;
		std::vector<float> vertexData;

		while (!file->eof()) {
			std::istringstream line(file->readLine());
			std::string type;
			line >> type;

			if (type == "v") {
				float x, y, z, w;
				line >> x >> y >> z >> w;
				vertices.emplace_back(x, y, z);

			} else if (type == "vt") {

			} else if (type == "vn") {
				float x, y, z;
				line >> x >> y >> z;
				normals.emplace_back(x, y, z);

			} else if (type == "f") {
				std::string v1, v2, v3;
				line >> v1 >> v2 >> v3;
				for (auto& v : { v1, v2, v3 }) {
					if (utils::contains(v, "//")) {
						auto result = utils::strip(v, "//");
						int vi, ni;
						std::stringstream(result[0]) >> vi; vi--;
						std::stringstream(result[1]) >> ni; ni--;

						vertexData.push_back(vertices[vi].x());
						vertexData.push_back(vertices[vi].y());
						vertexData.push_back(vertices[vi].z());

						vertexData.push_back(normals[ni].x());
						vertexData.push_back(normals[ni].y());
						vertexData.push_back(normals[ni].z());

					} else if (utils::contains(v, "/")) {
						auto result = utils::strip(v, "/");
						int vi, ti, ni;
						std::stringstream(result[0]) >> vi; vi--;
						std::stringstream(result[1]) >> ti; ti--;
						std::stringstream(result[2]) >> ni; ni--;

						vertexData.push_back(vertices[vi].x());
						vertexData.push_back(vertices[vi].y());
						vertexData.push_back(vertices[vi].z());

						// TODO texcoords

						vertexData.push_back(normals[ni].x());
						vertexData.push_back(normals[ni].y());
						vertexData.push_back(normals[ni].z());

					} else {
						int vi;
						std::stringstream(v) >> vi; vi--;
						vertexData.push_back(vertices[vi].x());
						vertexData.push_back(vertices[vi].y());
						vertexData.push_back(vertices[vi].z());
					}
				}
			}
		}

		return drawableCreator.createDrawable(vertexData, { 3, 3 }, gl::VertexFormat::INTERLEAVED);
	}
}

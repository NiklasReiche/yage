#include "obj.h"
#include <vector>
#include <map>
#include <tuple>
#include <gml/vector.h>
#include <utils/strings.h>
#include <utils/NotImplementedException.h>

namespace
{
	std::map<std::string, gl3d::Material> readMtl(const platform::IFileReader& fileReader, const std::string& filename)
	{
		auto file = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ);

		std::map<std::string, gl3d::Material> materials;
		gl3d::Material material;
		std::string name;

		while (!file->eof()) {
			std::istringstream line(file->readLine());
			std::string type;
			line >> type;

			if (type == "newmtl") {
				line >> name;
				if (!name.empty()) {
					materials[name] = material;
				}
			} else if (type == "Ka") {
				float x, y, z;
				line >> x >> y >> z;
                material.add_uniform("ambient", gml::Vec3f(x, y, z));
			} else if (type == "Kd") {
				float x, y, z;
				line >> x >> y >> z;
                material.add_uniform("diffuse", gml::Vec3f(x, y, z));
			} else if (type == "Ks") {
				float x, y, z;
				line >> x >> y >> z;
                material.add_uniform("specular", gml::Vec3f(x, y, z));
			} else if (type == "Ns") {
				float s;
				line >> s;
                material.add_uniform("shininess", s);
			}
		}

		if (!name.empty()) {
			materials[name] = material;
		}

		return materials;
	}
}

namespace gl3d::resources
{
	std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material>
	readObj(const platform::IFileReader& fileReader, const std::string& filename, gl::IDrawableCreator& drawableCreator)
	{
		auto tmp = utils::strip(filename, "/");
		tmp.pop_back();
		std::string path = utils::join(tmp);

		auto file = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ);

		std::map<std::string, Material> materials;
		Material material;

		std::vector<gml::Vec3f> vertices;
		std::vector<gml::Vec2f> texCoords;
		std::vector<gml::Vec3f> normals;
		std::vector<float> vertexData;

		while (!file->eof()) {
			std::istringstream line(file->readLine());
			std::string type;
			line >> type;

			if (type == "mtllib") {
				std::string mtlFile;
				line >> mtlFile;
				materials = readMtl(fileReader, path + "/" + mtlFile);
			} else if (type == "usemtl") {
				std::string name;
				line >> name;
				material = materials[name];
			} else if (type == "v") {
				float x, y, z, w;
				line >> x >> y >> z >> w;
				vertices.emplace_back(x, y, z);

			} else if (type == "vt") {
				float u, v, w;
				line >> u >> v >> w;
				texCoords.emplace_back(u, v);

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
						std::stringstream(result[0]) >> vi;
						vi--;
						std::stringstream(result[1]) >> ni;
						ni--;

						vertexData.push_back(vertices[vi].x());
						vertexData.push_back(vertices[vi].y());
						vertexData.push_back(vertices[vi].z());

						vertexData.push_back(normals[ni].x());
						vertexData.push_back(normals[ni].y());
						vertexData.push_back(normals[ni].z());

					} else if (utils::contains(v, "/")) {
						auto result = utils::strip(v, "/");
						int vi, ti, ni;
						std::stringstream(result[0]) >> vi;
						vi--;
						std::stringstream(result[1]) >> ti;
						ti--;
						std::stringstream(result[2]) >> ni;
						ni--;

						vertexData.push_back(vertices[vi].x());
						vertexData.push_back(vertices[vi].y());
						vertexData.push_back(vertices[vi].z());

						vertexData.push_back(texCoords[ti].x());
						vertexData.push_back(texCoords[ti].y());

						vertexData.push_back(normals[ni].x());
						vertexData.push_back(normals[ni].y());
						vertexData.push_back(normals[ni].z());

					} else {
						int vi;
						std::stringstream(v) >> vi;
						vi--;
						vertexData.push_back(vertices[vi].x());
						vertexData.push_back(vertices[vi].y());
						vertexData.push_back(vertices[vi].z());
					}
				}
			}
		}

        throw NotImplementedException();

		// TODO indexing
#if 0
		auto drawable = drawableCreator.createDrawable(vertexData, std::span<unsigned int>{}, { 3, 3 }, gl::VertexFormat::INTERLEAVED);
		return std::make_tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material>(std::move(drawable),
		                                                                       static_cast<Material&&>(material));
#endif
	}
}

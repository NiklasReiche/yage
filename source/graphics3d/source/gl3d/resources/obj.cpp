#include "obj.h"

#include <vector>
#include <unordered_map>

#include <core/gl/TextureCreator.h>
#include <math/vector.h>
#include <image/img.h>
#include <utils/strings.h>

#include "../mesh.h"

namespace yage::gl3d::resources
{
    std::unordered_map<std::string, Material> read_mtl(
        const std::string& filename,
        const platform::IFileReader& file_reader,
        gl::ITextureCreator& texture_creator,
        const ShaderMap& shaders)
    {
        auto tmp = utils::strip(filename, "/");
        tmp.pop_back();
        std::string base_path = utils::join(tmp) + "/";
        auto file = file_reader.open_text_file(filename, platform::IFile::AccessMode::READ);

        std::vector<unsigned char> full_data{255, 255, 255, 255};
        const std::shared_ptr<gl::ITexture2D> full_texture = texture_creator.createTexture2D(
            1, 1, gl::ImageFormat::RGBA, full_data);

        std::unordered_map<std::string, Material> materials;
        std::string name;

        while (!file->eof()) {
            std::istringstream line(file->read_line());
            std::string type;
            line >> type;

            if (type == "newmtl") {
                line >> name;
                materials[name] = Material();
                materials[name].add_uniform("ambientMap", full_texture);
                materials[name].add_uniform("diffuseMap", full_texture);
                materials[name].add_uniform("specularMap", full_texture);
                materials[name].add_uniform("shininessMap", full_texture);
                materials[name].set_shader(shaders.at(ShaderPermutation::PHONG));
            } else if (type == "Ka") {
                float x, y, z;
                line >> x >> y >> z;
                materials[name].add_uniform("ambient", math::Vec3f(x, y, z));
            }else if (type == "Kd") {
                float x, y, z;
                line >> x >> y >> z;
                materials[name].add_uniform("diffuse", math::Vec3f(x, y, z));
            } else if (type == "Ks") {
                float x, y, z;
                line >> x >> y >> z;
                materials[name].add_uniform("specular", math::Vec3f(x, y, z));
            } else if (type == "Ns") {
                float s;
                line >> s;
                materials[name].add_uniform("shininess", s);
            } else if (type == "map_Ka") {
                std::string path;
                line >> path;
                auto image_file = file_reader.open_binary_file(base_path + path, platform::IFile::AccessMode::READ);
                img::Image image = img::readFromFile(*image_file);
                materials[name].add_uniform("ambientMap", image.toTexture(texture_creator));
            } else if (type == "map_Kd") {
                std::string path;
                line >> path;
                auto image_file = file_reader.open_binary_file(base_path + path, platform::IFile::AccessMode::READ);
                img::Image image = img::readFromFile(*image_file);
                materials[name].add_uniform("diffuseMap", image.toTexture(texture_creator));
            } else if (type == "map_Ks") {
                std::string path;
                line >> path;
                auto image_file = file_reader.open_binary_file(base_path + path, platform::IFile::AccessMode::READ);
                img::Image image = img::readFromFile(*image_file);
                materials[name].add_uniform("specularMap", image.toTexture(texture_creator));
            } else if (type == "map_Ns") {
                std::string path;
                line >> path;
                auto image_file = file_reader.open_binary_file(base_path + path, platform::IFile::AccessMode::READ);
                img::Image image = img::readFromFile(*image_file);
                materials[name].add_uniform("shininessMap", image.toTexture(texture_creator));
            } else if (type == "norm") {
                std::string path;
                line >> path;
                auto image_file = file_reader.open_binary_file(base_path + path, platform::IFile::AccessMode::READ);
                img::Image image = img::readFromFile(*image_file);
                materials[name].add_uniform("normalMap", image.toTexture(texture_creator));
                materials[name].set_shader(shaders.at(ShaderPermutation::PHONG_NORMAL_MAP));
            }
        }

        return materials;
    }

    std::vector<Mesh> read_obj(const std::string& filename,
                  const platform::IFileReader& file_reader,
                  gl::ITextureCreator& texture_creator,
                  gl::IDrawableCreator& drawable_creator,
                  const ShaderMap& shaders)
    {
        auto tmp = utils::strip(filename, "\\"); // TODO: cross-platform
        tmp.pop_back();
        std::string base_path = utils::join(tmp, "/") + "/";

        auto file = file_reader.open_text_file(filename, platform::IFile::AccessMode::READ);

        std::unordered_map<std::string, Material> materials;
        std::shared_ptr<Material> material;
        std::vector<Mesh> meshes;
        std::string mesh_name;

        std::vector<math::Vec3f> vertices;
        std::vector<math::Vec2f> texCoords;
        std::vector<math::Vec3f> normals;
        std::vector<float> vertexData;
        std::vector<unsigned int> indices;
        unsigned int index = 0;

        while (!file->eof()) {
            std::istringstream line(file->read_line());
            std::string type;
            line >> type;

            if (type == "mtllib") {
                std::string mtlFile;
                line >> mtlFile;
                materials = read_mtl(base_path + mtlFile, file_reader, texture_creator, shaders);
            } else if (type == "o") {
                if (!mesh_name.empty()) {
                    std::shared_ptr<gl::IDrawable> drawable = drawable_creator.createDrawable(
                        vertexData, indices, std::vector<unsigned int>{3, 3, 2}, gl::VertexFormat::INTERLEAVED);
                    auto sub_mesh = std::make_unique<SubMesh>(drawable, material);
                    meshes.emplace_back();
                    meshes.back().add_sub_mesh(std::move(sub_mesh));
                }
                line >> mesh_name;
            } else if (type == "usemtl") {
                // TODO: submeshing
                std::string material_name;
                line >> material_name;
                material = std::make_unique<Material>(materials[material_name]);
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
                for (auto& v: {v1, v2, v3}) {
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

                        vertexData.push_back(0);
                        vertexData.push_back(0);

                        indices.push_back(index++);

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

                        vertexData.push_back(normals[ni].x());
                        vertexData.push_back(normals[ni].y());
                        vertexData.push_back(normals[ni].z());

                        vertexData.push_back(texCoords[ti].x());
                        vertexData.push_back(texCoords[ti].y());

                        indices.push_back(index++);

                    } else {
                        int vi;
                        std::stringstream(v) >> vi;
                        vi--;
                        vertexData.push_back(vertices[vi].x());
                        vertexData.push_back(vertices[vi].y());
                        vertexData.push_back(vertices[vi].z());

                        // TODO: calculate normals
                        vertexData.push_back(1);
                        vertexData.push_back(0);
                        vertexData.push_back(0);

                        vertexData.push_back(0);
                        vertexData.push_back(0);

                        indices.push_back(index++);

                    }
                }
            }
        }

        if (!mesh_name.empty()) {
            std::shared_ptr<gl::IDrawable> drawable = drawable_creator.createDrawable(
                vertexData, indices, std::vector<unsigned int>{3, 3, 2}, gl::VertexFormat::INTERLEAVED);
            auto sub_mesh = std::make_unique<SubMesh>(drawable, material);
            meshes.emplace_back();
            meshes.back().add_sub_mesh(std::move(sub_mesh));
        }

        return meshes;
    }
}

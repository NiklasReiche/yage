#include "gltf.h"
#include "gl3d/sceneGraph/sceneNode.h"
#include "gl3d/sceneGraph/sceneObject.h"
#include "gl3d/sceneGraph/sceneGroup.h"
#include <vector>
#include <utils/strings.h>
#include <gml/vector.h>

#include <utils/NotImplementedException.h>

#include <tiny_gltf.h>
#include <numeric>

namespace gl3d::resources
{
    gl::TextureWrapper convert_wrapper(int wrap)
    {
        switch (wrap) {
            case TINYGLTF_TEXTURE_WRAP_REPEAT:
                return gl::TextureWrapper::REPEAT;
            case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
                return gl::TextureWrapper::CLAMP_TO_EDGE;
            case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
                return gl::TextureWrapper::MIRRORED_REPEAT;
            default:
                throw std::invalid_argument("unknown TINYGLTF_TEXTURE_WRAP_* mode");
        }
    }

    gl::TextureFilter convert_filter(int filter)
    {
        switch (filter) {
            case TINYGLTF_TEXTURE_FILTER_LINEAR:
                return gl::TextureFilter::LINEAR;
            case TINYGLTF_TEXTURE_FILTER_NEAREST:
                return gl::TextureFilter::NEAREST;
            case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
                return gl::TextureFilter::LINEAR_MIPMAP_LINEAR;
            case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
                return gl::TextureFilter::LINEAR_MIPMAP_NEAREST;
            case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
                return gl::TextureFilter::NEAREST_MIPMAP_LINEAR;
            case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
                return gl::TextureFilter::NEAREST_MIPMAP_NEAREST;
            default:
                throw std::invalid_argument("unknown TINYGLTF_TEXTURE_FILTER_* mode");
        }
    }

    std::span<const std::byte> readBuffer(tinygltf::Model& model, int i, std::size_t offset, std::size_t size)
    {
        return std::as_bytes(std::span<unsigned char>{
                model.buffers[i].data.begin() + offset,
                model.buffers[i].data.begin() + offset + size
        });
    }

    std::span<const std::byte>
    readBufferView(tinygltf::Model& model, int i, std::size_t offset, std::size_t byte_length)
    {
        auto& bufferView = model.bufferViews[i];
        return readBuffer(model, bufferView.buffer, bufferView.byteOffset + offset, byte_length);
    }

    std::span<const std::byte> readAccessor(tinygltf::Model& model, tinygltf::Accessor& accessor)
    {
        auto type_byte_length = tinygltf::GetNumComponentsInType(accessor.type) *
                                tinygltf::GetComponentSizeInBytes(accessor.componentType);
        return readBufferView(model, accessor.bufferView, accessor.byteOffset, accessor.count * type_byte_length);
    }

    std::shared_ptr<gl::ITexture2D>
    read_texture(gl::ITextureCreator& textureCreator, tinygltf::Model& model, tinygltf::Texture& texture)
    {
        auto& image = model.images[texture.source];

        gl::ImageFormat format;
        switch (image.component) {
            case 1:
                format = gl::ImageFormat::R;
                break;
            case 2:
                format = gl::ImageFormat::RG;
                break;
            case 3:
                format = gl::ImageFormat::RGB;
                break;
            case 4:
                format = gl::ImageFormat::RGBA;
                break;
            default:
                throw std::invalid_argument("unsupported number of texture channels");
        }

        auto sampler = model.samplers[texture.sampler];
        gl::TextureWrapper wrapper_s = convert_wrapper(sampler.wrapS);
        gl::TextureWrapper wrapper_t = convert_wrapper(sampler.wrapT);
        gl::TextureFilter filter_min = convert_filter(sampler.minFilter);
        gl::TextureFilter filter_mag = convert_filter(sampler.magFilter);

        auto t = textureCreator.createTexture2D(image.width, image.height, format, image.image);
        t->configTextureWrapper(wrapper_s, wrapper_t);
        t->configTextureFilter(filter_min, filter_mag);
        if (t->requires_mipmaps()) {
            t->generateMipmaps();
        }
        return t;
    }

    std::shared_ptr<gl3d::Material>
    read_material(tinygltf::Model& model, tinygltf::Material& material,
                  const std::vector<std::shared_ptr<gl::ITexture2D>>& textures,
                  std::shared_ptr<gl::IShader>& shader,
                  const std::shared_ptr<gl::ITexture2D>& full_texture)
    {
        auto gl3d_material = std::make_shared<Material>();
        gl3d_material->add_uniform("albedo", gml::Vec3f(
                static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[0]),
                static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[1]),
                static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[2])));
        gl3d_material->add_uniform("metallic", static_cast<float>(material.pbrMetallicRoughness.metallicFactor));
        gl3d_material->add_uniform("roughness", static_cast<float>(material.pbrMetallicRoughness.roughnessFactor));
        gl3d_material->add_uniform("ao", 1.0f);

        if (material.pbrMetallicRoughness.baseColorTexture.index > -1) {
            gl3d_material->add_uniform("albedoMap",
                                       textures.at(material.pbrMetallicRoughness.baseColorTexture.index));
        } else {
            gl3d_material->add_uniform("albedoMap", full_texture);
        }

        if (material.pbrMetallicRoughness.metallicRoughnessTexture.index > -1) {
            gl3d_material->add_uniform("metallicRoughnessMap",
                                       textures.at(material.pbrMetallicRoughness.metallicRoughnessTexture.index));
        } else {
            gl3d_material->add_uniform("metallicRoughnessMap", full_texture);
        }

        if (material.normalTexture.index > -1) {
            gl3d_material->add_uniform("normalMap",
                                       textures.at(material.normalTexture.index));
        } else {
            gl3d_material->add_uniform("normalMap", full_texture);
        }

        if (material.occlusionTexture.index > -1) {
            gl3d_material->add_uniform("aoMap",
                                       textures.at(material.occlusionTexture.index));
        } else {
            gl3d_material->add_uniform("aoMap", full_texture);
        }

        gl3d_material->set_shader(shader);

        return gl3d_material;
    }

    std::unique_ptr<SubMesh> read_sub_mesh(tinygltf::Model& model, tinygltf::Primitive& primitive,
                                           gl::IDrawableCreator& drawableCreator,
                                           const std::vector<std::shared_ptr<gl3d::Material>>& materials,
                                           std::shared_ptr<gl::IShader>& shader,
                                           std::shared_ptr<gl::IShader>& shader_normal_mapping)
    {
        auto& gl3d_material = materials.at(primitive.material);

        std::vector<std::byte> vertices;
        std::vector<unsigned int> vertex_layout;

        auto position_accessor = model.accessors.at(primitive.attributes.at("POSITION"));
        if (position_accessor.type != TINYGLTF_TYPE_VEC3 || position_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
            throw std::runtime_error("unsupported format");
        }
        auto positions = readAccessor(model, position_accessor);
        vertices.insert(vertices.end(), positions.begin(), positions.end());
        vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(position_accessor.type)));

        // TODO: construct normals if not present
        auto normal_accessor = model.accessors.at(primitive.attributes.at("NORMAL"));
        if (normal_accessor.type != TINYGLTF_TYPE_VEC3 || normal_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
            throw std::runtime_error("unsupported format");
        }
        auto normals = readAccessor(model, normal_accessor);
        vertices.insert(vertices.end(), normals.begin(), normals.end());
        vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(normal_accessor.type)));

        if (primitive.attributes.contains("TANGENT")) {
            auto tangent_accessor = model.accessors.at(primitive.attributes.at("TANGENT"));
            if (tangent_accessor.type != TINYGLTF_TYPE_VEC4 ||
                    tangent_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
                throw std::runtime_error("unsupported format");
            }
            auto tangents = readAccessor(model, tangent_accessor);
            vertices.insert(vertices.end(), tangents.begin(), tangents.end());
            vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tangent_accessor.type)));
            gl3d_material->set_shader(shader_normal_mapping);
        } else {
            gl3d_material->set_shader(shader);
        }

        // TODO: fill in dummy tex coords if not present
        auto tex_coord_accessor = model.accessors.at(primitive.attributes.at("TEXCOORD_0")); // TODO
        if (tex_coord_accessor.type != TINYGLTF_TYPE_VEC2 ||
                tex_coord_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
            throw std::runtime_error("unsupported format");
        }
        auto tex_coords = readAccessor(model, tex_coord_accessor);
        vertices.insert(vertices.end(), tex_coords.begin(), tex_coords.end());
        vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tex_coord_accessor.type)));

        auto indices_accessor = model.accessors[primitive.indices];
        auto indices = readAccessor(model, indices_accessor);

        std::shared_ptr<gl::IDrawable> drawable = drawableCreator.createDrawable(
                vertices, indices, vertex_layout,
                indices.size() / tinygltf::GetComponentSizeInBytes(indices_accessor.componentType),
                gl::VertexFormat::BATCHED);


        return std::make_unique<SubMesh>(drawable, gl3d_material);
    }

    std::unique_ptr<Mesh> read_mesh(tinygltf::Model& model, tinygltf::Mesh& mesh,
                                    gl::IDrawableCreator& drawableCreator,
                                    const std::vector<std::shared_ptr<gl3d::Material>>& materials,
                                    std::shared_ptr <gl::IShader>& shader,
                                    std::shared_ptr<gl::IShader>& shader_normal_mapping)
    {
        auto gl3d_mesh = std::make_unique<Mesh>();
        for (auto& primitive: mesh.primitives) {
            gl3d_mesh->add_sub_mesh(read_sub_mesh(model, primitive, drawableCreator, materials, shader, shader_normal_mapping));
        }
        return gl3d_mesh;
    }

    std::shared_ptr<SceneGroup> read_node(tinygltf::Node& node, const std::vector<std::shared_ptr<gl3d::Mesh>>& meshes)
    {
        gml::Mat4d transform;
        if (node.matrix.size() == 16) {
            transform = gml::transpose(gml::Mat4d(std::span<double, 16>{node.matrix.begin(), node.matrix.end()}));
        } else {
            transform = gml::matrix::Id4d;
            if (node.scale.size() == 3) {
                transform *= gml::matrix::scale(node.scale.at(0), node.scale.at(1), node.scale.at(2));
            }
            if (node.rotation.size() == 4) {
                transform *= gml::matrix::fromQuaternion(
                        gml::Quatd(node.rotation.at(0), node.rotation.at(1),
                                   node.rotation.at(2), node.rotation.at(3)));
            }
            if (node.translation.size() == 3) {
                transform *= gml::matrix::translate(node.translation.at(0), node.translation.at(1),
                                                    node.translation.at(2));
            }
        }

        if (node.children.empty()) {
            auto object = std::make_shared<gl3d::SceneObject>(node.name, transform);
            if (node.mesh > -1) {
                object->bindMesh(meshes.at(node.mesh));
            }

            auto group = std::make_shared<gl3d::SceneGroup>(node.name + "_group");
            group->addChild(object);
            return group;
        } else {
            auto group = std::make_shared<gl3d::SceneGroup>(node.name, transform);
            if (node.mesh > -1) {
                auto object = std::make_shared<gl3d::SceneObject>(node.name + "_object");
                object->bindMesh(meshes.at(node.mesh));
                group->addChild(object);
            }

            return group;
        }
    }

    void construct_node(tinygltf::Model& model, tinygltf::Node& node,
                        const std::shared_ptr<gl3d::SceneGroup>& root,
                        const std::vector<std::shared_ptr<gl3d::SceneGroup>>& scene_nodes)
    {
        for (auto& child_index: node.children) {
            auto& child = scene_nodes.at(child_index);
            root->addChild(child);
            construct_node(model, model.nodes[child_index], child, scene_nodes);
        }
    }

    tinygltf::Model read_file(const platform::IFileReader& fileReader, const std::string& filename)
    {
        auto tmp = utils::strip(filename, "/");
        std::string filetype = utils::strip(tmp.back(), ".").back();
        tmp.pop_back();
        std::string basedir = utils::join(tmp, "/");
        basedir.pop_back();

        tinygltf::TinyGLTF loader;
        loader.SetPreserveImageChannels(true);

        tinygltf::Model model;
        std::string err;
        std::string warn;
        bool ret;
        if (filetype == "gltf") {
            std::string data = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ)->readAll();
            ret = loader.LoadASCIIFromString(&model, &err, &warn, data.data(), data.length(), basedir);
        } else if (filetype == "glb") {
            std::vector<std::byte> data = fileReader.openBinaryFile(filename, platform::IFile::AccessMode::READ)
                                                    ->read_all();
            ret = loader.LoadBinaryFromMemory(&model, &err, &warn, reinterpret_cast<unsigned char*>(data.data()),
                                              data.size(), basedir);
        } else {
            throw std::invalid_argument("invalid filetype for gltf");
        }

        if (!warn.empty()) {
            printf("Warn: %s\n", warn.c_str()); // TODO
        }
        if (!ret) {
            throw std::logic_error("TinyGLTF parsing error: " + err);
        }

        return model;
    }

    std::vector<std::shared_ptr<gl3d::Mesh>> read_meshes(tinygltf::Model& model,
                                                         gl::IDrawableCreator& drawableCreator,
                                                         gl::ITextureCreator& textureCreator,
                                                         std::shared_ptr<gl::IShader>& shader,
                                                         std::shared_ptr<gl::IShader>& shader_normal_mapping)
    {
        std::vector<unsigned char> full_data{255, 255, 255, 255};
        std::shared_ptr<gl::ITexture2D> full_texture = textureCreator.createTexture2D(
                1, 1, gl::ImageFormat::RGBA, full_data);

        std::vector<std::shared_ptr<gl::ITexture2D>> textures;
        textures.reserve(model.textures.size());
        for (auto& texture: model.textures) {
            textures.push_back(read_texture(textureCreator, model, texture));
        }

        std::vector<std::shared_ptr<gl3d::Material>> materials;
        materials.reserve(model.materials.size());
        for (auto& material: model.materials) {
            materials.push_back(read_material(model, material, textures, shader, full_texture));
        }

        std::vector<std::shared_ptr<gl3d::Mesh>> meshes;
        meshes.reserve(model.meshes.size());
        for (auto& mesh: model.meshes) {
            meshes.push_back(read_mesh(model, mesh, drawableCreator, materials, shader, shader_normal_mapping));
        }

        return meshes;
    }

    std::shared_ptr<SceneGroup> gltf_read_scene(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            std::shared_ptr<gl::IShader>& shader,
            std::shared_ptr<gl::IShader>& shader_normal_mapping)
    {
        tinygltf::Model model = read_file(fileReader, filename);

        auto meshes = read_meshes(model, drawableCreator, textureCreator, shader, shader_normal_mapping);

        std::vector<std::shared_ptr<gl3d::SceneGroup>> scene_nodes;
        scene_nodes.reserve(model.nodes.size());
        // TODO: camera
        for (auto& node: model.nodes) {
            scene_nodes.push_back(read_node(node, meshes));
        }

        std::vector<std::shared_ptr<gl3d::SceneGroup>> scenes;
        scenes.reserve(model.scenes.size());
        for (auto& scene: model.scenes) {
            auto world_root = std::make_shared<gl3d::SceneGroup>("root");
            for (auto root_index : scene.nodes) {
                auto root = scene_nodes.at(root_index);
                world_root->addChild(root);
                construct_node(model, model.nodes.at(root_index), root, scene_nodes);
            }
            scenes.push_back(world_root);
        }

        // TODO: support multiple scenes
        return scenes.at(model.defaultScene);

        // TODO: do we need to handle sparse accessors explicitly?
    }

    std::vector<std::shared_ptr<gl3d::Mesh>> gltf_read_meshes(const platform::IFileReader& fileReader, const std::string& filename,
                                             gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
                                             std::shared_ptr<gl::IShader>& shader,
                                             std::shared_ptr<gl::IShader>& shader_normal_mapping)
    {
        tinygltf::Model model = read_file(fileReader, filename);
        return read_meshes(model, drawableCreator, textureCreator, shader, shader_normal_mapping);
    }
}

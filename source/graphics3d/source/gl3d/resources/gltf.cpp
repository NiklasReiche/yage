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
                  std::shared_ptr<gl::IShader>& shader)
    {
        // TODO: use a 1x1 dummy texture or disable tex_coords in shader, if there's no textures

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
        }
        if (material.pbrMetallicRoughness.metallicRoughnessTexture.index > -1) {
            gl3d_material->add_uniform("aoMap",
                                       textures.at(material.pbrMetallicRoughness.metallicRoughnessTexture.index));
        }
        if (material.normalTexture.index > -1) {
            gl3d_material->add_uniform("normalMap",
                                       textures.at(material.normalTexture.index));
        }
        if (material.occlusionTexture.index > -1) {
            gl3d_material->add_uniform("metallicRoughnessMap",
                                       textures.at(material.occlusionTexture.index));
        }
        gl3d_material->set_shader(shader);

        return gl3d_material;
    }

    std::unique_ptr<SubMesh> read_sub_mesh(tinygltf::Model& model, tinygltf::Primitive& primitive,
                                           gl::IDrawableCreator& drawableCreator,
                                           const std::vector<std::shared_ptr<gl3d::Material>>& materials)
    {
        // TODO: fallback when attributes are not present
        auto position_accessor = model.accessors[primitive.attributes["POSITION"]];
        auto normal_accessor = model.accessors[primitive.attributes["NORMAL"]];
        auto tangent_accessor = model.accessors[primitive.attributes["TANGENT"]];
        auto tex_coord_accessor = model.accessors[primitive.attributes["TEXCOORD_0"]]; // TODO

        auto positions = readAccessor(model, position_accessor);
        auto normals = readAccessor(model, normal_accessor);
        auto tangents = readAccessor(model, tangent_accessor);
        auto tex_coords = readAccessor(model, tex_coord_accessor);

        std::vector<std::byte> vertices;
        vertices.insert(vertices.end(), positions.begin(), positions.end());
        vertices.insert(vertices.end(), normals.begin(), normals.end());
        vertices.insert(vertices.end(), tangents.begin(), tangents.end());
        vertices.insert(vertices.end(), tex_coords.begin(), tex_coords.end());

        auto indices_accessor = model.accessors[primitive.indices];
        auto indices = readAccessor(model, indices_accessor);

        const std::vector<unsigned int> vertex_layout{
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(position_accessor.type)),
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(normal_accessor.type)),
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tangent_accessor.type)),
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tex_coord_accessor.type))
        };
        std::shared_ptr<gl::IDrawable> drawable = drawableCreator.createDrawable(
                vertices, indices, vertex_layout,
                indices.size() / tinygltf::GetComponentSizeInBytes(indices_accessor.componentType),
                gl::VertexFormat::BATCHED);

        auto& gl3d_material = materials[primitive.material];
        return std::make_unique<SubMesh>(drawable, gl3d_material);
    }

    std::unique_ptr<Mesh> read_mesh(tinygltf::Model& model, tinygltf::Mesh& mesh,
                                    gl::IDrawableCreator& drawableCreator,
                                    const std::vector<std::shared_ptr<gl3d::Material>>& materials)
    {
        auto gl3d_mesh = std::make_unique<Mesh>();
        for (auto& primitive: mesh.primitives) {
            gl3d_mesh->add_sub_mesh(read_sub_mesh(model, primitive, drawableCreator, materials));
        }
        return gl3d_mesh;
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

    std::shared_ptr<SceneGroup> readGltf(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            std::shared_ptr<gl::IShader>& shader)
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
            // TODO
            //fileReader.openBinaryFile(filename, platform::IFile::AccessMode::READ);
            //loader.LoadBinaryFromMemory();
            throw NotImplementedException();
        } else {
            throw std::invalid_argument("invalid filetype for gltf");
        }

        if (!warn.empty()) {
            printf("Warn: %s\n", warn.c_str()); // TODO
        }
        if (!ret) {
            throw std::logic_error("TinyGLTF parsing error: " + err);
        }

        std::vector<std::shared_ptr<gl::ITexture2D>> textures;
        for (auto& texture: model.textures) {
            textures.push_back(read_texture(textureCreator, model, texture));
        }

        std::vector<std::shared_ptr<gl3d::Material>> materials;
        for (auto& material: model.materials) {
            materials.push_back(read_material(model, material, textures, shader));
        }

        std::vector<std::shared_ptr<gl3d::Mesh>> meshes;
        for (auto& mesh: model.meshes) {
            meshes.push_back(read_mesh(model, mesh, drawableCreator, materials));
        }

        std::vector<std::shared_ptr<gl3d::SceneGroup>> scene_nodes;
        // TODO: camera
        for (auto& node: model.nodes) {
            gml::Mat4d transform = gml::matrix::Id4d;
            if (node.matrix.size() == 16) {
                transform = gml::Mat4d(std::span<double, 16>{node.matrix.begin(), node.matrix.end()});
            }

            if (node.children.empty()) {
                auto object = std::make_shared<gl3d::SceneObject>(node.name, transform);
                if (node.mesh > -1) {
                    object->bindMesh(meshes.at(node.mesh));
                }

                auto group = std::make_shared<gl3d::SceneGroup>(node.name + "_group");
                group->addChild(object);
                scene_nodes.push_back(group);
            } else {
                auto group = std::make_shared<gl3d::SceneGroup>(node.name, transform);
                scene_nodes.push_back(group);

                if (node.mesh > -1) {
                    auto object = std::make_shared<gl3d::SceneObject>(node.name + "_object");
                    object->bindMesh(meshes.at(node.mesh));
                    group->addChild(object);
                }
            }
        }

        std::vector<std::shared_ptr<gl3d::SceneGroup>> scenes;
        for (auto& scene: model.scenes) {
            auto world_root = std::make_shared<gl3d::SceneGroup>("root");
            for (auto root_index : scene.nodes) {
                construct_node(model, model.nodes[root_index], world_root, scene_nodes);
            }
            scenes.push_back(world_root);
        }

        // TODO: support multiple scenes
        return scenes.at(model.defaultScene);

        // TODO: do we need to handle sparse accessors explicitly?
    }
}

#include "gltf.h"
#include "gl3d/sceneGraph/sceneNode.h"
#include "gl3d/sceneGraph/sceneObject.h"
#include "gl3d/sceneGraph/sceneGroup.h"
#include <vector>
#include <utils/strings.h>
#include <math/vector.h>

#include <utils/NotImplementedException.h>

#include <tiny_gltf.h>
#include <numeric>

namespace yage::gl3d::resources
{
    gl::TextureWrapper convert_wrapper(const int wrap)
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

    gl::TextureFilter convert_filter(const int filter)
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

    std::span<const std::byte> readBuffer(tinygltf::Model& model, const int i, const std::size_t offset, const std::size_t size)
    {
        return std::as_bytes(std::span{
                model.buffers[i].data.begin() + offset,
                model.buffers[i].data.begin() + offset + size
        });
    }

    std::span<const std::byte>
    readBufferView(tinygltf::Model& model, const int i, const std::size_t offset, const std::size_t byte_length)
    {
        const auto& bufferView = model.bufferViews[i];
        return readBuffer(model, bufferView.buffer, bufferView.byteOffset + offset, byte_length);
    }

    std::span<const std::byte> readAccessor(tinygltf::Model& model, const tinygltf::Accessor& accessor)
    {
        const auto type_byte_length = tinygltf::GetNumComponentsInType(accessor.type) *
                                      tinygltf::GetComponentSizeInBytes(accessor.componentType);
        return readBufferView(model, accessor.bufferView, accessor.byteOffset, accessor.count * type_byte_length);
    }

    std::shared_ptr<gl::ITexture2D>
    read_texture(gl::ITextureCreator& textureCreator, tinygltf::Model& model, const tinygltf::Texture& texture)
    {
        auto& image = model.images[texture.source];

        // TODO: consider image bit depth
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

        const auto sampler = model.samplers[texture.sampler];
        const gl::TextureWrapper wrapper_s = convert_wrapper(sampler.wrapS);
        const gl::TextureWrapper wrapper_t = convert_wrapper(sampler.wrapT);
        const gl::TextureFilter filter_min = convert_filter(sampler.minFilter);
        const gl::TextureFilter filter_mag = convert_filter(sampler.magFilter);

        auto t = textureCreator.createTexture2D(image.width, image.height, format, image.image);
        t->configTextureWrapper(wrapper_s, wrapper_t);
        t->configTextureFilter(filter_min, filter_mag);
        if (t->requires_mipmaps()) {
            t->generateMipmaps();
        }
        return t;
    }

    std::shared_ptr<Material>
    read_material(const tinygltf::Material& material,
                  const std::vector<std::shared_ptr<gl::ITexture2D>>& textures,
                  const std::shared_ptr<gl::ITexture2D>& full_texture)
    {
        auto gl3d_material = std::make_shared<Material>();
        gl3d_material->add_uniform("albedo", math::Vec3f(
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

        return gl3d_material;
    }

    std::unique_ptr<SubMesh> read_sub_mesh(tinygltf::Model& model, const tinygltf::Primitive& primitive,
                                           gl::IDrawableCreator& drawableCreator,
                                           const std::vector<std::shared_ptr<Material>>& materials,
                                           const ShaderMap& shaders)
    {
        auto& gl3d_material = materials.at(primitive.material);

        std::vector<std::byte> vertices;
        std::vector<unsigned int> vertex_layout;

        const auto position_accessor = model.accessors.at(primitive.attributes.at("POSITION"));
        if (position_accessor.type != TINYGLTF_TYPE_VEC3 || position_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
            throw std::runtime_error("unsupported format");
        }
        auto positions = readAccessor(model, position_accessor);
        vertices.insert(vertices.end(), positions.begin(), positions.end());
        vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(position_accessor.type)));

        // TODO: construct normals if not present
        const auto normal_accessor = model.accessors.at(primitive.attributes.at("NORMAL"));
        if (normal_accessor.type != TINYGLTF_TYPE_VEC3 || normal_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
            throw std::runtime_error("unsupported format");
        }
        auto normals = readAccessor(model, normal_accessor);
        vertices.insert(vertices.end(), normals.begin(), normals.end());
        vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(normal_accessor.type)));

        if (primitive.attributes.contains("TANGENT")) {
            const auto tangent_accessor = model.accessors.at(primitive.attributes.at("TANGENT"));
            if (tangent_accessor.type != TINYGLTF_TYPE_VEC4 ||
                    tangent_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
                throw std::runtime_error("unsupported format");
            }
            auto tangents = readAccessor(model, tangent_accessor);
            vertices.insert(vertices.end(), tangents.begin(), tangents.end());
            vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tangent_accessor.type)));
            gl3d_material->set_shader(shaders.at(ShaderPermutation::PBR_NORMAL_MAP));
        } else {
            gl3d_material->set_shader(shaders.at(ShaderPermutation::PBR));
        }

        // TODO: fill in dummy tex coords if not present
        const auto tex_coord_accessor = model.accessors.at(primitive.attributes.at("TEXCOORD_0")); // TODO
        if (tex_coord_accessor.type != TINYGLTF_TYPE_VEC2 ||
                tex_coord_accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
            throw std::runtime_error("unsupported format");
        }
        auto tex_coords = readAccessor(model, tex_coord_accessor);
        vertices.insert(vertices.end(), tex_coords.begin(), tex_coords.end());
        vertex_layout.push_back(static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tex_coord_accessor.type)));

        const auto indices_accessor = model.accessors[primitive.indices];
        const auto indices = readAccessor(model, indices_accessor);

        std::shared_ptr<gl::IDrawable> drawable = drawableCreator.createDrawable(
                vertices, indices, vertex_layout,
                indices.size() / tinygltf::GetComponentSizeInBytes(indices_accessor.componentType),
                gl::VertexFormat::BATCHED);


        return std::make_unique<SubMesh>(drawable, gl3d_material);
    }

    Mesh read_mesh(tinygltf::Model& model, tinygltf::Mesh& mesh,
                                    gl::IDrawableCreator& drawableCreator,
                                    const std::vector<std::shared_ptr<Material>>& materials,
                                    const ShaderMap& shaders)
    {
        Mesh gl3d_mesh;
        for (auto& primitive: mesh.primitives) {
            gl3d_mesh.add_sub_mesh(read_sub_mesh(model, primitive, drawableCreator, materials, shaders));
        }
        return gl3d_mesh;
    }

    std::unique_ptr<SceneGroup> read_node(tinygltf::Node& node, const std::vector<std::shared_ptr<Mesh>>&)
    {
        math::Mat4d transform;
        if (node.matrix.size() == 16) {
            transform = math::transpose(math::Mat4d(std::span<double, 16>{node.matrix.begin(), node.matrix.end()}));
        } else {
            transform = math::matrix::Id4d;
            if (node.scale.size() == 3) {
                transform *= math::matrix::scale(node.scale.at(0), node.scale.at(1), node.scale.at(2));
            }
            if (node.rotation.size() == 4) {
                transform *= math::matrix::from_quaternion(
                        math::Quatd(node.rotation.at(0), node.rotation.at(1),
                                   node.rotation.at(2), node.rotation.at(3)));
            }
            if (node.translation.size() == 3) {
                transform *= math::matrix::translate(node.translation.at(0), node.translation.at(1),
                                                    node.translation.at(2));
            }
        }

        if (node.children.empty()) {
            std::unique_ptr<SceneGroup> group = std::make_unique<gl3d::SceneGroup>(node.name + "_group");
            //SceneObject& object = group->create_object(node.name, transform);
            if (node.mesh > -1) {
                //object.mesh = meshes.at(node.mesh); TODO
            }

            return group;
        } else {
            std::unique_ptr<SceneGroup> group = std::make_unique<gl3d::SceneGroup>(node.name, transform);
            if (node.mesh > -1) {
                //SceneObject& object = group->create_object(node.name + "_object");
                //object.mesh = meshes.at(node.mesh); TODO
            }

            return group;
        }
    }

    void construct_node(tinygltf::Model& model, tinygltf::Node& node,
                        const std::unique_ptr<gl3d::SceneGroup>& root,
                        std::vector<std::unique_ptr<gl3d::SceneGroup>>& scene_nodes)
    {
        for (const auto& child_index: node.children) {
            auto& child = scene_nodes.at(child_index);
            construct_node(model, model.nodes[child_index], child, scene_nodes);
            root->add_node(std::move(child));
        }
    }

    tinygltf::Model read_file(const platform::IFileReader& fileReader, const std::string& filename)
    {
        auto tmp = utils::strip(filename, "/");
        const std::string filetype = utils::strip(tmp.back(), ".").back();
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
            const std::string data = fileReader.openTextFile(filename, platform::IFile::AccessMode::READ)->readAll();
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

    std::vector<Mesh> read_meshes(tinygltf::Model& model,
        gl::IDrawableCreator& drawableCreator,
        gl::ITextureCreator& textureCreator,
        const ShaderMap& shaders)
    {
        std::vector<unsigned char> full_data{255, 255, 255, 255};
        const std::shared_ptr<gl::ITexture2D> full_texture = textureCreator.createTexture2D(
                1, 1, gl::ImageFormat::RGBA, full_data);

        std::vector<std::shared_ptr<gl::ITexture2D>> textures;
        textures.reserve(model.textures.size());
        for (auto& texture: model.textures) {
            textures.push_back(read_texture(textureCreator, model, texture));
        }

        std::vector<std::shared_ptr<Material>> materials;
        materials.reserve(model.materials.size());
        for (auto& material: model.materials) {
            materials.push_back(read_material(material, textures, full_texture));
        }

        std::vector<Mesh> meshes;
        meshes.reserve(model.meshes.size());
        for (auto& mesh: model.meshes) {
            meshes.push_back(read_mesh(model, mesh, drawableCreator, materials, shaders));
        }

        return meshes;
    }

    std::unique_ptr<SceneGroup> gltf_read_scene(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            const ShaderMap& shaders)
    {
        tinygltf::Model model = read_file(fileReader, filename);

        std::vector<Mesh> meshes = read_meshes(model, drawableCreator, textureCreator, shaders);

        std::vector<std::unique_ptr<gl3d::SceneGroup>> scene_nodes;
        scene_nodes.reserve(model.nodes.size());
        // TODO: camera
        //for (auto& node: model.nodes) {
            //scene_nodes.push_back(read_node(node, meshes));
        //}

        std::vector<std::unique_ptr<gl3d::SceneGroup>> scenes;
        scenes.reserve(model.scenes.size());
        for (auto& scene: model.scenes) {
            auto world_root = std::make_unique<gl3d::SceneGroup>("root");
            for (const auto root_index : scene.nodes) {
                auto& root = scene_nodes.at(root_index);
                construct_node(model, model.nodes.at(root_index), root, scene_nodes);
                world_root->add_node(std::move(root));
            }
            scenes.push_back(std::move(world_root));
        }

        // TODO: support multiple scenes
        return std::move(scenes.at(model.defaultScene));

        // TODO: do we need to handle sparse accessors explicitly?
    }

    std::vector<Mesh> gltf_read_meshes(const platform::IFileReader& fileReader, const std::string& filename,
                                             gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
                                             const ShaderMap& shaders)
    {
        tinygltf::Model model = read_file(fileReader, filename);
        return read_meshes(model, drawableCreator, textureCreator, shaders);
    }
}

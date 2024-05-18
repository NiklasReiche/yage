#include "gltf.h"
#include <vector>
#include <utils/strings.h>
#include <gml/vector.h>

#include <utils/NotImplementedException.h>

#include <tiny_gltf.h>
#include <numeric>

namespace gl3d::resources
{
    typedef unsigned char byte;

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

    std::span<byte> readBuffer(tinygltf::Model& model, int i, std::size_t offset, std::size_t size)
    {
        return {model.buffers[i].data.begin() + offset, model.buffers[i].data.begin() + offset + size};
    }

    std::span<byte> readBufferView(tinygltf::Model& model, int i, std::size_t offset, std::size_t byte_length)
    {
        auto& bufferView = model.bufferViews[i];
        return readBuffer(model, bufferView.buffer, bufferView.byteOffset + offset, byte_length);
    }

    std::span<byte> readAccessor(tinygltf::Model& model, tinygltf::Accessor& accessor)
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

    std::unique_ptr<SubMesh> read_sub_mesh(tinygltf::Model& model, tinygltf::Primitive& primitive,
                                           gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator)
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

        std::vector<byte> vertices;
        vertices.insert(vertices.end(), positions.begin(), positions.end());
        vertices.insert(vertices.end(), normals.begin(), normals.end());
        vertices.insert(vertices.end(), tangents.begin(), tangents.end());
        vertices.insert(vertices.end(), tex_coords.begin(), tex_coords.end());

        auto indices_accessor = model.accessors[primitive.indices];
        auto indices = readAccessor(model, indices_accessor);

        const std::vector<unsigned int>& vertexLayout{
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(position_accessor.type)),
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(normal_accessor.type)),
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tangent_accessor.type)),
                static_cast<unsigned int>(tinygltf::GetNumComponentsInType(tex_coord_accessor.type))
        };
        std::shared_ptr<gl::IDrawable> drawable = drawableCreator.createDrawable(vertices, indices, vertexLayout,
                                                       indices.size() / tinygltf::GetComponentSizeInBytes(
                                                               indices_accessor.componentType),
                                                       gl::VertexFormat::BATCHED);

        auto& material = model.materials[primitive.material];

        // TODO: use a 1x1 dummy texture or disable tex_coords in shader, if there's no textures
        auto base_texture = read_texture(textureCreator, model,
                                         model.textures[material.pbrMetallicRoughness.baseColorTexture.index]);
        auto normal_texture = read_texture(textureCreator, model, model.textures[material.normalTexture.index]);
        auto occlusion_texture = read_texture(textureCreator, model, model.textures[material.occlusionTexture.index]);
        auto metallic_roughness_texture = read_texture(textureCreator, model,
                                                       model.textures[material.pbrMetallicRoughness
                                                                              .metallicRoughnessTexture.index]);

        auto gl3d_material = std::make_shared<Material>();
        gl3d_material->addVec3("albedo", gml::Vec3f(static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[0]),
                                                    static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[1]),
                                                    static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[2])));
        gl3d_material->addFloat("metallic", static_cast<float>(material.pbrMetallicRoughness.metallicFactor));
        gl3d_material->addFloat("roughness", static_cast<float>(material.pbrMetallicRoughness.roughnessFactor));
        gl3d_material->addFloat("ao", 1);

        gl3d_material->addTexture("albedoMap", base_texture);
        gl3d_material->addTexture("normalMap", normal_texture);
        gl3d_material->addTexture("metallicRoughnessMap", metallic_roughness_texture);
        gl3d_material->addTexture("aoMap", occlusion_texture);

        return std::make_unique<SubMesh>(drawable, gl3d_material);
    }

    std::unique_ptr<Mesh> read_mesh(tinygltf::Model& model, tinygltf::Mesh& mesh,
                                    gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator)
    {
        auto gl3d_mesh = std::make_unique<Mesh>();
        for (auto& primitive : mesh.primitives) {
            gl3d_mesh->add_sub_mesh(read_sub_mesh(model, primitive, drawableCreator, textureCreator));
        }
        return gl3d_mesh;
    }

    std::unique_ptr<Mesh> readGltf(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator)
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

        // TODO: we currently only support single models
        return read_mesh(model, model.meshes[0], drawableCreator, textureCreator);

        // TODO: do we need to handle sparse accessors explicitly?
    }
}

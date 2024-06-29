#include "MeshLoader.h"
#include <utility>
#include <utils/strings.h>
#include "resources/gltf.h"

namespace yage::gl3d
{
    MeshFileLoader::MeshFileLoader(const std::shared_ptr<platform::IFileReader>& file_reader,
                                   const std::shared_ptr<gl::ITextureCreator>& texture_creator,
                                   const std::shared_ptr<gl::IDrawableCreator>& drawable_creator,
                                   ShaderMap shaders) noexcept
        : m_file_reader(file_reader),
          m_texture_creator(texture_creator),
          m_drawable_creator(drawable_creator),
          m_shaders(std::move(shaders))
    {
    }

    Mesh MeshFileLoader::load_resource(const std::string& uri)
    {
        const std::vector<std::string>& uri_parts = utils::strip(uri, ":");
        const std::vector<std::string>& path_parts = utils::strip(uri_parts[0], ".");
        if (path_parts.size() < 2) {
            throw std::invalid_argument("Missing file extension in uri '" + uri + "'");
        }
        if (path_parts[1] != "glb" && path_parts[1] != "gltf") {
            throw std::invalid_argument("Unsupported file type in uri '" + uri + "'");
        }

        int mesh_index = 0;
        if (uri_parts.size() > 1) {
            mesh_index = std::stoi(uri_parts[1]);
        }

        return std::move(resources::gltf_read_meshes(*m_file_reader, uri_parts[0], *m_drawable_creator,
                                                     *m_texture_creator,
                                                     m_shaders).at(mesh_index));
    }

    void MeshFileLoader::load_archive(const std::string& uri, std::vector<Mesh>& resources,
                                      std::vector<std::string>& uris)
    {
        const std::vector<std::string>& path_parts = utils::strip(uri, ".");
        if (path_parts.size() < 2) {
            throw std::invalid_argument("Missing file extension in uri '" + uri + "'");
        }
        if (path_parts[1] != "glb" && path_parts[1] != "gltf") {
            throw std::invalid_argument("Unsupported file type in uri '" + uri + "'");
        }

        resources = resources::gltf_read_meshes(*m_file_reader, uri, *m_drawable_creator, *m_texture_creator,
                                                m_shaders);

        uris.reserve(resources.size());
        for (std::size_t i = 0; i < resources.size(); ++i) {
            uris.push_back(uri + ":" + std::to_string(i));
        }
    }
}

#include "MeshLoader.h"
#include "resources/gltf.h"
#include <utility>

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

    std::unique_ptr<Mesh> MeshFileLoader::load(const std::string location)
    {
        return std::move(resources::gltf_read_meshes(*m_file_reader, location, *m_drawable_creator, *m_texture_creator, m_shaders).at(0));
    }
}

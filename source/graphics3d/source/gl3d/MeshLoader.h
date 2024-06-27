#pragma once

#include <core/platform/IFileReader.h>
#include <resource/Loader.h>

#include "mesh.h"
#include "shaders.h"

namespace yage::gl3d
{
    class MeshFileLoader final : public res::Loader<std::unique_ptr<Mesh>, std::string>
    {
    public:
        explicit MeshFileLoader(const std::shared_ptr<platform::IFileReader>& file_reader,
                                const std::shared_ptr<gl::ITextureCreator>& texture_creator,
                                const std::shared_ptr<gl::IDrawableCreator>& drawable_creator,
                                ShaderMap shaders) noexcept;

        std::unique_ptr<Mesh> load(std::string location) override;

    private:
        std::shared_ptr<platform::IFileReader> m_file_reader;
        std::shared_ptr<gl::ITextureCreator> m_texture_creator;
        std::shared_ptr<gl::IDrawableCreator> m_drawable_creator;
        ShaderMap m_shaders;
    };
}

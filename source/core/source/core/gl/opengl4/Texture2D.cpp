#include "Texture2D.h"
#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    Texture2D::Texture2D(Context* context, const TextureSampler& sampler, const PixelTransferInfo& data_info,
                         const std::span<const std::byte> data)
        : m_context(context)
    {
        glGenTextures(1, &m_texture_handle);

        m_context->bind_texture(GL_TEXTURE_2D, m_texture_handle);

        // unpack alignment must be 1 to replicate vulkan behaviour
        m_context->set_pixel_store_param(GL_UNPACK_ALIGNMENT, 1);

        const auto internal_format = static_cast<GLint>(to_internal_format(data_info.image_format));
        // we assume the client data to be in the same format as the internal format to replicate vulkan behaviour
        const GLenum format = to_client_format(data_info.image_format);
        const GLenum type = to_client_pixel_type(data_info.image_format);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, static_cast<GLsizei>(data_info.width),
                     static_cast<GLsizei>(data_info.height), 0, format, type, data.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(convert(sampler.u_wrapper)));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(convert(sampler.u_wrapper)));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        static_cast<GLint>(convert(sampler.min_filter, sampler.mip_map_mode)));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        static_cast<GLint>(convert(sampler.mag_filter, sampler.mip_map_mode)));
    }

    Texture2D::Texture2D(Context* context, const MSAASamples samples, const PixelTransferInfo& data_info)
        : m_context(context)
    {
        glGenTextures(1, &m_texture_handle);

        m_context->bind_texture(GL_TEXTURE_2D_MULTISAMPLE, m_texture_handle);

        const auto internal_format = static_cast<GLint>(to_internal_format(data_info.image_format));
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLsizei>(n_samples(samples)), internal_format,
                                static_cast<GLsizei>(data_info.width), static_cast<GLsizei>(data_info.height), GL_TRUE);

        // we have no sampler parameters for multusampled textures, since they cannot be sampled in shaders
    }

    Texture2D::~Texture2D()
    {
        clear();
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept
        : m_context(other.m_context),
          m_texture_handle(other.m_texture_handle)
    {
        other.m_context = nullptr;
        other.m_texture_handle = 0;
    }

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_texture_handle = other.m_texture_handle;

        other.m_context = nullptr;
        other.m_texture_handle = 0;

        return *this;
    }

    GLuint Texture2D::gl_handle() const
    {
        return m_texture_handle;
    }

    void Texture2D::clear()
    {
        if (m_context != nullptr) {
            glDeleteTextures(1, &m_texture_handle); // TODO: check that m_texture_handle doesn't get reset to zero
            m_context->unbind_texture(GL_TEXTURE_2D, m_texture_handle);
        }
    }
}

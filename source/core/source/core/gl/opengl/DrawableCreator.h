#pragma once

#include "../DrawableCreator.h"

#include "BaseObject.h"
#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace opengl
{
    class DrawableCreator final : public BaseObject, public gl::IDrawableCreator
    {
    public:
        std::unique_ptr<gl::IDrawable> createDrawable(
                const std::span<const float>& vertices,
                const std::span<const unsigned int>& indices,
                const std::span<const unsigned int>& vertex_layout,
                gl::VertexFormat format) override;

        std::unique_ptr<gl::IDrawable> createDrawable(
                const std::span<const std::byte>& vertices,
                const std::span<const std::byte>& indices,
                const std::span<const unsigned int>& vertex_layout,
                unsigned int n_indices,
                gl::VertexFormat format) override;

        std::unique_ptr<gl::ElementBuffer> createElementBuffer(const std::span<const unsigned int>& indices) override;

        std::unique_ptr<gl::ElementBuffer> createElementBuffer(const std::span<const std::byte>& indices) override;

        std::unique_ptr<gl::VertexBuffer> createVertexBuffer(const std::span<const float>& vertices) override;

        std::unique_ptr<gl::VertexBuffer> createVertexBuffer(const std::span<const std::byte>& vertices) override;

        std::unique_ptr<gl::VertexArray> createVertexArray(
                const std::shared_ptr<gl::VertexBuffer>& vertex_buffer,
                const std::shared_ptr<gl::ElementBuffer>& element_buffer,
                const std::span<const unsigned int>& vertex_layout,
                unsigned int n_vertices,
                gl::VertexFormat format) override;

    private:
        using BaseObject::BaseObject;

        std::unique_ptr<VertexArray> createVertexArrayInternal(
                const std::shared_ptr<gl::VertexBuffer>& vertex_buffer,
                const std::shared_ptr<gl::ElementBuffer>& element_buffer,
                const std::span<const unsigned int>& vertex_layout,
                unsigned int n_vertices,
                gl::VertexFormat format);

        friend class Context;
    };
}

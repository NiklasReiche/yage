#pragma once

#include <unordered_map>
#include <memory>

#include <glad/gl.h>

#include "../Handle.h"
#include "../IContext.h"
#include "DescriptorSet.h"
#include "DescriptorSetCreator.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSetLayoutBuilder.h"
#include "Drawable.h"
#include "DrawableCreator.h"
#include "IndexBuffer.h"
#include "IndexBufferCreator.h"
#include "RenderTarget.h"
#include "RenderTargetBuilder.h"
#include "Texture2D.h"
#include "Texture2DCreator.h"
#include "UniformBuffer.h"
#include "UniformBufferCreator.h"
#include "VertexBuffer.h"
#include "VertexBufferCreator.h"
#include "Pipeline.h"
#include "PipelineBuilder.h"
#include "Renderer.h"

namespace yage::gl::opengl4
{
    using UniformBufferStore = Store<IUniformBuffer, UniformBuffer>;
    using VertexBufferStore = Store<IVertexBuffer, VertexBuffer>;
    using IndexBufferStore = Store<IIndexBuffer, IndexBuffer>;
    using FrameBufferStore = Store<IRenderTarget, RenderTarget>;
    using Texture2DStore = Store<ITexture2D2, Texture2D>;
    using DrawableStore = Store<IDrawable2, Drawable>;
    using DescriptorSetStore = Store<IDescriptorSet, DescriptorSet>;
    using DescriptorSetLayoutStore = Store<IDescriptorSetLayout, DescriptorSetLayout>;
    using PipelineStore = Store<IPipeline, Pipeline>;
    using RenderTargetStore = Store<IRenderTarget, RenderTarget>;

    class Context final : public IContext
    {
    public:
        Context();

        IVertexBufferCreator& vertex_buffer_creator() override;
        IIndexBufferCreator& index_buffer_creator() override;
        IUniformBufferCreator& uniform_buffer_creator() override;
        IDrawableCreator2& drawable_creator() override;
        ITexture2DCreator& texture_2d_creator() override;
        IDescriptorSetCreator& descriptor_set_creator() override;
        IDescriptorSetLayoutBuilder& descriptor_set_layout_builder() override;
        IPipelineBuilder& pipeline_builder() override;
        IRenderTargetBuilder& render_target_builder() override;
        IRenderer2& renderer() override;

        VertexBufferStore& store_vertex_buffers();
        IndexBufferStore& store_index_buffers();
        UniformBufferStore& store_uniform_buffers();
        DrawableStore& store_drawables();
        Texture2DStore& store_texture2ds();
        DescriptorSetStore& store_descriptor_sets();
        DescriptorSetLayoutStore& store_descriptor_set_layouts();
        PipelineStore& store_pipelines();
        RenderTargetStore& store_render_targets();

        void bind_buffer(GLenum target, GLuint buffer);

        void force_bind_buffer(GLenum target, GLuint buffer);

        void unbind_buffer(GLenum target, GLuint buffer);

        void bind_texture(GLenum target, GLuint texture);

        void force_bind_texture(GLenum target, GLuint texture);

        void unbind_texture(GLenum target, GLuint texture);

        void bind_vao(GLuint vao);

        void unbind_vao(GLuint vao);

        void bind_frame_buffer(GLenum target, GLuint frame_buffer);

        void force_bind_frame_buffer(GLenum target, GLuint frame_buffer);

        void unbind_frame_buffer(GLenum target, GLuint frame_buffer);

        void bind_render_buffer(GLuint render_buffer);

        void force_bind_render_buffer(GLuint render_buffer);

        void unbind_render_buffer(GLuint render_buffer);

        void set_pixel_store_param(GLenum param, GLint value);

    private:
        struct OpenGlState
        {
            std::unordered_map<GLenum, GLuint> bound_buffer_per_target;
            std::unordered_map<GLenum, GLuint> bound_texture_per_target;
            std::unordered_map<GLenum, GLuint> bound_framebuffers;
            GLuint bound_vao;
            GLuint bound_render_buffer;

            std::unordered_map<GLenum, GLint> pixel_store_params;
        };
        OpenGlState m_state{};

        // TODO: without shared_ptr
        std::shared_ptr<VertexBufferStore> m_store_vertex_buffers;
        std::shared_ptr<IndexBufferStore> m_store_index_buffers;
        std::shared_ptr<UniformBufferStore> m_store_uniform_buffers;
        std::shared_ptr<DrawableStore> m_store_drawables;
        std::shared_ptr<Texture2DStore> m_store_texture2ds;
        std::shared_ptr<DescriptorSetStore> m_store_descriptor_sets;
        std::shared_ptr<DescriptorSetLayoutStore> m_store_descriptor_set_layouts;
        std::shared_ptr<PipelineStore> m_store_pipelines;
        std::shared_ptr<RenderTargetStore> m_store_render_targets;

        // TODO: allow multiple instances
        std::optional<VertexBufferCreator> m_vertex_buffer_creator;
        std::optional<IndexBufferCreator> m_index_buffer_creator;
        std::optional<UniformBufferCreator> m_uniform_buffer_creator;
        std::optional<DrawableCreator> m_drawable_creator;
        std::optional<Texture2DCreator> m_texture2d_creator;
        std::optional<DescriptorSetCreator> m_descriptor_set_creator;
        std::optional<DescriptorSetLayoutBuilder> m_descriptor_set_layout_builder;
        std::optional<PipelineBuilder> m_pipeline_builder;
        std::optional<RenderTargetBuilder> m_render_target_builder;
        std::optional<Renderer> m_renderer;

        void populate_default_state();
    };
}

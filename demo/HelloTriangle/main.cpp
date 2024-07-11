#include <memory>

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>

#include <core/gl/vulkan/Instance.h>
#include <core/gl/vulkan/PipelineBuilder.h>
#include <core/gl/vulkan/Renderer.h>

#include "core/gl/vulkan/DescriptorSetCreator.h"
#include "core/gl/vulkan/DescriptorSetLayoutBuilder.h"
#include "core/gl/vulkan/DrawableCreator.h"
#include "core/gl/vulkan/UniformBufferCreator.h"
#include "math/math.h"

using namespace yage;

struct UniformBufferData
{
    math::Mat4f projection;
    math::Mat4f view;
    math::Mat4f model;
};

int main()
{
    const std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(
            500, 500, "Hello Triangle", platform::desktop::GlfwWindow::GlApi::API_VULKAN);
    const std::shared_ptr<gl::vulkan::Instance> context =
            std::make_shared<gl::vulkan::Instance>(std::static_pointer_cast<platform::desktop::GlfwWindow>(window));
    context->initialize();

    const auto drawable_creator = gl::vulkan::DrawableCreator(context);

    const gl::VertexDataInfo vertex_data_info{
            .vertex_description =
                    {
                            {gl::VertexComponentSize::VEC2, gl::VertexComponentType::FLOAT_32},
                            {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
                    },
            .data_layout = gl::VertexDataLayout::INTERLEAVED,
    };
    /* clang-format off */
    constexpr std::array<const float, 15> vertex_data = {
            0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    /* clang-format on */
    constexpr gl::IndexDataInfo index_data_info{
            .data_type = gl::IndexType::UINT_16,
            .index_count = 3,
    };
    constexpr std::array<std::uint16_t, 3> index_data = {0, 1, 2};
    const auto drawable = drawable_creator.create(vertex_data_info, std::as_bytes(std::span{vertex_data}),
                                                  index_data_info, std::as_bytes(std::span{index_data}));

    const auto uniform_buffer_creator = gl::vulkan::UniformBufferCreator(context);
    const gl::UniformBufferSharedHandle ubo =
            uniform_buffer_creator.create(sizeof(UniformBufferData), gl::ResourceUsage::DYNAMIC).as_shared();

    auto descriptor_set_layout_builder = gl::vulkan::DescriptorSetLayoutBuilder(context);
    const gl::DescriptorSetLayoutSharedHandle layout =
            descriptor_set_layout_builder.with_uniform_buffer_at(0).build_shared();

    auto descriptor_set_creator = gl::vulkan::DescriptorSetCreator(context);
    gl::DescriptorSetHandle descriptor_set = descriptor_set_creator.create(gl::ResourceUsage::DYNAMIC, layout);
    descriptor_set->write(0, ubo);

    const auto file_reader = window->getFileReader();
    auto vert_code = file_reader->openBinaryFile(R"(assets/vert.spv)", platform::IFile::AccessMode::READ)->read_all();
    auto frag_code = file_reader->openBinaryFile(R"(assets/frag.spv)", platform::IFile::AccessMode::READ)->read_all();
    constexpr gl::VertexComponent vertex_layout[] = {
            {gl::VertexComponentSize::VEC2, gl::VertexComponentType::FLOAT_32},
            {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
    };
    const auto graphics_pipeline = gl::vulkan::PipelineBuilder(context)
                                           .with_shaders(vert_code, frag_code)
                                           .with_rasterizer(gl::PolygonMode::FILL, gl::CullMode::NONE, 1.0f)
                                           .with_render_pass(context->swap_chain_render_pass())
                                           .with_viewport({0, 0, 500.0f, 500.0f}, {0, 0, 500, 500})
                                           .with_vertex_format(gl::PrimitiveTopology::TRIANGLE_LIST, vertex_layout,
                                                               gl::VertexFormat::INTERLEAVED)
                                           .with_layout(layout)
                                           .build();

    gl::vulkan::Renderer renderer(context);

    UniformBufferData ubo_data;
    ubo_data.projection = transpose(math::matrix::perspective(90.0f, 1.0f, 0.1f, 100.f));
    ubo_data.view = transpose(math::matrix::look_at(math::Vec3f(2, 2, 2), math::Vec3f(0, 0, 0)));
    ubo_data.model = transpose(math::matrix::translate(0.f, 0.f, 0.f));

    window->show();
    while (!window->shouldDestroy()) {
        window->pollEvents();

        ubo->get<gl::vulkan::UniformBuffer>().update_data(sizeof(UniformBufferData), &ubo_data);

        context->prepare_frame();

        renderer.begin_command_buffer();
        renderer.begin_render_pass(context->swap_chain_frame_buffer());
        renderer.bind_pipeline(*graphics_pipeline, *descriptor_set);
        renderer.draw(*drawable);
        renderer.end_render_pass();
        renderer.end_command_buffer();

        context->present_frame();
    }

    context->flush_resources();
    return 0;
}

#pragma once

#include <unordered_map>

#include "../IDescriptorSet.h"

namespace yage::gl::opengl4
{
    class DescriptorSet final : public IDescriptorSet
    {
    public:
        void write(unsigned binding, const UniformBufferHandle& uniform_buffer) override;

        void write(unsigned binding, const Texture2DHandle& texture_2d) override;

        void bind_state() const;

    private:
        std::pmr::unordered_map<unsigned int, Texture2DHandle> m_samplers;
    };
}

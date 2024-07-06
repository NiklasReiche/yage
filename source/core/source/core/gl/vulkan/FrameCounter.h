#pragma once

namespace yage::gl::vulkan
{
    struct FrameCounter
    {
        const unsigned int* const curent_frame_index;
        const unsigned int max_frame_index;
    };
}

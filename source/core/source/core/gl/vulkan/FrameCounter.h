#pragma once

namespace yage::gl::vulkan
{
    struct FrameCounter
    {
        const unsigned int* curent_frame_index;
        unsigned int max_frame_index;
    };
}

#pragma once 

#include "../FrameCreator.h"

#include "GL3_BaseObject.h"

namespace gl3
{
	class GL3_FrameCreator final : public GL3_BaseObject, public gl::IFrameCreator
	{
	public:
		std::unique_ptr<gl::IFrame> createFrame(
			int width,
			int height,
			gl::ImageFormat format = gl::ImageFormat::RGB) override;

		std::shared_ptr<gl::IFrame> getDefaultFrame();

	private:
		using GL3_BaseObject::GL3_BaseObject;

		friend class GL3_Context;
	};
}

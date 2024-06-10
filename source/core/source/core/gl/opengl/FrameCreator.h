#pragma once 

#include "../FrameCreator.h"

#include "BaseObject.h"

namespace yage::opengl
{
	class FrameCreator final : public BaseObject, public gl::IFrameCreator
	{
	public:
		std::unique_ptr<gl::IFrame> createFrame(
			int width,
			int height,
			gl::ImageFormat format = gl::ImageFormat::RGB) override;

		std::shared_ptr<gl::IFrame> getDefaultFrame();

	private:
		using BaseObject::BaseObject;

		friend class Context;
	};
}

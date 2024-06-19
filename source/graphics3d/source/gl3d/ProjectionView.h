#pragma once

#include <memory>

#include <core/gl/IUniformBlock.h>
#include <math/matrix.h>

namespace yage::gl3d
{
	class ProjectionView
	{
	public:
		math::Mat4f projection;
		math::Mat4f view;

		explicit ProjectionView(std::unique_ptr<gl::IUniformBlock> uniformBlock);

		void sync();

		void sync_projection();

		void sync_view();

		[[nodiscard]] const gl::IUniformBlock& ubo() const;

	private:
		std::unique_ptr<gl::IUniformBlock> m_uniform_block;
	};
}

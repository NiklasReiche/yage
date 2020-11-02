#pragma once

#include "../DrawableCreator.h"

#include "BaseObject.h"

namespace opengl
{
	class DrawableCreator final : public BaseObject, public gl::IDrawableCreator
	{
	public:
		std::unique_ptr<gl::IDrawable> createDrawable(
			const std::span<const float>& vertices,
			const std::vector<unsigned int>& vertexLayout,
			gl::VertexFormat format = gl::VertexFormat::INTERLEAVED) override;

	private:
		using BaseObject::BaseObject;

		friend class Context;
	};
}// namespace gl3

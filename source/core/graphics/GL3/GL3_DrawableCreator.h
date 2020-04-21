#pragma once 

#include "../DrawableCreator.h"

#include "GL3_BaseObject.h"

namespace gl3
{
	class GL3_DrawableCreator final : public GL3_BaseObject, public gl::IDrawableCreator
	{
	public:
		std::unique_ptr<gl::IDrawable> createDrawable(
			const std::vector<float> & vertices,
			const std::vector<unsigned int> & vertexLayout,
			gl::VertexFormat format = gl::VertexFormat::INTERLEAVED) override;

	private:
		using GL3_BaseObject::GL3_BaseObject;

		friend class GL3_Context;
	};
}

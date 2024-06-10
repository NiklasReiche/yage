#pragma once

#include "../ElementBuffer.h"
#include "OpenGlObject.h"
#include "OpenGL.h"

namespace yage::opengl
{
	class ElementBuffer final
		: public gl::ElementBuffer, public OpenGlObject, public std::enable_shared_from_this<ElementBuffer>
	{
	public:
		~ElementBuffer() override;

		ElementBuffer(const ElementBuffer& other) = delete;

		ElementBuffer& operator=(const ElementBuffer& other) = delete;

	private:
		GLuint ebo = 0;
		int byteSize = 0;

		using OpenGlObject::OpenGlObject;

		ElementBuffer(ElementBuffer&& other) noexcept;

		ElementBuffer& operator=(ElementBuffer&& other) noexcept;

		friend class Context;

		friend class Renderer;

		friend class DrawableCreator;
	};
}

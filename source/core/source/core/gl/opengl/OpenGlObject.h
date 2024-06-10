#pragma once

#include "BaseObject.h"
#include "OpenGL.h"

namespace yage::opengl
{
	class OpenGlObject : public BaseObject
	{
	public:
		~OpenGlObject() override = default;
		OpenGlObject(const OpenGlObject& other) = delete;
		OpenGlObject& operator=(const OpenGlObject& other) = delete;
		
		friend bool operator==(const OpenGlObject& left, const OpenGlObject& right);
		friend bool operator!=(const OpenGlObject& left, const OpenGlObject& right);

		[[nodiscard]]
		bool isValid() const;
		
		[[nodiscard]]
		GLuint getId() const;

	protected:
		GLuint id = 0;

		using BaseObject::BaseObject;

		OpenGlObject(OpenGlObject&& other) noexcept;
		OpenGlObject& operator=(OpenGlObject&& other) noexcept;

	private:
		friend class Context;
	};
}

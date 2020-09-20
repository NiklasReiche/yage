#pragma once

#include <platform/OpenGL.h>

#include "GL3_BaseObject.h"

namespace gl3
{
	class GL3_Object : public GL3_BaseObject
	{
	public:
		virtual ~GL3_Object() = default;
		GL3_Object(const GL3_Object& other) = delete;
		GL3_Object& operator=(const GL3_Object& other) = delete;
		
		friend bool operator==(const GL3_Object & left, const GL3_Object & right);
		friend bool operator!=(const GL3_Object & left, const GL3_Object & right);

		[[nodiscard]]
		bool isValid() const;
		
		[[nodiscard]]
		GLuint getId() const;

	protected:
		GLuint id = 0;

		using GL3_BaseObject::GL3_BaseObject;
		
		GL3_Object(GL3_Object&& other) noexcept;
		GL3_Object& operator=(GL3_Object&& other) noexcept;

	private:
		friend class GL3_Context;
	};
}

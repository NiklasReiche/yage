#pragma once

#include <memory>
#include <stdexcept>

namespace gl3
{
	class GL3_Context;
	
	class GL3_BaseObject
	{
	public:
		virtual ~GL3_BaseObject() = default;
		GL3_BaseObject(const GL3_BaseObject& other) = delete;
		GL3_BaseObject& operator=(const GL3_BaseObject& other) = delete;
		
	protected:
		explicit GL3_BaseObject(std::weak_ptr<GL3_Context> contextPtr)
			: contextPtr(std::move(contextPtr)) { }
		
		GL3_BaseObject(GL3_BaseObject&& other) = default;
		GL3_BaseObject& operator=(GL3_BaseObject&& other) = default;

		[[nodiscard]]
		std::shared_ptr<GL3_Context> lockContextPtr() const
		{
			auto ptr = contextPtr.lock();
			if (!ptr)
				throw std::logic_error("Trying to access OpenGL function, but the Context was destroyed");
			return ptr;
		}

	private:
		std::weak_ptr<GL3_Context> contextPtr;
	};
}

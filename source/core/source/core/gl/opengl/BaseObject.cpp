#include "BaseObject.h"

namespace opengl
{
	BaseObject::BaseObject(std::weak_ptr<Context> contextPtr)
	: contextPtr (std::move(contextPtr))
	{ }

	std::shared_ptr<Context> BaseObject::lockContextPtr() const
	{
		auto ptr = contextPtr.lock();
		if (!ptr)
			throw std::logic_error("Trying to access OpenGL function, but the Context was destroyed");
		return ptr;
	}
}

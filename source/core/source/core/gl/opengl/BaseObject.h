#pragma once

#include <memory>
#include <stdexcept>

namespace yage::opengl
{
	class Context;

	class BaseObject
	{
	public:
		virtual ~BaseObject() = default;

		BaseObject(const BaseObject& other) = delete;

		BaseObject& operator=(const BaseObject& other) = delete;

	protected:
		explicit BaseObject(std::weak_ptr<Context> contextPtr);

		BaseObject(BaseObject&& other) = default;

		BaseObject& operator=(BaseObject&& other) = default;

		[[nodiscard]]
		std::shared_ptr<Context> lockContextPtr() const;

	private:
		std::weak_ptr<Context> contextPtr;
	};
}

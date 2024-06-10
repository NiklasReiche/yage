#pragma once

namespace yage::gl
{
	class IUniformBlock
	{
	public:
		virtual ~IUniformBlock() = default;

		virtual void setData(const void* data, std::size_t size) = 0;

		virtual void setSubData(std::size_t offset, const void* data, std::size_t size) = 0;
	};
}

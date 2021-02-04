#pragma once

#include "OpenGlObject.h"
#include "../IUniformBlock.h"

namespace opengl
{
	class UniformBuffer final : public OpenGlObject, public gl::IUniformBlock, std::enable_shared_from_this<UniformBuffer>
	{
	public:
		~UniformBuffer() override;

		UniformBuffer(const UniformBuffer& other) = delete;

		UniformBuffer& operator=(const UniformBuffer& other) = delete;

		void setData(const void* data, std::size_t size) override;

		void setSubData(std::size_t offset, const void* data, std::size_t size) override;

		std::string getName() const;

	private:
		GLuint ubo;
		std::string name;

		using OpenGlObject::OpenGlObject;

		UniformBuffer(UniformBuffer&& other) noexcept;

		UniformBuffer& operator=(UniformBuffer&& other) noexcept;

		friend class Context;
		friend class Renderer;
		friend class ShaderCreator;
	};
}

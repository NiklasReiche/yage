#pragma once

namespace gl
{
	class ICubemap
	{
	public:
		virtual ~ICubemap() = default;

		[[nodiscard]]
		virtual int getWidth() const = 0;

		[[nodiscard]]
		virtual int getHeight() const = 0;

		[[nodiscard]]
		virtual int getChannels() const = 0;

	protected:
		ICubemap() = default;

		ICubemap(const ICubemap &other) = default;

		ICubemap(ICubemap &&other) = default;

		ICubemap &operator=(const ICubemap &other) = default;

		ICubemap &operator=(ICubemap &&other) = default;
	};
}

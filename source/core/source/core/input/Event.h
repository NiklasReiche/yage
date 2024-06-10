#pragma once

namespace yage::input
{
	class Event
	{
	public:
		enum class Type
		{
			KEY_EVENT,
			MOUSE_POS_EVENT,
			MOUSE_BUTTON_EVENT,
			MOUSE_WHEEL_EVENT,
            CHAR_EVENT
		};

		explicit Event(Type type) noexcept;

		virtual ~Event() = default;

		[[nodiscard]]
		Type getType() const noexcept;

	private:
		const Type type;
	};
}

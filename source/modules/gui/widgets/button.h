#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "../core.h"
#include "../Widget.h"
#include "Label.h"
#include "frame.h"

namespace gui
{
	struct ButtonTemplate : public WidgetTemplate
	{
		TextLayout text;
		unsigned int clickColor = color;
		unsigned int hoverColor = color;
		std::function<void()> command;
	};

	class PushButton : public Widget
	{
	protected:
		Label * label;
		std::function<void()> command;
		gml::Vec4<float> idleColor;
		gml::Vec4<float> hoverColor;
		gml::Vec4<float> clickColor;

	public:
		PushButton(Widget * parent, MasterInterface master, const ButtonTemplate & layout);
		virtual ~PushButton() {}

		virtual void onClick();
		virtual void onClickRelease();
		virtual void onHover();
		virtual void onHoverRelease();
		virtual void onCancel();

		void setCallback(std::function<void(void)> command) { this->command = command; }
		void setText(TextLayout text) { label->setText(text); }
		void setTextColor(unsigned int color) { label->setTextColor(color); }
	};

	class CheckButton : public PushButton
	{
	protected:
		bool state = false;

	public:
		CheckButton(Widget * parent, MasterInterface master, const ButtonTemplate & layout, bool activate = false);

		virtual void onClick();
		virtual void onClickRelease();
		virtual void onHover();
		virtual void onHoverRelease();
		virtual void onCancel();

		bool getState() { return state; }
	};


	template<typename T> class RadioGroup;

	template<typename T>
	class RadioButton : public CheckButton
	{
	protected:
		T value;

	public:
		RadioButton(Widget * parent, MasterInterface master, const ButtonTemplate & layout, T value, bool isDefault = false)
			: CheckButton(parent, master, layout, isDefault), value(value) {}

		virtual void onClick()
		{
			if (!state) {
				setColor(clickColor);
				updateParams();
			}
		}
		virtual void onClickRelease()
		{
			if (!state) {
				state = true;
				((RadioGroup<T>*)parent)->onRadioButtonClick(value);
			}
		}

		virtual void deactivate()
		{
			state = false;
			setColor(idleColor);
			updateParams();
		}
	};

	template<typename T>
	class RadioGroup : public Frame
	{
	private:
		T state;
		ButtonTemplate buttonLayout;
		std::map<T, RadioButton<T>*> buttons;

	public:
		RadioGroup(Widget * parent, MasterInterface master, const FrameTemplate & layout, const ButtonTemplate & buttonLayout, T defaultValue)
			: Frame(parent, master, layout), buttonLayout(buttonLayout), state(defaultValue) 
		{
			this->parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_AROUND);
			this->childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);
			this->buttonLayout.geometry.offset = gml::Vec2<float>();
		}

		void addButton(std::string text, T value, bool isDefault = false)
		{
			buttonLayout.text.text = text;

			buttons[value] = createWidget<RadioButton<T>>(master, buttonLayout, value, isDefault);
			if (isDefault) {
				state = value;
			}
		}

		void onRadioButtonClick(T value)
		{
			if (state != value) {
				buttons.at(state)->deactivate();
				state = value;

				try {
					buttonLayout.command();
				}
				catch (std::bad_function_call) {}
			}
		}

		T getState() { return state; }
	};
}
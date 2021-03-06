#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "../core.h"
#include "../widget.h"
#include "label.h"
#include "frame.h"

namespace gui
{
	struct ButtonTemplate : public WidgetTemplate
	{
		TextTemplate text;
		unsigned int clickColor = color;
		unsigned int hoverColor = color;
		WidgetTextureTemplate clickTexture = texture;
		WidgetTextureTemplate hoverTexture = texture;
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
		gml::Vec4<float> idleTexCoords;
		gml::Vec4<float> hoverTexCoords;
		gml::Vec4<float> clickTexCoords;

	public:
		PushButton(Widget * parent, MasterInterface master, const ButtonTemplate & layout);
		virtual ~PushButton() {}

		virtual void onClick();
		virtual void onClickRelease();
		virtual void onHover();
		virtual void onHoverRelease();
		virtual void onCancel();
		virtual void onResume();

		void setCallback(std::function<void(void)> command) { this->command = command; }
		void setText(TextTemplate text) { label->setText(text); }
		void setTextColor(unsigned int color) { label->setTextColor(color); }

		void setIdleTexture(WidgetTextureTemplate texture) { this->idleTexCoords = loadTexture(texture); /*setTexCoords(idleTexCoords); updateParams();*/ }
		void setHoverTexture(WidgetTextureTemplate texture) { this->hoverTexCoords = loadTexture(texture); }
		void setClickTexture(WidgetTextureTemplate texture) { this->clickTexCoords = loadTexture(texture); }

		gml::Vec2f calcPrefSize();

		void debug() { onHover(); }
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
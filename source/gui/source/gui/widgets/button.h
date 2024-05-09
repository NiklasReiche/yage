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
		TextureAtlasView idleTexCoords;
        TextureAtlasView hoverTexCoords;
        TextureAtlasView clickTexCoords;

	public:
		PushButton(Widget * parent, Master* master, const ButtonTemplate & layout);
		virtual ~PushButton() {}

		virtual void on_click();
		virtual void on_click_release();
		virtual void on_hover();
		virtual void on_hover_release();
		virtual void on_cancel();
		virtual void on_resume();

		void setCallback(std::function<void(void)> command) { this->command = command; }
		void setText(TextTemplate text) { label->setText(text); }
		//void setTextColor(unsigned int color) { label->setTextColor(color); }

		gml::Vec2f calcPrefSize();

		void debug() { on_hover(); }
	};

	class CheckButton : public PushButton
	{
	protected:
		bool state = false;

	public:
		CheckButton(Widget * parent, Master* master, const ButtonTemplate & layout, bool activate = false);

		virtual void on_click();
		virtual void on_click_release();
		virtual void on_hover();
		virtual void on_hover_release();
		virtual void on_cancel();

		bool getState() { return state; }
	};


	template<typename T> class RadioGroup;

	template<typename T>
	class RadioButton : public CheckButton
	{
	protected:
		T value;

	public:
		RadioButton(Widget * parent, Master* master, const ButtonTemplate & layout, T value, bool isDefault = false)
			: CheckButton(parent, master, layout, isDefault), value(value) {}

		virtual void on_click()
		{
			if (!state) {
				setColor(clickColor);
                update_vertices();
			}
		}
		virtual void on_click_release()
		{
			if (!state) {
				state = true;
				((RadioGroup<T>*)m_parent)->onRadioButtonClick(value);
			}
		}

		virtual void deactivate()
		{
			state = false;
			setColor(idleColor);
            update_vertices();
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
		RadioGroup(Widget * parent, Master* master, const FrameTemplate & layout, const ButtonTemplate & buttonLayout, T defaultValue)
			: Frame(parent, master, layout), state(defaultValue), buttonLayout(buttonLayout)
		{
			this->buttonLayout.geometry.offset = gml::Vec2<float>();
		}

		void addButton(std::u32string text, T value, bool isDefault = false)
		{
			buttonLayout.text.text = text;

			buttons[value] = create_widget<RadioButton<T>>(m_master, buttonLayout, value, isDefault);
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
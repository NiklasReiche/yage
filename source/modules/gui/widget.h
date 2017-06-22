#pragma once

#include <vector>
#include <array>
#include <memory>
#include <functional>

#include "core.h"
#include "Interface.h"
#include "FontManager.h"

namespace gui
{
	struct W_Geometry
	{
		gml::Vec2<float> position;
		gml::Vec2<float> size;
	};
	struct W_Border
	{
		int size = 0;
		unsigned int color = gl::Color::BLACK;
	};
	struct W_Shadow
	{
		int offset = 0;
		float hardness = 0;
	};
	struct WidgetLayout
	{
		W_Geometry geometry;
		W_Border border;
		W_Shadow shadow;
		unsigned int color = gl::Color::WHITE;
	};

	class Widget : public gl::Drawable
	{
	private:
		friend class InputManager;

		void constructVertices(std::vector<gl::Gfloat> & vertices);

	protected:
		Widget* parent;
		std::vector<std::unique_ptr<Widget>> children;

		MasterInterface master;

		int level = 0; /* position of the widget in the widget tree hirachy */
		bool isActive = true;
		bool isInteractable = false;
		bool isHovered = false;
		bool hasText = false;

		gml::Vec2<float> position;
		gml::Vec2<float> size;
		gml::Vec4<float> color;

		int borderSize;
		gml::Vec4<float> borderColor;
		
		int shadowOffset;
		float shadowHardness;

	public:
		Widget() {}
		Widget(Widget * parent, MasterInterface master, W_Geometry geometry, unsigned int color, W_Border border, W_Shadow shadow);
		Widget(Widget * parent, MasterInterface master, const WidgetLayout & layout);
		virtual ~Widget() {}

		template <typename Element, typename... Args>
		Element* createWidget(MasterInterface mInterface, Args... args)
		{
			children.push_back( std::make_unique<Element>(this, mInterface, std::forward<Args>(args)...) );
			return (Element*)children.back().get();
		}
		
		virtual void onHover() {}
		virtual void onHoverRelease() {}
		virtual void onClick() {}
		virtual void onClickRelease() {}
		virtual void onCancel() {}

		virtual void update() {};
		//virtual void del() {}

		/* Getter */
		bool is_Active() { return isActive; }
		bool is_Interactable() { return isInteractable; }
		bool isHoverActive() { return isHovered; }
		bool has_Text() { return hasText; }

		Widget& getChild(int i) { return *children[i].get(); }
		unsigned int getChildrenCount() { return children.size(); }

		virtual font::Text* getText() { return nullptr; }

		gml::Vec2<float> getPosition() { return position; }
		gml::Vec2<float> getSize() { return size; }
		gml::Vec4<float> getColor() { return color; }

		/* Setter */
		void hide() { isActive = false; }
		void show() { isActive = true; }

		void setGeometry(W_Geometry geometry) { this->position = geometry.position; this->size = geometry.size; }
		void setPosition(gml::Vec2<float> position) { this->position = position; }
		void setSize(gml::Vec2<float> size) { this->size = size; }

		void setColor(int color) { this->color = gl::toVec4(color); }
		void setColor(gml::Vec4<float> color) { this->color = color; }

		void setBorder(W_Border border) { this->borderSize = border.size; this->borderColor = gl::toVec4(border.color); }
		void setBorderSize(int size) { this->borderSize = size; }
		void setBorderColor(unsigned int color) { this->borderColor = gl::toVec4(color); }

		void setShadow(W_Shadow shadow) { this->shadowOffset = shadow.offset; this->shadowHardness = shadow.hardness; }
		void setShadow(int offset) { this->shadowOffset = offset; }
		void setShadow(float hardness) { this->shadowHardness = hardness; }

		/* update semantics */
		void updateParams();
		void resize(gml::Vec2<float> size) { setSize(size); updateParams(); }
		void reposition(gml::Vec2<float> position) { setPosition(position); updateParams(); }
	};
}
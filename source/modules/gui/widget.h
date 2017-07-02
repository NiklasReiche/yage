#pragma once

#include <vector>
#include <array>
#include <memory>
#include <tuple>
#include <functional>

#include "core.h"
#include "Interface.h"
#include "FontManager.h"
#include "Animation.h"
#include "Layouts.h"

namespace gui
{
	class Master;

	enum class Anchor
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		CENTER
	};
	enum class ValueType
	{
		ABSOLUTE,
		RELATIVE
	};
	enum class ParentSizeHint
	{
		WRAP_CHILDREN,
		WRAP_AROUND
	};
	enum class ChildSizeHint
	{
		MIN,
		FIXED,
		ASPECT
	};

	struct W_Geometry
	{
		Anchor anchor = Anchor::TOP_LEFT;
		ValueType offsetType = ValueType::ABSOLUTE;
		gml::Vec2<float> offset;
		ValueType sizeType = ValueType::ABSOLUTE;
		gml::Vec2<float> size;
		ParentSizeHint parentSizeHint = ParentSizeHint::WRAP_CHILDREN;
		ChildSizeHint childSizeHint = ChildSizeHint::MIN;
	};
	struct W_Border
	{
		int size = 0;
		unsigned int color = gl::Color::BLACK;
	};
	struct W_Shadow
	{
		int offset = 0;
		float hardness = 0.5f;
	};
	struct WidgetLayout
	{
		LayoutType layout;
		W_Geometry geometry;
		W_Border border;
		W_Shadow shadow;
		unsigned int color = gl::Color::WHITE;
	};


	class Widget : public gl::Drawable
	{
	private:
		friend class InputManager;
		friend class Animation;

		void constructCoords(std::vector<gl::Gfloat> & vertices);
		void constructColors(std::vector<gl::Gfloat> & vertices);
		void constructVertices(std::vector<gl::Gfloat> & vertices);

	protected:
		MasterInterface master;
		Widget* parent = nullptr;
		std::vector<std::unique_ptr<Widget>> children;
		std::unique_ptr<Layout> layouter;
		std::vector<Animation> animations;

		int level = 0; /* position of the widget in the widget tree hirachy */
		bool isActive = true;
		bool isInteractable = false;
		bool isHovered = false;
		bool hasText = false;

		Anchor anchor;
		ParentSizeHint parentSizeHint;
		ChildSizeHint childSizeHint;
		gml::Vec2<float> offset;
		gml::Vec2<float> position;
		gml::Vec2<float> size;
		gml::Vec4<float> color;

		int borderSize;
		gml::Vec4<float> borderColor;
		
		int shadowOffset;
		float shadowHardness;

	public:
		Widget();
		Widget(Widget * parent, MasterInterface master, const WidgetLayout & layout);
		virtual ~Widget() {}

		template <typename Element, typename... Args>
		Element* createWidget(MasterInterface mInterface, Args... args)
		{
			children.push_back( std::make_unique<Element>(this, mInterface, std::forward<Args>(args)...) );
			layouter->update(this);
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

#if 0
		void setGeometry(W_Geometry geometry) { this->position = geometry.position; this->size = geometry.size; }
		void setPosition(gml::Vec2<float> position) { this->position = position; }
		void setSize(gml::Vec2<float> size) { this->size = size; }
#endif

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
		virtual void updateGeometry();
		virtual void resize(gml::Vec2<float> size);
		virtual void move(gml::Vec2<float> position);
		virtual void setAnchor(Anchor anchor);

		ParentSizeHint getParentSizeHint() { return parentSizeHint; }
		ChildSizeHint getChildSizeHint() { return childSizeHint; }

		Animation* createAnimation(Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time);
	};
}
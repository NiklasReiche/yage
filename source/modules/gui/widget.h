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
#include "widgetTemplate.h"
#include "Layouts.h"

namespace gui
{
	class Master;

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

		std::vector<std::unique_ptr<Animation>> animations;

		int level = 0; /* position of the widget in the widget tree hirachy */
		bool isActive = true;
		bool isInteractable = false;
		bool keepFocus = false;
		bool isHovered = false;
		bool hasText = false;

		std::unique_ptr<Layout> layout;
		gml::Vec2<ParentSizeHint> parentSizeHint = gml::Vec2<ParentSizeHint>(ParentSizeHint::WRAP_CHILDREN_RESIZE);
		gml::Vec2<ChildSizeHint> childSizeHint = gml::Vec2<ChildSizeHint>(ChildSizeHint::MIN);

		Anchor anchor = Anchor::TOP_LEFT;
		/* offset from parent widget */
		gml::Vec2f offset;
		/* offset value used by layouts */
		gml::Vec2f cellMargin;
		/* padding for layouts */
		gml::Vec2f layoutMargin;
		/* prefered size for layouts */
		gml::Vec2f prefSize;

		/* absolute position of the outer top left edge */
		gml::Vec2f position;
		/* absolute total size of the widget */
		gml::Vec2f size;
		/* absolute position of the inner top left edge - equal to position for borderless widgets */
		gml::Vec2f innerPosition;
		/* absolute size of the widget interior space - equal to size for borderless widgets */
		gml::Vec2f innerSize;

		gml::Vec4<float> color;
		int borderSize = 0;
		gml::Vec4<float> borderColor;
		int shadowOffset = 0;
		float shadowHardness = 0.0f;

	public:
		Widget();
		Widget(Widget * parent, MasterInterface master, const WidgetTemplate & widgetTemplate);
		virtual ~Widget() {}

		template <typename Element, typename... Args>
		Element* createWidget(MasterInterface mInterface, Args... args)
		{
			children.push_back( std::make_unique<Element>(this, mInterface, std::forward<Args>(args)...) );
			relayout();
			return (Element*)children.back().get();
		}

		template <typename Element, typename... Args>
		Animation* createAnimation(Master* master, Args... args)
		{
			animations.push_back( std::make_unique<Element>(this, master, std::forward<Args>(args)...) );
			return (Element*)animations.back().get();
		}
		

		virtual void onHover() {}
		virtual void onHoverRelease() {}
		virtual void onClick() {}
		virtual void onClickRelease() {}
		virtual void onCancel() {}
		virtual void onFocus() {}
		virtual void onFocusRelease() {}
		virtual void onCharInput(char character) {}
		virtual void onKeyPress(input::KeyCode key) {}

		virtual font::Text* getText() { return nullptr; }


		void hide() { isActive = false; }
		void show() { isActive = true; }

		/* Getter */
		bool is_Active() { return isActive; }
		bool is_Interactable() { return isInteractable; }
		bool has_Text() { return hasText; }

		Widget* getParent() { return parent; }
		Widget& getChild(int i) { return *children[i].get(); }
		unsigned int getChildrenCount() { return children.size(); }


		gml::Vec2<ParentSizeHint> getParentSizeHint() { return parentSizeHint; }
		gml::Vec2<ChildSizeHint> getChildSizeHint() { return childSizeHint; }
		gml::Vec2f getPreferredSize() { return prefSize; }
		gml::Vec2f getCellMargin() { return cellMargin; }
		gml::Vec2f getLayoutMargin() { return layoutMargin; }
		gml::Vec2f getOffset() { return offset; }
		gml::Vec2f getPosition() { return position; }
		gml::Vec2f getInnerPosition() { return innerPosition; }
		gml::Vec2f getSize() { return size; }
		gml::Vec2f getInnerSize() { return innerSize; }
		gml::Vec4f getColor() { return color; }
		

		/* Setter */
		void setColor(int color) { this->color = gl::toVec4(color); }
		void setColor(gml::Vec4<float> color) { this->color = color; }
		

		/* update semantics */
		void updateParams();
		virtual void updateGeometry();
		virtual void relayout();

		virtual void setSize(gml::Vec2f size);
		virtual void setPreferredSize(gml::Vec2f size) { prefSize = size; }
		virtual void setAnchor(Anchor anchor);
		virtual void resize(gml::Vec2f size);
		virtual void move(gml::Vec2f position);


		gml::Vec2f toAbs(GeoVec geoVec);
	};
}
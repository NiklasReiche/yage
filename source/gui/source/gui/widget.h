#pragma once

#include <vector>
#include <array>
#include <memory>
#include <tuple>
#include <functional>

#include <core/input/KeyEvent.h>

#include "core.h"
#include "texturemanager.h"
#include "animation.h"
#include "widgettemplate.h"
#include "layouts.h"

namespace gui
{
    class Master;

    class Widget
    {
    public:
        std::unique_ptr<gl::IDrawable> drawable;

        Widget(Widget* parent, Master* master, const WidgetTemplate& widgetTemplate);

        virtual ~Widget() = default;

        template<typename Element, typename... Args>
        Element* createWidget(Master* mInterface, Args... args)
        {
            children.push_back(std::make_unique<Element>(this, mInterface, std::forward<Args>(args)...));
            update_layout();
            return (Element*) children.back().get();
        }

        template<typename Element, typename... Args>
        Element* createAnimation(Master* _master, Args... args)
        {
            animations.push_back(std::make_unique<Element>(this, _master, std::forward<Args>(args)...));
            return (Element*) animations.back().get();
        }

        virtual font::Text* getText()
        {
            return nullptr;
        }

        virtual gml::Vec2f calcPrefSize()
        {
            return prefSize;
        }


        void hide()
        {
            isActive = false;
        }

        void show()
        {
            isActive = true;
        }

        /* Getter */
        bool is_Active()
        {
            return isActive;
        }

        bool is_Interactable()
        {
            return isInteractable;
        }

        bool has_Text()
        {
            return hasText;
        }

        Widget* getParent()
        {
            return parent;
        }

        Widget& getChild(int i)
        {
            return *children[i].get();
        }

        unsigned int getChildrenCount()
        {
            return children.size();
        }


        gml::Vec2<SizeHint> getSizeHint()
        {
            return sizeHint;
        }

        gml::Vec2<OffsetHint> getOffsetHint()
        {
            return offsetHint;
        }

        gml::Vec2f getMinSize()
        {
            return minSize;
        }

        gml::Vec2f getMaxSize()
        {
            return maxSize;
        }

        gml::Vec2f getPreferredSize()
        {
            return prefSize;
        }

        gml::Vec2f getCellMargin()
        {
            return cellMargin;
        }

        gml::Vec2f getLayoutMargin()
        {
            return layoutMargin;
        }

        gml::Vec2f getOffset()
        {
            return offset;
        }

        gml::Vec2f getPosition()
        {
            return position;
        }

        gml::Vec2f getInnerPosition()
        {
            return innerPosition;
        }

        gml::Vec2f getSize()
        {
            return size;
        }

        gml::Vec2f getInnerSize()
        {
            return innerSize;
        }

        gml::Vec4f getColor()
        {
            return color;
        }


        /* Setter */
        void setColor(int _color)
        {
            this->color = gl::toVec4(_color);
        }

        void setColor(gml::Vec4<float> _color)
        {
            this->color = _color;
        }

        void setSizeHint(gml::Vec2<SizeHint> _sizeHint)
        {
            this->sizeHint = _sizeHint;
        }

        void setPreferredSize(gml::Vec2f _size)
        {
            this->prefSize = _size;
        }

        void setTextureAtlasView(TextureAtlasView texture_atlas_view)
        {
            m_texture_atlas_view = texture_atlas_view;
        }


        /* update semantics */
        void update_parameters();

        virtual void update_geometry();

        virtual void update_layout();

        virtual void setAnchor(Anchor _anchor);

        virtual void setSize(gml::Vec2f _size);

        virtual void resize(gml::Vec2f _size);

        virtual void setOffset(gml::Vec2f _offset);

        virtual void move(gml::Vec2f _cellMargin);


        gml::Vec2f toAbs(gml::Vec2f value);

        float toAbsX(float value);

        float toAbsY(float value);

        float fromAspect();

        TextureAtlasView texture_atlas_view();

        virtual void onHover()
        {}

        virtual void onHoverRelease()
        {}

        virtual void onClick()
        {}

        virtual void onClickRelease()
        {}

        virtual void onCancel()
        {}

        virtual void onResume()
        {}

        virtual void onFocus()
        {}

        virtual void onFocusRelease()
        {}

        virtual void onCharInput(char character)
        {}

        virtual void onKeyPress(input::KeyEvent::Code key)
        {}

    protected:
        Master* master;
        Widget* parent = nullptr;
        std::vector<std::unique_ptr<Widget>> children;

        std::vector<std::unique_ptr<Animation>> animations;

        int level = 0; /* position of the widget in the widget tree hirachy */
        bool isActive = true;
        bool isInteractable = false;
        bool keepFocus = false;
        bool isHovered = false;
        bool hasText = false;
        bool fitChildren = false;

        std::unique_ptr<Layout> m_layout;
        gml::Vec2<SizeHint> sizeHint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
        gml::Vec2<OffsetHint> offsetHint = gml::Vec2<OffsetHint>(OffsetHint::FIXED);

        Anchor anchor = Anchor::TOP_LEFT;
        /* offset value used by layouts */
        gml::Vec2f cellMargin;
        /* padding for layouts */
        gml::Vec2f layoutMargin;
        /* prefered size for layouts */
        gml::Vec2f prefSize;
        /* minimal size for layouts */
        gml::Vec2f minSize;
        /* maximal size for layouts */
        gml::Vec2f maxSize;

        /* absolute offset from parent widget */
        gml::Vec2f offset;
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

        TextureAtlasView m_texture_atlas_view;

        TextureAtlasView loadTexture(WidgetTextureTemplate tTemplate);

    private:
        friend class InputManager;

        friend class Animation;

        std::vector<float> constructCoords();

        std::vector<float> constructColors();

        std::vector<float> constructTexCoords(TextureAtlasView texture_atlas_view);

        std::vector<float> constructVertices(TextureAtlasView texture_atlas_view);

        std::vector<unsigned int> construct_indices();
    };
}
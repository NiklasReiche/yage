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
        Widget(Widget* parent, Master* master, const WidgetTemplate& widgetTemplate);

        virtual ~Widget() = default;

        template<typename Element, typename... Args>
        Element* create_widget(Master* master, Args... args)
        {
            m_children.push_back(std::make_unique<Element>(this, master, std::forward<Args>(args)...));
            update_layout();
            return (Element*) m_children.back().get();
        }

        template<typename Element, typename... Args>
        Element* create_animation(Master* master, Args... args)
        {
            m_animations.push_back(std::make_unique<Element>(this, master, std::forward<Args>(args)...));
            return (Element*) m_animations.back().get();
        }

        virtual font::Text* text()
        {
            return nullptr;
        }

        virtual gml::Vec2f calcPrefSize()
        {
            return m_hint_pref_size;
        }


        void hide()
        {
            m_is_active = false;
        }

        void show()
        {
            m_is_active = true;
        }

        [[nodiscard]]
        bool is_Active() const
        {
            return m_is_active;
        }

        [[nodiscard]]
        bool is_Interactable() const
        {
            return m_is_interactable;
        }

        [[nodiscard]]
        bool has_Text() const
        {
            return m_has_text;
        }

        Widget* getParent()
        {
            return m_parent;
        }

        Widget& getChild(int i)
        {
            return *m_children[i].get();
        }
// TODO: use iterators
        unsigned int getChildrenCount()
        {
            return m_children.size();
        }


        gml::Vec2<SizeHint> getSizeHint()
        {
            return m_size_hint;
        }

        gml::Vec2<OffsetHint> getOffsetHint()
        {
            return m_offset_hint;
        }

        gml::Vec2f getMinSize()
        {
            return m_hint_min_size;
        }

        gml::Vec2f getMaxSize()
        {
            return m_hint_max_size;
        }

        gml::Vec2f getPreferredSize()
        {
            return m_hint_pref_size;
        }

        gml::Vec2f getCellMargin()
        {
            return m_margin;
        }

        gml::Vec2f getLayoutMargin()
        {
            return m_padding;
        }

        gml::Vec2f getOffset()
        {
            return m_offset;
        }

        gml::Vec2f getPosition()
        {
            return m_position;
        }

        gml::Vec2f getInnerPosition()
        {
            return m_inner_position;
        }

        gml::Vec2f getSize()
        {
            return m_size;
        }

        gml::Vec2f getInnerSize()
        {
            return m_inner_size;
        }

        gml::Vec4f getColor()
        {
            return m_color;
        }

        void setColor(int color)
        {
            this->m_color = gl::toVec4(color);
        }

        void setColor(gml::Vec4<float> color)
        {
            this->m_color = color;
        }

        void setSizeHint(gml::Vec2<SizeHint> sizeHint)
        {
            this->m_size_hint = sizeHint;
        }

        void setPreferredSize(gml::Vec2f size)
        {
            this->m_hint_pref_size = size;
        }

        void setTextureAtlasView(TextureAtlasView texture_atlas_view)
        {
            m_texture_atlas_view = texture_atlas_view;
        }


        /* update semantics */
        void update_vertices();

        virtual void update_geometry();

        virtual void update_layout();


        virtual void set_anchor(Anchor _anchor);

        virtual void set_size(gml::Vec2f _size);

        virtual void resize(gml::Vec2f _size);

        virtual void set_offset(gml::Vec2f _offset);

        virtual void move(gml::Vec2f _cellMargin);


        gml::Vec2f to_absolute(gml::Vec2f value);

        float to_absolute_x(float value);

        float to_absolute_y(float value);

        float from_aspect();

        TextureAtlasView texture_atlas_view();

        gl::IDrawable& drawable();

        virtual void on_hover()
        {}

        virtual void on_hover_release()
        {}

        virtual void on_click()
        {}

        virtual void on_click_release()
        {}

        virtual void on_cancel()
        {}

        virtual void on_resume()
        {}

        virtual void on_focus()
        {}

        virtual void on_focus_release()
        {}

        virtual void on_char_input(char character)
        {}

        virtual void on_key_press(input::KeyEvent::Code key)
        {}

    protected:
        Master* m_master;
        Widget* m_parent = nullptr;
        std::vector<std::unique_ptr<Widget>> m_children;

        std::vector<std::unique_ptr<Animation>> m_animations;

        int m_level = 0; /* position of the widget in the widget tree hierarchy */
        bool m_is_active = true;
        bool m_is_interactable = false;
        bool m_keep_focus = false;
        bool m_is_hovered = false;
        bool m_has_text = false;
        bool m_fit_children = false;

        /** Decides how children are ordered */
        std::unique_ptr<Layout> m_layout;
        gml::Vec2<SizeHint> m_size_hint = gml::Vec2<SizeHint>(SizeHint::EXPANDING);
        gml::Vec2<OffsetHint> m_offset_hint = gml::Vec2<OffsetHint>(OffsetHint::FIXED);

        Anchor m_anchor = Anchor::TOP_LEFT;
        /** margin for layouts */
        gml::Vec2f m_margin;
        /** padding for layouts */
        gml::Vec2f m_padding;
        /** preferred size for layouts */
        gml::Vec2f m_hint_pref_size;
        /** minimal size for layouts */
        gml::Vec2f m_hint_min_size;
        /** maximum size for layouts */
        gml::Vec2f m_hint_max_size;

        /** absolute offset from parent widget */
        gml::Vec2f m_offset;
        /** absolute position of the outer top left edge */
        gml::Vec2f m_position;
        /** absolute total size of the widget */
        gml::Vec2f m_size;
        /** absolute position of the inner top left edge - equal to position for borderless widgets */
        gml::Vec2f m_inner_position;
        /** absolute size of the widget's interior space - equal to size for borderless widgets */
        gml::Vec2f m_inner_size;

        gml::Vec4<float> m_color;
        int m_border_size = 0;
        gml::Vec4<float> m_border_color;
        int m_shadow_offset = 0;
        float m_shadow_hardness = 0.0f;

        TextureAtlasView m_texture_atlas_view;

        TextureAtlasView load_texture(WidgetTextureTemplate tTemplate);

    private:
        std::unique_ptr<gl::IDrawable> m_drawable;

        std::vector<float> construct_coords();

        std::vector<float> construct_colors();

        std::vector<float> construct_tex_coords(TextureAtlasView texture_atlas_view);

        std::vector<float> construct_vertices(TextureAtlasView texture_atlas_view);

        std::vector<unsigned int> construct_indices();

        friend class InputManager;

        friend class Animation;
    };
}
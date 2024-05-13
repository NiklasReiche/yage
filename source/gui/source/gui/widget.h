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

namespace gui
{
    class Master;

    class Widget
    {
    public:
        virtual ~Widget() = default;

        /**
         * Creates a new widget and adds it as a child to this widget. This widget takes ownership of the child.
         * @tparam Element The widget class to instantiate.
         * @tparam Args Constructor parameter types for the new widget.
         * @param args Constructor arguments for the new widget.
         * @return Observer pointer to the new widget.
         */
        template<class Element, typename... Args>
        Element* create_widget(Args... args)
        {
            m_children.push_back(std::make_unique<Element>(this, m_master, std::forward<Args>(args)...));

            if (m_parent == nullptr){
                update_layout();
            }
            else {
                m_parent->update_layout(); // TODO: is this sufficient for updating the whole UI tree?
            }

            return (Element*) m_children.back().get();
        }

        /**
         * Creates a new animation for this widget. This widget takes ownership of the animation.
         * @tparam Element The animation class to instantiate.
         * @tparam Args Constructor parameter types for the animation.
         * @param args Constructor arguments for the animation.
         * @return Observer pointer to the animation.
         */
        template<class Element, typename... Args>
        Element* create_animation(Args... args)
        {
            m_animations.push_back(std::make_unique<Element>(this, m_master, std::forward<Args>(args)...));
            return (Element*) m_animations.back().get();
        }

        virtual font::Text* text()
        {
            return nullptr;
        }

        /**
         * Deactivates this widget and all its children. It will not be rendered or listen to input unless it is
         * activated again. However, this widget is still respected during the layout process.
         */
        void hide()
        {
            m_is_active = false;
        }

        /**
         * Actives this widget, so that it gets rendered and listens to input.
         */
        void show()
        {
            m_is_active = true;
        }

        /**
         * @return Whether this widget is hidden or visible.
         */
        [[nodiscard]]
        bool is_Active() const
        {
            return m_is_active;
        }

        /**
         * @return Whether this widget listens to input.
         */
        [[nodiscard]]
        bool is_Interactable() const
        {
            return m_is_interactable;
        }

        Widget* parent()
        {
            return m_parent;
        }

        const std::vector<std::unique_ptr<Widget>>& children()
        {
            return m_children;
        }

        virtual gml::Vec2f min_size();

        virtual gml::Vec2f max_size();

        virtual gml::Vec2f preferred_size();

        virtual gml::Vec2<SizeHint> size_hint() {
            return m_template.geometry.size_hint;
        }

        virtual gml::Vec2f offset();

        gml::Vec2f actual_size()
        {
            return m_size_abs;
        }

        void setTextureAtlasView(TextureAtlasView texture_atlas_view)
        {
            m_texture_atlas_view = texture_atlas_view;
        }

        virtual void set_template(WidgetTemplate widget_template);

        /* update semantics */
        void update_vertices();

        virtual void update_geometry();

        virtual void update_layout();

        virtual void set_anchor(Anchor anchor);

        void set_actual_size(gml::Vec2f size);

        void set_actual_anchor(Anchor anchor);

        virtual void resize(Size size);

        void set_absolute_offset(gml::Vec2f offset);

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

        virtual void on_char_input([[maybe_unused]] char character)
        {}

        virtual void on_key_press([[maybe_unused]] input::KeyEvent::Code key)
        {}

    protected:
        Master* m_master;
        Widget* m_parent = nullptr;
        std::vector<std::unique_ptr<Widget>> m_children;
        std::vector<std::unique_ptr<Animation>> m_animations;

        bool m_is_active = true;
        bool m_is_interactable = false;
        bool m_keep_focus = false;

        WidgetTemplate m_template;

        TextureAtlasView m_texture_atlas_view;

        /** absolute offset from parent widget */
        gml::Vec2f m_offset_abs;
        /** absolute position of the outer top left edge */
        gml::Vec2f m_position_abs;
        /** absolute total size of the widget */
        gml::Vec2f m_size_abs;
        /** absolute position of the inner top left edge - equal to position for borderless widgets */
        gml::Vec2f m_inner_position_abs;
        /** absolute size of the widget's interior space - equal to size for borderless widgets */
        gml::Vec2f m_inner_size_abs;

        Widget(Widget* parent, Master* master, const WidgetTemplate& widgetTemplate);

        TextureAtlasView load_texture(WidgetTextureTemplate tTemplate);

        float to_absolute_x(float value);

        float to_absolute_y(float value);

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
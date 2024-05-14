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

    /**
     * Base class for all UI elements, i.e. widgets.
     */
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

            if (m_parent == nullptr) {
                update_layout(); // we are the root widget
            } else {
                update_root_layout(); // layout the whole UI tree
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

        /**
         * Deactivates this widget and all its children. It will not be rendered or listen to input unless it is
         * activated again. However, this widget is still respected during the layout process.
         */
        void hide();

        /**
         * Actives this widget, so that it gets rendered and listens to input.
         */
        void show();

        /**
         * Updates the internal geometry values used for constructing the coordinate vertices. May be overridden by
         * subclasses that have additional drawables that depend on screen space positions (e.g. text).
         */
        virtual void update_geometry();

        /**
         * Layouts the children of this widget within its given actual size.
         * The default behaviour is to simply layout children with their preferred size and offset.
         */
        virtual void update_layout();

        /**
         * Updates the whole template for this widget and triggers a layout update.
         */
        virtual void set_template(WidgetTemplate widget_template);

        /**
         * Updates the anchor for this widget and triggers a layout update.
         */
        virtual void set_anchor(Anchor anchor);

        /**
         * Updates the preferred size for this widget and triggers a layout update.
         */
        virtual void resize(Size size);

        void set_actual_size(gml::Vec2f size);

        void set_absolute_offset(gml::Vec2f offset);

        void override_anchor(Anchor anchor);

        /**
         * @return The absolute minimum size of this widget.
         */
        [[nodiscard]]
        virtual gml::Vec2f min_size() const;

        /**
         * @return The absolute maximum size of this widget.
         */
        [[nodiscard]]
        virtual gml::Vec2f max_size() const;

        /**
         * @return The absolute preferred size of this widget.
         */
        [[nodiscard]]
        virtual gml::Vec2f preferred_size() const;

        [[nodiscard]]
        virtual gml::Vec2<SizeHint> size_hint() const;

        /**
         * @return This widget's absolute offset from its parent. The reference point is dependent on the anchor position.
         */
        [[nodiscard]]
        virtual gml::Vec2f offset() const;

        /**
         * @return The absolute size that this widget got assigned during the last layout pass.
         */
        [[nodiscard]]
        gml::Vec2f actual_size() const;

        /**
         * @return Whether this widget is hidden or visible.
         */
        [[nodiscard]]
        bool is_Active() const;

        /**
         * @return Whether this widget listens to input.
         */
        [[nodiscard]]
        bool is_Interactable() const;

        [[nodiscard]]
        const std::vector<std::unique_ptr<Widget>>& children() const;

        [[nodiscard]]
        const gl::IDrawable& drawable() const;

        [[nodiscard]]
        virtual font::Text* text() const;

        TextureAtlasView texture_atlas_view();

        virtual void on_hover();

        virtual void on_hover_release();

        virtual void on_click();

        virtual void on_click_release();

        virtual void on_cancel();

        virtual void on_resume();

        virtual void on_focus();

        virtual void on_focus_release();

        virtual void on_char_input(unsigned int character);

        virtual void on_key_press(input::KeyEvent::Code key);

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

        TextureAtlasView load_texture(const WidgetTextureTemplate& tTemplate);

        [[nodiscard]]
        float to_absolute_x(float value) const;

        [[nodiscard]]
        float to_absolute_y(float value) const;

        void update_vertices();

    private:
        std::unique_ptr<gl::IDrawable> m_drawable;

        [[nodiscard]]
        std::vector<float> construct_coords() const;

        [[nodiscard]]
        std::vector<float> construct_colors() const;

        [[nodiscard]]
        std::vector<float> construct_tex_coords(TextureAtlasView texture_atlas_view) const;

        [[nodiscard]]
        std::vector<float> construct_vertices(TextureAtlasView texture_atlas_view) const;

        [[nodiscard]]
        std::vector<unsigned int> construct_indices() const;

        /**
         * Triggers a layout update on the UI tree root.
         */
        void update_root_layout();

        friend class InputManager;

        friend class Animation;
    };
}
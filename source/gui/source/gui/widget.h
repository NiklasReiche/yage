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
        Widget(Widget* parent, Master* master, const WidgetTemplate& widgetTemplate);

        virtual ~Widget() = default;

        template<typename Element, typename... Args>
        Element* create_widget(Master* master, Args... args) // TODO: this doesn't need the master, we can use m_master
        {
            m_children.push_back(std::make_unique<Element>(this, master, std::forward<Args>(args)...));

            // TODO: correct? defer?
            if (m_parent == nullptr)
                update_layout();
            else
                m_parent->update_layout();

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

        Widget* parent()
        {
            return m_parent;
        }

        const std::vector<std::unique_ptr<Widget>>& children()
        {
            return m_children;
        }

        virtual gml::Vec2f min_size()
        {
            gml::Vec2f size = m_template.geometry.min_size.value;
            if (m_template.geometry.min_size.value_type.x() == ValueType::RELATIVE) {
                size.x() = to_absolute_x(size.x());
            }
            if (m_template.geometry.min_size.value_type.y() == ValueType::RELATIVE) {
                size.y() = to_absolute_y(size.y());
            }
            return size;
        }

        virtual gml::Vec2f max_size()
        {
            gml::Vec2f size = m_template.geometry.max_size.value;
            if (m_template.geometry.max_size.value_type.x() == ValueType::RELATIVE) {
                size.x() = to_absolute_x(size.x());
            }
            if (m_template.geometry.max_size.value_type.y() == ValueType::RELATIVE) {
                size.y() = to_absolute_y(size.y());
            }
            return size;
        }

        virtual gml::Vec2f preferred_size()
        {
            gml::Vec2f size = m_template.geometry.preferred_size.value;

            if (m_template.geometry.size_hint.x() == SizeHint::FIT_CHILDREN) {
                // default behaviour is that children are positioned on top of each other
                float preferred_size = 0;
                for (auto& child : m_children) {
                    preferred_size = std::max(preferred_size, child->preferred_size().x());
                }
                size.x() = preferred_size;
            }
            else if (m_template.geometry.preferred_size.value_type.x() == ValueType::RELATIVE) {
                size.x() = to_absolute_x(size.x());
            }

            if (m_template.geometry.size_hint.y() == SizeHint::FIT_CHILDREN) {
                // default behaviour is that children are positioned on top of each other
                float preferred_size = 0;
                for (auto& child: m_children) {
                    preferred_size = std::max(preferred_size, child->preferred_size().y());
                }
                size.y() = preferred_size;
            }
            else if (m_template.geometry.preferred_size.value_type.y() == ValueType::RELATIVE) {
                size.y() = to_absolute_y(size.y());
            }

            // TODO: should this incorporate shadow?
            return size + gml::Vec2f(2 * m_template.border.thickness);
        }

        virtual gml::Vec2<SizeHint> size_hint() {
            return m_template.geometry.size_hint;
        }

        virtual gml::Vec2f offset()
        {
            gml::Vec2f offset = m_template.geometry.anchor.offset;
            if (m_template.geometry.anchor.value_type.x() == ValueType::RELATIVE) {
                offset.x() = to_absolute_x(offset.x());
            }
            if (m_template.geometry.anchor.value_type.y() == ValueType::RELATIVE) {
                offset.y() = to_absolute_y(offset.y());
            }
            return offset;
        }

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
#include "widget.h"
#include "master.h"
#include "image/enum.h"
#include <image/img.h>

#include <utility>

namespace gui
{
    Widget::Widget(Widget* parent, Master* master, const WidgetTemplate& wTemplate)
            : m_master(master), m_parent(parent),
              m_template(wTemplate),
              m_texture_atlas_view(load_texture(wTemplate.texture))
    {
        m_inner_position_abs = m_position_abs + gml::Vec2f(m_template.border.thickness);
        m_inner_size_abs = m_size_abs - gml::Vec2f(m_template.border.thickness * 2.0f);

        std::vector<unsigned int> indices = construct_indices();
        std::vector<float> vertices = construct_vertices(m_texture_atlas_view);
        m_drawable = master->gl_context().getDrawableCreator()
                ->createDrawable(vertices, indices, {2, 4, 2}, gl::VertexFormat::BATCHED);

        Widget::update_geometry();
    }

    std::vector<unsigned int> Widget::construct_indices() const
    {
        std::vector<unsigned int> indices;
        unsigned index = 0;

        if (m_template.shadow.offset > 0) {
            for (int i = 0; i < 6; ++i) {
                indices.push_back(index + i);
            }
            index += 6;
        }

        if (m_template.border.thickness > 0) {
            for (int i = 0; i < 24; ++i) {
                indices.push_back(index + i);
            }
            index += 24;
        }

        for (int i = 0; i < 6; ++i) {
            indices.push_back(index + i);
        }
        return indices;
    }

    std::vector<float> Widget::construct_coords() const
    {
        std::vector<float> vertices;

        float left = m_position_abs.x();
        float top = m_position_abs.y();
        float right = m_position_abs.x() + m_size_abs.x();
        float bottom = m_position_abs.y() + m_size_abs.y();

        float shadow_offset = m_template.shadow.offset;
        float border_size = m_template.border.thickness;

        if (shadow_offset != 0) {
            std::array<float, 12> pos_shadow = {
                    left + shadow_offset, top + shadow_offset,
                    left + shadow_offset, bottom + shadow_offset,
                    right + shadow_offset, bottom + shadow_offset,
                    right + shadow_offset, bottom + shadow_offset,
                    right + shadow_offset, top + shadow_offset,
                    left + shadow_offset, top + shadow_offset,
            };
            vertices.insert(std::end(vertices), std::begin(pos_shadow), std::end(pos_shadow));
        }

        if (border_size != 0) {
            // Check if inline border
            border_size *= -1;
            if (border_size < 0) {
                border_size *= -1;
                left += border_size;
                top += border_size;
                right -= border_size;
                bottom -= border_size;
            }

            std::array<float, 48> pos_border = {
                    left, top,
                    left - border_size, top - border_size,
                    right + border_size, top - border_size,
                    right + border_size, top - border_size,
                    right, top,
                    left, top,

                    right, top,
                    right + border_size, top - border_size,
                    right + border_size, bottom + border_size,
                    right + border_size, bottom + border_size,
                    right, bottom,
                    right, top,

                    right, bottom,
                    right + border_size, bottom + border_size,
                    left - border_size, bottom + border_size,
                    left - border_size, bottom + border_size,
                    left, bottom,
                    right, bottom,

                    left, bottom,
                    left - border_size, bottom + border_size,
                    left - border_size, top - border_size,
                    left - border_size, top - border_size,
                    left, top,
                    left, bottom
            };
            vertices.insert(std::end(vertices), std::begin(pos_border), std::end(pos_border));
        }

        std::array<float, 12> pos_plain = {
                left, top,
                left, bottom,
                right, bottom,
                right, bottom,
                right, top,
                left, top,
        };
        vertices.insert(std::end(vertices), std::begin(pos_plain), std::end(pos_plain));

        return vertices;
    }

    std::vector<float> Widget::construct_colors() const
    {
        std::vector<float> vertices;

        gml::Vec4<float> color = gl::toVec4(m_template.color);
        gml::Vec4<float> border_color = gl::toVec4(m_template.border.color);
        float shadow_hardness = m_template.shadow.hardness;

        if (m_template.shadow.offset != 0) {
            for (int i = 0; i < 6; ++i) {
                vertices.insert(std::end(vertices), {0.0f, 0.0f, 0.0f, shadow_hardness});
            }
        }

        if (m_template.border.thickness > 0) {
            for (int i = 0; i < 24; ++i) {
                vertices.insert(std::end(vertices),
                                {border_color.x(), border_color.y(), border_color.z(), border_color.w()});
            }
        }

        for (int i = 0; i < 6; ++i) {
            vertices.insert(std::end(vertices), {color.x(), color.y(), color.z(), color.w()});
        }

        return vertices;
    }

    std::vector<float> Widget::construct_tex_coords(TextureAtlasView texture_atlas_view) const
    {
        std::vector<float> vertices;

        TextureAtlasView transparent = m_master->texture_atlas_store().transparent();
        auto left = transparent.coordinates_start().x();
        auto right = transparent.coordinates_end().x();
        auto top = transparent.coordinates_start().y();
        auto bottom = transparent.coordinates_end().y();

        if (m_template.shadow.offset != 0) {
            std::array<float, 12> tex_shadow = {
                    left, top,
                    left, bottom,
                    right, bottom,
                    right, bottom,
                    right, top,
                    left, top,
            };
            vertices.insert(std::end(vertices), std::begin(tex_shadow), std::end(tex_shadow));
        }

        if (m_template.border.thickness > 0) {
            std::array<float, 48> tex_border = {
                    left, top,
                    left, bottom,
                    right, bottom,
                    right, bottom,
                    right, top,
                    left, top,

                    left, top,
                    left, bottom,
                    right, bottom,
                    right, bottom,
                    right, top,
                    left, top,

                    left, top,
                    left, bottom,
                    right, bottom,
                    right, bottom,
                    right, top,
                    left, top,

                    left, top,
                    left, bottom,
                    right, bottom,
                    right, bottom,
                    right, top,
                    left, top,
            };
            vertices.insert(std::end(vertices), std::begin(tex_border), std::end(tex_border));
        }

        std::array<float, 12> tex_plain = {
                texture_atlas_view.coordinates_start().x(), texture_atlas_view.coordinates_start().y(),
                texture_atlas_view.coordinates_start().x(), texture_atlas_view.coordinates_end().y(),
                texture_atlas_view.coordinates_end().x(), texture_atlas_view.coordinates_end().y(),
                texture_atlas_view.coordinates_end().x(), texture_atlas_view.coordinates_end().y(),
                texture_atlas_view.coordinates_end().x(), texture_atlas_view.coordinates_start().y(),
                texture_atlas_view.coordinates_start().x(), texture_atlas_view.coordinates_start().y(),
        };
        vertices.insert(std::end(vertices), std::begin(tex_plain), std::end(tex_plain));

        return vertices;
    }

    std::vector<float> Widget::construct_vertices(TextureAtlasView texture_atlas_view) const
    {
        std::vector<float> vertices;

        auto coords = construct_coords();
        vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

        auto colors = construct_colors();
        vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));

        auto tex_coords = construct_tex_coords(texture_atlas_view);
        vertices.insert(std::end(vertices), std::begin(tex_coords), std::end(tex_coords));

        return vertices;
    }

    TextureAtlasView Widget::load_texture(const WidgetTextureTemplate& tTemplate)
    {
        if (!tTemplate.filename.empty()) {
            auto reader = m_master->window().getFileReader();
            auto file = reader->openBinaryFile(tTemplate.filename, platform::IFile::AccessMode::READ);
            img::Image image = img::readFromFile(*file, img::FORCE_CHANNELS::RGBA);
            return m_master->texture_atlas_store().add(image);
        } else {
            return m_master->texture_atlas_store().transparent();
        }
    }

    void Widget::update_vertices()
    {
        if (m_drawable != nullptr) {
            std::vector<float> vertices = construct_vertices(m_texture_atlas_view);
            m_drawable->setSubData(0, vertices);
        }
    }

    void Widget::update_layout()
    {
        // the default behaviour is to layout children with their preferred size and offset
        for (auto& child: m_children) {
            child->set_actual_size(child->preferred_size());
            child->set_absolute_offset(child->offset());
            child->update_layout();
        }
    }

    void Widget::update_geometry()
    {
        gml::Vec2f lastPosition = m_position_abs;

        if (m_parent != nullptr) {
            switch (m_template.geometry.anchor.position) {
                case AnchorPosition::TOP_LEFT:
                    m_position_abs = m_parent->m_inner_position_abs + m_offset_abs;
                    break;

                case AnchorPosition::BOTTOM_LEFT:
                    m_position_abs.x() = m_parent->m_inner_position_abs.x() + m_offset_abs.x();
                    m_position_abs.y() =
                            m_parent->m_inner_position_abs.y() + m_parent->m_inner_position_abs.y() -
                            (m_offset_abs.y() + m_size_abs.y());
                    break;

                case AnchorPosition::TOP_RIGHT:
                    m_position_abs.x() =
                            (m_parent->m_inner_position_abs.x() + m_parent->m_inner_position_abs.x()) -
                            (m_offset_abs.x() + m_size_abs.x());
                    m_position_abs.y() = m_parent->m_inner_position_abs.y() + m_offset_abs.y();
                    break;

                case AnchorPosition::BOTTOM_RIGHT:
                    m_position_abs.x() =
                            m_parent->m_inner_position_abs.x() + m_parent->m_inner_size_abs.x() -
                            (m_offset_abs.x() + m_size_abs.x());
                    m_position_abs.y() =
                            m_parent->m_inner_position_abs.y() + m_parent->m_inner_size_abs.y() -
                            (m_offset_abs.y() + m_size_abs.y());
                    break;

                case AnchorPosition::CENTER:
                    m_position_abs = m_parent->m_inner_position_abs + m_parent->m_inner_size_abs / 2.0f + m_offset_abs;
                    break;
            }

            m_inner_position_abs = m_position_abs + gml::Vec2f((float) m_template.border.thickness);
        }

        if (lastPosition != m_position_abs) {
            for (auto& child: m_children) {
                child->update_geometry();
            }
        }

        update_vertices();
    }

    void Widget::set_actual_size(gml::Vec2f size)
    {
        if (m_size_abs != size) {
            m_size_abs = size;
            m_inner_size_abs = size - gml::Vec2f(m_template.border.thickness * 2.0f);
            update_geometry();
        }
    }

    void Widget::override_anchor(Anchor anchor)
    {
        if (m_template.geometry.anchor != anchor) {
            m_template.geometry.anchor = anchor;
            update_geometry();
        }
    }

    void Widget::set_absolute_offset(gml::Vec2f offset)
    {
        if (m_offset_abs != offset) {
            m_offset_abs = offset;
            update_geometry();
        }
    }

    void Widget::resize(Size size)
    {
        m_template.geometry.preferred_size = size;
        update_root_layout();
    }

    void Widget::set_anchor(Anchor anchor)
    {
        m_template.geometry.anchor = anchor;
        update_root_layout();
    }

    void Widget::set_template(WidgetTemplate widget_template)
    {
        m_template = std::move(widget_template);
        update_root_layout();
    }

    float Widget::to_absolute_x(float value) const
    {
        return value * m_parent->m_size_abs.x();
    }

    float Widget::to_absolute_y(float value) const
    {
        return value * m_parent->m_size_abs.y();
    }

    TextureAtlasView Widget::texture_atlas_view()
    {
        return m_texture_atlas_view;
    }

    const gl::IDrawable& Widget::drawable() const
    {
        return *m_drawable;
    }

    gml::Vec2f Widget::offset() const
    {
        gml::Vec2f offset = m_template.geometry.anchor.offset;
        if (m_template.geometry.anchor.value_type.x() == ValueType::RELATIVE_TO_PARENT) {
            offset.x() = to_absolute_x(offset.x());
        }
        if (m_template.geometry.anchor.value_type.y() == ValueType::RELATIVE_TO_PARENT) {
            offset.y() = to_absolute_y(offset.y());
        }
        return offset;
    }

    gml::Vec2f Widget::preferred_size() const
    {
        // TODO: it might be worth it to save these calculations for memoization
        gml::Vec2f size = m_template.geometry.preferred_size.value;

        if (m_template.geometry.size_hint.x() == SizeHint::FIT_CHILDREN) {
            // default behaviour is that children are positioned on top of each other
            float preferred_size = 0;
            for (auto& child: m_children) {
                preferred_size = std::max(preferred_size, child->preferred_size().x());
            }
            size.x() = preferred_size;
        } else if (m_template.geometry.preferred_size.value_type.x() == ValueType::RELATIVE_TO_PARENT) {
            size.x() = to_absolute_x(size.x());
        }

        if (m_template.geometry.size_hint.y() == SizeHint::FIT_CHILDREN) {
            // default behaviour is that children are positioned on top of each other
            float preferred_size = 0;
            for (auto& child: m_children) {
                preferred_size = std::max(preferred_size, child->preferred_size().y());
            }
            size.y() = preferred_size;
        } else if (m_template.geometry.preferred_size.value_type.y() == ValueType::RELATIVE_TO_PARENT) {
            size.y() = to_absolute_y(size.y());
        }

        // TODO: should this incorporate shadow?
        return size + gml::Vec2f(2 * m_template.border.thickness);
    }

    gml::Vec2f Widget::min_size() const
    {
        gml::Vec2f size = m_template.geometry.min_size.value;
        if (m_template.geometry.min_size.value_type.x() == ValueType::RELATIVE_TO_PARENT) {
            size.x() = to_absolute_x(size.x());
        }
        if (m_template.geometry.min_size.value_type.y() == ValueType::RELATIVE_TO_PARENT) {
            size.y() = to_absolute_y(size.y());
        }
        return size;
    }

    gml::Vec2f Widget::max_size() const
    {
        gml::Vec2f size = m_template.geometry.max_size.value;
        if (m_template.geometry.max_size.value_type.x() == ValueType::RELATIVE_TO_PARENT) {
            size.x() = to_absolute_x(size.x());
        }
        if (m_template.geometry.max_size.value_type.y() == ValueType::RELATIVE_TO_PARENT) {
            size.y() = to_absolute_y(size.y());
        }
        return size;
    }

    void Widget::update_root_layout()
    {
        m_master->root().update_layout();
    }

    bool Widget::is_Active() const
    {
        return m_is_active;
    }

    bool Widget::is_Interactable() const
    {
        return m_is_interactable;
    }

    font::Text* Widget::text() const
    {
        return nullptr;
    }

    const std::vector<std::unique_ptr<Widget>>& Widget::children() const
    {
        return m_children;
    }

    void Widget::on_hover()
    {
    }

    void Widget::on_hover_release()
    {
    }

    void Widget::on_click()
    {
    }

    void Widget::on_click_release()
    {
    }

    void Widget::on_cancel()
    {
    }

    void Widget::on_resume()
    {
    }

    void Widget::on_focus()
    {
    }

    void Widget::on_focus_release()
    {
    }

    void Widget::on_char_input(char)
    {
    }

    void Widget::on_key_press(input::KeyEvent::Code)
    {
    }

    gml::Vec2f Widget::actual_size() const
    {
        return m_size_abs;
    }

    gml::Vec2<SizeHint> Widget::size_hint() const
    {
        return m_template.geometry.size_hint;
    }

    void Widget::hide()
    {
        m_is_active = false;
    }

    void Widget::show()
    {
        m_is_active = true;
    }
}

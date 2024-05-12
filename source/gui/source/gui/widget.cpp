#include "widget.h"
#include "master.h"
#include "image/enum.h"
#include <core/platform/Window.h>
#include <image/img.h>

namespace gui
{
	Widget::Widget(Widget * parent, Master* master, const WidgetTemplate & wTemplate)
		: m_master(master), m_parent(parent),
          m_template(wTemplate),
          m_texture_atlas_view(load_texture(wTemplate.texture))
	{
		this->m_inner_position_abs = m_position_abs + gml::Vec2f((float) m_template.border.thickness);
		this->m_inner_size_abs = m_size_abs - gml::Vec2f((float) m_template.border.thickness * 2);

        std::vector<unsigned int> indices = construct_indices();
		std::vector<float> vertices = construct_vertices(m_texture_atlas_view);
        m_drawable = master->gl_context().getDrawableCreator()
                ->createDrawable(vertices, indices, std::vector<unsigned int> { 2, 4, 2 }, gl::VertexFormat::BATCHED);

        update_geometry();
	}

    std::vector<unsigned int> Widget::construct_indices()
    {
        int index = -1;

        std::vector<unsigned int> indices;
        if (m_template.shadow.offset > 0) {
            std::array<unsigned int, 6> indices_shadow = {
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index)
            };
            indices.insert(std::end(indices), std::begin(indices_shadow), std::end(indices_shadow));
        }

        if (m_template.border.thickness > 0) {
            std::array<unsigned int, 24> indices_border = {
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
            };
            indices.insert(std::end(indices), std::begin(indices_border), std::end(indices_border));
        }

        std::array<unsigned int, 6> indices_plain = {
                static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                static_cast<unsigned int>(++index), static_cast<unsigned int>(++index)
        };
        indices.insert(std::end(indices), std::begin(indices_plain), std::end(indices_plain));
        return indices;
    }

    std::vector<float> Widget::construct_coords()
	{
        std::vector<float> vertices;

		float left = m_position_abs.x();
		float top = m_position_abs.y();
		float right = m_position_abs.x() + m_size_abs.x();
		float bottom = m_position_abs.y() + m_size_abs.y();

		int so = m_template.shadow.offset;
		int bs = m_template.border.thickness;

		if (so != 0)
		{
			std::array<float, 12> pos_shadow = {
				left + so,	top + so,
				left + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	top + so,
				left + so,	top + so,
			};
            vertices.insert(std::end(vertices), std::begin(pos_shadow), std::end(pos_shadow));
		}

		if (bs != 0)
		{
			// Check if inline border
			bs *= -1;
			if (bs < 0)
			{
				bs *= -1;
				left += bs;
				top += bs;
				right -= bs;
				bottom -= bs;
			}

			std::array<float, 48> pos_border = {
				left,		top,
				left - bs,	top - bs,
				right + bs,	top - bs,
				right + bs,	top - bs,
				right,		top,
				left,		top,

				right,		top,
				right + bs,	top - bs,
				right + bs,	bottom + bs,
				right + bs,	bottom + bs,
				right,		bottom,
				right,		top,

				right,		bottom,
				right + bs,	bottom + bs,
				left - bs,	bottom + bs,
				left - bs,	bottom + bs,
				left,		bottom,
				right,		bottom,

				left,		bottom,
				left - bs,	bottom + bs,
				left - bs,	top - bs,
				left - bs,	top - bs,
				left,		top,
				left,		bottom
			};
			vertices.insert(std::end(vertices), std::begin(pos_border), std::end(pos_border));
		}

		std::array<float, 12> pos_plain = {
			left,	top,
			left,	bottom,
			right,	bottom,
			right,	bottom,
			right,	top,
			left,	top,
		};
		vertices.insert(std::end(vertices), std::begin(pos_plain), std::end(pos_plain));

        return vertices;
	}

    std::vector<float> Widget::construct_colors()
	{
        std::vector<float> vertices;

		gml::Vec4<float> gl_color = gl::toVec4(m_template.color);
		gml::Vec4<float> gl_bc = gl::toVec4(m_template.border.color);
		float sh = m_template.shadow.hardness;

		if (m_template.shadow.offset != 0)
		{
			std::array<float, 24> color_shadow = {
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
			};
			vertices.insert(std::end(vertices), std::begin(color_shadow), std::end(color_shadow));
		}

		if (m_template.border.thickness > 0)
		{
			std::array<float, 96> color_border = {
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),

				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),

				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),

				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w(),
				gl_bc.x(), gl_bc.y(), gl_bc.z(), gl_bc.w()
			};
			vertices.insert(std::end(vertices), std::begin(color_border), std::end(color_border));
		}

		std::array<float, 24> color_plain = {
			gl_color.x(), gl_color.y(), gl_color.z(), gl_color.w(),
			gl_color.x(), gl_color.y(), gl_color.z(), gl_color.w(),
			gl_color.x(), gl_color.y(), gl_color.z(), gl_color.w(),
			gl_color.x(), gl_color.y(), gl_color.z(), gl_color.w(),
			gl_color.x(), gl_color.y(), gl_color.z(), gl_color.w(),
			gl_color.x(), gl_color.y(), gl_color.z(), gl_color.w()
		};
		vertices.insert(std::end(vertices), std::begin(color_plain), std::end(color_plain));

        return vertices;
	}

    std::vector<float> Widget::construct_tex_coords(TextureAtlasView texture_atlas_view)
	{
        std::vector<float> vertices;

        TextureAtlasView transparent = m_master->texture_atlas_store().transparent();
        auto left = transparent.coordinates_start().x();
        auto right = transparent.coordinates_end().x();
        auto top = transparent.coordinates_start().y();
        auto bottom = transparent.coordinates_end().y();

		if (m_template.shadow.offset != 0)
		{
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

    std::vector<float> Widget::construct_vertices(TextureAtlasView texture_atlas_view)
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

    TextureAtlasView Widget::load_texture(WidgetTextureTemplate tTemplate)
	{
		if (tTemplate.filename.size() > 0) {
            auto reader = m_master->window().getFileReader();
            auto file = reader->openBinaryFile(tTemplate.filename, platform::IFile::AccessMode::READ);
			img::Image image = img::readFromFile(*file, img::FORCE_CHANNELS::RGBA);
			return m_master->texture_atlas_store().add(image);
		}
		else {
			return m_master->texture_atlas_store().transparent();
		}
	}


	void Widget::update_vertices()
	{
        if (m_drawable != nullptr){
            std::vector<float> vertices = construct_vertices(m_texture_atlas_view);
            m_drawable->setSubData(0, vertices);
        }
	}
	
	void Widget::update_layout()
	{
        // the default behaviour is to layout children with their preferred size and offset
        for (auto& child : m_children) {
            child->set_actual_size(child->preferred_size());
            child->set_absolute_offset(child->offset());
        }
	}

    void Widget::update_geometry()
    {
        gml::Vec2f lastPosition = m_position_abs;

        if (m_parent != nullptr) {
            switch (m_template.geometry.anchor.position) {
                case AnchorPosition::TOP_LEFT:
                    m_position_abs = m_parent->m_inner_position_abs + (m_offset_abs);
                    break;

                case AnchorPosition::BOTTOM_LEFT:
                    m_position_abs.x() = m_parent->m_inner_position_abs.x() + (m_offset_abs).x();
                    m_position_abs.y() =
                            (m_parent->m_inner_position_abs.y() + m_parent->m_inner_position_abs.y()) - ((m_offset_abs).y() + m_size_abs.y());
                    break;

                case AnchorPosition::TOP_RIGHT:
                    m_position_abs.x() =
                            (m_parent->m_inner_position_abs.x() + m_parent->m_inner_position_abs.x()) - ((m_offset_abs).x() + m_size_abs.x());
                    m_position_abs.y() = m_parent->m_inner_position_abs.y() + (m_offset_abs).y();
                    break;

                case AnchorPosition::BOTTOM_RIGHT:
                    m_position_abs.x() =
                            (m_parent->m_inner_position_abs.x() + m_parent->m_inner_size_abs.x()) - ((m_offset_abs).x() + m_size_abs.x());
                    m_position_abs.y() =
                            (m_parent->m_inner_position_abs.y() + m_parent->m_inner_size_abs.y()) - ((m_offset_abs).y() + m_size_abs.y());
                    break;

                case AnchorPosition::CENTER:
                    m_position_abs = m_parent->m_inner_position_abs + m_parent->m_inner_size_abs / 2.0f + (m_offset_abs);
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
		if (this->m_size_abs != size) {
			this->m_size_abs = size;
			this->m_inner_size_abs = size - gml::Vec2f((float)m_template.border.thickness) * 2.0f;
            update_geometry();
			update_layout(); // TODO: maybe make this explicit
		}
	}

    void Widget::set_actual_anchor(Anchor anchor)
    {
        m_template.geometry.anchor = anchor;
        update_geometry();
        update_layout(); // TODO: maybe make this explicit
    }

    void Widget::set_absolute_offset(gml::Vec2f offset)
    {
        if (this->m_offset_abs != offset) {
            this->m_offset_abs = offset;
            update_geometry();
        }
    }

	void Widget::resize(Size size)
	{
        m_template.geometry.preferred_size = size;
        m_parent->update_layout();
	}

    void Widget::set_anchor(Anchor anchor)
    {
        m_template.geometry.anchor = anchor;
        m_parent->update_layout();
    }

	float Widget::to_absolute_x(float value)
	{
		return value * m_parent->m_size_abs.x();
	}

	float Widget::to_absolute_y(float value)
	{
		return value * m_parent->m_size_abs.y();
	}

    TextureAtlasView Widget::texture_atlas_view()
    {
        return m_texture_atlas_view;
    }

    gl::IDrawable& Widget::drawable()
    {
        return *m_drawable;
    }
}

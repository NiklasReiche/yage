#include "widget.h"
#include "master.h"
#include "image/enum.h"
#include <core/platform/Window.h>
#include <image/img.h>

namespace gui
{
	Widget::Widget(Widget * parent, Master* master, const WidgetTemplate & wTemplate)
		: m_master(master), m_parent(parent),
          m_size_hint(wTemplate.geometry.sizeHint),
          m_offset_hint(wTemplate.geometry.offsetHint),
          m_anchor(wTemplate.geometry.anchor),
          m_margin(wTemplate.geometry.offset),
          m_hint_pref_size(wTemplate.geometry.size),
          m_color(gl::toVec4(wTemplate.color)),
          m_border_size(wTemplate.border.size),
          m_border_color(gl::toVec4(wTemplate.border.color)),
          m_shadow_offset(wTemplate.shadow.offset),
          m_shadow_hardness(wTemplate.shadow.hardness),
          m_texture_atlas_view(load_texture(wTemplate.texture))
	{
		this->m_inner_position = m_position + gml::Vec2f((float)m_border_size);
		this->m_inner_size = m_size - gml::Vec2f((float)m_border_size * 2);

		this->m_layout = std::make_unique<Layout>();

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
        if (m_shadow_offset > 0) {
            std::array<unsigned int, 6> indices_shadow = {
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index)
            };
            indices.insert(std::end(indices), std::begin(indices_shadow), std::end(indices_shadow));
        }

        if (m_border_size > 0) {
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

		float left = m_position.x();
		float top = m_position.y();
		float right = m_position.x() + m_size.x();
		float bottom = m_position.y() + m_size.y();

		int so = m_shadow_offset;
		int bs = m_border_size;

        int index = -1;

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

		gml::Vec4<float> gl_color = m_color;
		gml::Vec4<float> gl_bc = m_border_color;
		float sh = m_shadow_hardness;

		if (m_shadow_offset != 0)
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

		if (m_border_size != 0)
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

		if (m_shadow_offset != 0)
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

		if (m_border_size != 0) {
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
		if (m_fit_children) {
            m_hint_pref_size = calcPrefSize();
            m_parent->update_layout();
		}
		m_layout->update(this);
	}

    void Widget::update_geometry()
    {
        gml::Vec2f lastPosition = m_position;

        if (m_parent != nullptr) {
            switch (m_anchor) {
                case Anchor::TOP_LEFT:
                    m_position = m_parent->getInnerPosition() + (m_offset);
                    break;

                case Anchor::BOTTOM_LEFT:
                    m_position.x() = m_parent->getInnerPosition().x() + (m_offset).x();
                    m_position.y() =
                            (m_parent->getInnerPosition().y() + m_parent->getInnerSize().y()) - ((m_offset).y() + m_size.y());
                    break;

                case Anchor::TOP_RIGHT:
                    m_position.x() =
                            (m_parent->getInnerPosition().x() + m_parent->getInnerSize().x()) - ((m_offset).x() + m_size.x());
                    m_position.y() = m_parent->getInnerPosition().y() + (m_offset).y();
                    break;

                case Anchor::BOTTOM_RIGHT:
                    m_position.x() =
                            (m_parent->getInnerPosition().x() + m_parent->getInnerSize().x()) - ((m_offset).x() + m_size.x());
                    m_position.y() =
                            (m_parent->getInnerPosition().y() + m_parent->getInnerSize().y()) - ((m_offset).y() + m_size.y());
                    break;

                case Anchor::CENTER:
                    m_position = m_parent->getInnerPosition() + m_parent->getInnerSize() / 2.0f + (m_offset);
                    break;
            }

            m_inner_position = m_position + gml::Vec2f((float) m_border_size);
        }

        if (lastPosition != m_position) {
            for (auto& child: m_children) {
                child->update_geometry();
            }
        }

        update_vertices();
    }

	void Widget::set_size(gml::Vec2f _size)
	{
		if (this->m_size != _size) {
			this->m_size = _size;
			this->m_inner_size = _size - gml::Vec2f((float)m_border_size) * 2.0f;
            update_geometry();
			m_layout->update(this);
		}
	}

	void Widget::resize(gml::Vec2f _size)
	{
		if (this->m_hint_pref_size != _size) {
            m_hint_pref_size = _size;
            m_parent->update_layout();
			m_layout->update(this);
		}
	}

	void Widget::set_offset(gml::Vec2f _offset)
	{
		if (this->m_offset != _offset) {
			this->m_offset = _offset;
            update_geometry();
		}
	}

	void Widget::move(gml::Vec2f _cellMargin)
	{
		if (this->m_margin != _cellMargin) {
			this->m_margin = _cellMargin;
            m_parent->update_layout();
			m_layout->update(this);
		}
	}

	void Widget::set_anchor(Anchor _anchor)
	{
		if (this->m_anchor != _anchor) {
			this->m_anchor = _anchor;
            update_geometry();
		}
	}




	gml::Vec2f Widget::to_absolute(gml::Vec2f value)
	{
		gml::Vec2f vec;
		gml::Vec2f parentSize = m_parent->getSize();
		vec.x() = value.x() * parentSize.x();
		vec.y() = value.y() * parentSize.y();
		return vec;
	}
	float Widget::to_absolute_x(float value)
	{
		return value * m_parent->getSize().x();
	}
	float Widget::to_absolute_y(float value)
	{
		return value * m_parent->getSize().y();
	}

	float Widget::from_aspect()
	{
		if (m_size_hint.x() == SizeHint::INFINITE) {
			return to_absolute_x(m_hint_pref_size.x()) * m_hint_pref_size.y();
		}
		else if (m_size_hint.y() == SizeHint::INFINITE) {
			return m_hint_pref_size.x() * to_absolute_y(m_hint_pref_size.y());
		}
		else {
			return m_hint_pref_size.x() * m_hint_pref_size.y();
		}
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

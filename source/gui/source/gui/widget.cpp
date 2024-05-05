#include "widget.h"
#include "master.h"
#include "image/enum.h"
#include <core/platform/Window.h>
#include <image/img.h>

namespace gui
{
	Widget::Widget(Widget * parent, Master* master, const WidgetTemplate & wTemplate)
		: master(master), parent(parent),
		sizeHint(wTemplate.geometry.sizeHint),
		offsetHint(wTemplate.geometry.offsetHint),
		anchor(wTemplate.geometry.anchor),
		cellMargin(wTemplate.geometry.offset),
		prefSize(wTemplate.geometry.size),
		color(gl::toVec4(wTemplate.color)),
		borderSize(wTemplate.border.size),
		borderColor(gl::toVec4(wTemplate.border.color)),
		shadowOffset(wTemplate.shadow.offset),
		shadowHardness(wTemplate.shadow.hardness),
        m_texture_atlas_view(loadTexture(wTemplate.texture))
	{
		this->innerPosition = position + gml::Vec2f((float)borderSize);
		this->innerSize = size - gml::Vec2f((float)borderSize * 2);

		this->m_layout = std::make_unique<Layout>();

        std::vector<unsigned int> indices = construct_indices();
		std::vector<float> vertices = constructVertices(m_texture_atlas_view);
		drawable = master->gl_context().getDrawableCreator()
                ->createDrawable(vertices, indices, std::vector<unsigned int> { 2, 4, 2 }, gl::VertexFormat::BATCHED);

        update_geometry();
	}

    std::vector<unsigned int> Widget::construct_indices()
    {
        int index = -1;

        std::vector<unsigned int> indices;
        if (shadowOffset > 0) {
            std::array<unsigned int, 6> indices_shadow = {
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index),
                    static_cast<unsigned int>(++index), static_cast<unsigned int>(++index)
            };
            indices.insert(std::end(indices), std::begin(indices_shadow), std::end(indices_shadow));
        }

        if (borderSize > 0) {
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

    std::vector<float> Widget::constructCoords()
	{
        std::vector<float> vertices;

		float left = position.x();
		float top = position.y();
		float right = position.x() + size.x();
		float bottom = position.y() + size.y();

		int so = shadowOffset;
		int bs = borderSize;

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

    std::vector<float> Widget::constructColors()
	{
        std::vector<float> vertices;

		gml::Vec4<float> gl_color = color;
		gml::Vec4<float> gl_bc = borderColor;
		float sh = shadowHardness;

		if (shadowOffset != 0)
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

		if (borderSize != 0)
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

    std::vector<float> Widget::constructTexCoords(TextureAtlasView texture_atlas_view)
	{
        std::vector<float> vertices;

        TextureAtlasView transparent = master->texture_atlas_store().transparent();
        auto left = transparent.coordinates_start().x();
        auto right = transparent.coordinates_end().x();
        auto top = transparent.coordinates_start().y();
        auto bottom = transparent.coordinates_end().y();

		if (shadowOffset != 0)
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

		if (borderSize != 0) {
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

    std::vector<float> Widget::constructVertices(TextureAtlasView texture_atlas_view)
	{
        std::vector<float> vertices;

        auto coords = constructCoords();
        vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));

		auto colors = constructColors();
        vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));

		auto tex_coords = constructTexCoords(texture_atlas_view);
        vertices.insert(std::end(vertices), std::begin(tex_coords), std::end(tex_coords));

        return vertices;
	}

    TextureAtlasView Widget::loadTexture(WidgetTextureTemplate tTemplate)
	{
		if (tTemplate.filename.size() > 0) {
            auto reader = master->window().getFileReader();
            auto file = reader->openBinaryFile(tTemplate.filename, platform::IFile::AccessMode::READ);
			img::Image image = img::readFromFile(*file, img::FORCE_CHANNELS::RGBA);
			return master->texture_atlas_store().add(image);
		}
		else {
			return master->texture_atlas_store().transparent();
		}
	}


	void Widget::update_parameters()
	{
        if (drawable != nullptr){
            std::vector<float> vertices = constructVertices(m_texture_atlas_view);
            drawable->setSubData(0, vertices);
        }
	}
	
	void Widget::update_layout()
	{
		if (fitChildren) {
			prefSize = calcPrefSize();
            parent->update_layout();
		}
		m_layout->update(this);
	}

	void Widget::setSize(gml::Vec2f _size)
	{
		if (this->size != _size) {
			this->size = _size;
			this->innerSize = _size - gml::Vec2f((float)borderSize) * 2.0f;
            update_geometry();
			m_layout->update(this);
		}
	}
	void Widget::resize(gml::Vec2f _size)
	{
		if (this->prefSize != _size) {
			prefSize = _size;
            parent->update_layout();
			m_layout->update(this);
		}
	}
	void Widget::setOffset(gml::Vec2f _offset)
	{
		if (this->offset != _offset) {
			this->offset = _offset;
            update_geometry();
		}
	}
	void Widget::move(gml::Vec2f _cellMargin)
	{
		if (this->cellMargin != _cellMargin) {
			this->cellMargin = _cellMargin;
            parent->update_layout();
			m_layout->update(this);
		}
	}
	void Widget::setAnchor(Anchor _anchor)
	{
		if (this->anchor != _anchor) {
			this->anchor = _anchor;
            update_geometry();
		}
	}

	void Widget::update_geometry()
	{
		gml::Vec2f lastPosition = position;

		if (parent != nullptr) {
			switch (anchor)
			{
			case Anchor::TOP_LEFT:
				position = parent->getInnerPosition() + (offset);
				break;

			case Anchor::BOTTOM_LEFT:
				position.x() = parent->getInnerPosition().x() + (offset).x();
				position.y() = (parent->getInnerPosition().y() + parent->getInnerSize().y()) - ((offset).y() + size.y());
				break;

			case Anchor::TOP_RIGHT:
				position.x() = (parent->getInnerPosition().x() + parent->getInnerSize().x()) - ((offset).x() + size.x());
				position.y() = parent->getInnerPosition().y() + (offset).y();
				break;

			case Anchor::BOTTOM_RIGHT:
				position.x() = (parent->getInnerPosition().x() + parent->getInnerSize().x()) - ((offset).x() + size.x());
				position.y() = (parent->getInnerPosition().y() + parent->getInnerSize().y()) - ((offset).y() + size.y());
				break;

			case Anchor::CENTER:
				position = parent->getInnerPosition() + parent->getInnerSize() / 2.0f + (offset);
				break;
			}

			innerPosition = position + gml::Vec2f((float)borderSize);
		}

		if (lastPosition != position) {
			for (auto & child : children) {
                child->update_geometry();
			}
		}

        update_parameters();
	}


	gml::Vec2f Widget::toAbs(gml::Vec2f value)
	{
		gml::Vec2f vec;
		gml::Vec2f parentSize = parent->getSize();
		vec.x() = value.x() * parentSize.x();
		vec.y() = value.y() * parentSize.y();
		return vec;
	}
	float Widget::toAbsX(float value)
	{
		return value * parent->getSize().x();
	}
	float Widget::toAbsY(float value)
	{
		return value * parent->getSize().y();
	}

	float Widget::fromAspect()
	{
		if (sizeHint.x() == SizeHint::INFINITE) {
			return toAbsX(prefSize.x()) * prefSize.y();
		}
		else if (sizeHint.y() == SizeHint::INFINITE) {
			return prefSize.x() * toAbsY(prefSize.y());
		}
		else {
			return prefSize.x() * prefSize.y();
		}
	}

    TextureAtlasView Widget::texture_atlas_view()
    {
        return m_texture_atlas_view;
    }
}

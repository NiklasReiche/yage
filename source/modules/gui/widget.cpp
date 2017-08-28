#include "Widget.h"

namespace gui
{
	Widget::Widget()
	{
		this->layout = std::make_unique<Layout>();
	}
	Widget::Widget(Widget * parent, MasterInterface master, const WidgetTemplate & wTemplate)
		: parent(parent), master(master), 
		anchor(wTemplate.geometry.anchor),
		cellMargin(wTemplate.geometry.offset),
		prefSize(wTemplate.geometry.size),
		sizeHint(wTemplate.geometry.sizeHint),
		offsetHint(wTemplate.geometry.offsetHint),
		color(gl::toVec4(wTemplate.color)),
		borderSize(wTemplate.border.size),
		borderColor(gl::toVec4(wTemplate.border.color)),
		shadowOffset(wTemplate.shadow.offset),
		shadowHardness(wTemplate.shadow.hardness)
	{
		this->innerPosition = position + gml::Vec2f((float)borderSize);
		this->innerSize = size - gml::Vec2f((float)borderSize * 2);

		this->layout = std::make_unique<Layout>();

		

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices);
		master.glContext->createDrawable(*this, vertices, std::vector<int> { 2, 4 }, gl::DrawMode::DRAW_DYNAMIC, gl::VertexFormat::BATCHED);

		updateGeometry();
	}
	
	void Widget::constructCoords(std::vector<gl::Gfloat> & vertices)
	{
		vertices.resize(0);

		float left = position.x;
		float top = position.y;
		float right = position.x + size.x;
		float bottom = position.y + size.y;

		int so = shadowOffset;
		int bs = borderSize;

		if (so != 0)
		{
			std::array<gl::Gfloat, 12> pos_shadow = {
				left + so,	top + so,
				left + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	top + so,
				left + so,	top + so,
			};
			vertices.insert(std::begin(vertices), std::begin(pos_shadow), std::end(pos_shadow));
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

			std::array<gl::Gfloat, 48> pos_border = {
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

		std::array<gl::Gfloat, 12> pos_plain = {
			left,	top,
			left,	bottom,
			right,	bottom,
			right,	bottom,
			right,	top,
			left,	top,
		};
		vertices.insert(std::end(vertices), std::begin(pos_plain), std::end(pos_plain));
	}

	void Widget::constructColors(std::vector<gl::Gfloat> & vertices)
	{
		gml::Vec4<float> gl_color = color;
		gml::Vec4<float> gl_bc = borderColor;
		float sh = shadowHardness;

		if (sh > 0)
		{
			std::array<gl::Gfloat, 24> color_shadow = {
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
			};
			vertices.insert(std::begin(vertices), std::begin(color_shadow), std::end(color_shadow));
		}

		if (borderSize != 0)
		{
			std::array<gl::Gfloat, 96> color_border = {
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,

				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,

				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,

				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w
			};
			vertices.insert(std::end(vertices), std::begin(color_border), std::end(color_border));
		}

		std::array<gl::Gfloat, 24> color_plain = {
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w
		};
		vertices.insert(std::end(vertices), std::begin(color_plain), std::end(color_plain));
	}

	void Widget::constructVertices(std::vector<gl::Gfloat>& vertices)
	{
		std::vector<gl::Gfloat> coords;
		std::vector<gl::Gfloat> colors;

		float left = position.x;
		float top = position.y;
		float right = position.x + size.x;
		float bottom = position.y + size.y;

		float sh = shadowHardness;
		int so = shadowOffset;
		int bs = borderSize;

		gml::Vec4<float> gl_color = color;
		gml::Vec4<float> gl_bc = borderColor;
		

		// Drop Shadow Effect
		if (so != 0 && sh > 0)
		{
			std::array<gl::Gfloat, 12> pos_shadow = {
				left + so,	top + so,
				left + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	top + so,
				left + so,	top + so,
			};
			std::array<gl::Gfloat, 24> color_shadow = {
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
				0.0f, 0.0f, 0.0f, sh,
			};
			coords.insert(std::begin(coords), std::begin(pos_shadow), std::end(pos_shadow));
			colors.insert(std::begin(colors), std::begin(color_shadow), std::end(color_shadow));
		}

		// Border Effect
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

			std::array<gl::Gfloat, 48> pos_border = {
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
			std::array<gl::Gfloat, 96> color_border = {
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,

				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w,
				gl_bc.x, gl_bc.y, gl_bc.z, gl_bc.w
			};
			coords.insert(std::end(coords), std::begin(pos_border), std::end(pos_border));
			colors.insert(std::end(colors), std::begin(color_border), std::end(color_border));
		}

		// Main geometry
		std::array<gl::Gfloat, 12> pos_plain = {
			left,	top,
			left,	bottom,
			right,	bottom,
			right,	bottom,
			right,	top,
			left,	top,
		};
		std::array<gl::Gfloat, 24> color_plain = {
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w,
			gl_color.x, gl_color.y, gl_color.z, gl_color.w
		};
		coords.insert(std::end(coords), std::begin(pos_plain), std::end(pos_plain));
		colors.insert(std::end(colors), std::begin(color_plain), std::end(color_plain));

		vertices.insert(std::end(vertices), std::begin(coords), std::end(coords));
		vertices.insert(std::end(vertices), std::begin(colors), std::end(colors));
	}


	void Widget::updateParams()
	{
		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices);
		bufferSubData(0, vertices);
	}

	void Widget::relayout()
	{
		if (fitChildren) {
			prefSize = calcPrefSize();
			parent->relayout();
		}
		layout->update(this);
	}
	void Widget::setSize(gml::Vec2f size)
	{
		if (this->size != size) {
			this->size = size;
			this->innerSize = size - gml::Vec2f((float)borderSize) * 2;
			updateGeometry();
			layout->update(this);
		}
	}
	void Widget::resize(gml::Vec2f size)
	{
		if (this->prefSize != size) {
			prefSize = size;
			parent->relayout();
			layout->update(this);
		}
	}
	void Widget::setOffset(gml::Vec2f offset)
	{
		if (this->offset != offset) {
			this->offset = offset;
			updateGeometry();
		}
	}
	void Widget::move(gml::Vec2f cellMargin)
	{
		if (this->cellMargin != cellMargin) {
			this->cellMargin = cellMargin;
			parent->relayout();
			layout->update(this);
		}
	}
	void Widget::setAnchor(Anchor anchor) 
	{
		if (this->anchor != anchor) {
			this->anchor = anchor;
			updateGeometry();
		}
	}

	void Widget::updateGeometry()
	{
		gml::Vec2f lastPosition = position;

		if (parent != nullptr) {
			switch (anchor)
			{
			case Anchor::TOP_LEFT:
				position = parent->getInnerPosition() + (offset);
				break;

			case Anchor::BOTTOM_LEFT:
				position.x = parent->getInnerPosition().x + (offset).x;
				position.y = (parent->getInnerPosition().y + parent->getInnerSize().y) - ((offset).y + size.y);
				break;

			case Anchor::TOP_RIGHT:
				position.x = (parent->getInnerPosition().x + parent->getInnerSize().x) - ((offset).x + size.x);
				position.y = parent->getInnerPosition().y + (offset).y;
				break;

			case Anchor::BOTTOM_RIGHT:
				position.x = (parent->getInnerPosition().x + parent->getInnerSize().x) - ((offset).x + size.x);
				position.y = (parent->getInnerPosition().y + parent->getInnerSize().y) - ((offset).y + size.y);
				break;

			case Anchor::CENTER:
				position = parent->getInnerPosition() + parent->getInnerSize() / 2 + (offset);
				break;
			}

			innerPosition = position + gml::Vec2f((float)borderSize);
		}

		if (lastPosition != position) {
			for (auto & child : children) {
				child->updateGeometry();
			}
		}

		updateParams();
	}


	gml::Vec2f Widget::toAbs(gml::Vec2f value)
	{
		gml::Vec2f vec;
		gml::Vec2f parentSize = parent->getSize();
		vec.x = value.x * parentSize.x;
		vec.y = value.y * parentSize.y;
		return vec;
	}
	float Widget::toAbsX(float value)
	{
		return value * parent->getSize().x;
	}
	float Widget::toAbsY(float value)
	{
		return value * parent->getSize().y;
	}
}

#include "Widget.h"

namespace gui
{
	Widget::Widget()
	{
		this->layouter = std::make_unique<Layout>();
	}
	Widget::Widget(Widget * parent, MasterInterface master, const WidgetLayout & layout)
		: parent(parent), master(master), offset(layout.geometry.offset), anchor(layout.geometry.anchor), size(layout.geometry.size), borderSize(layout.border.size), shadowOffset(layout.shadow.offset), shadowHardness(layout.shadow.hardness)
	{
		this->color = gl::toVec4(layout.color);
		this->borderColor = gl::toVec4(layout.border.color);

		this->parentSizeHint = layout.geometry.parentSizeHint;
		this->childSizeHint = layout.geometry.childSizeHint;

		if (layout.layout == LayoutType::V_LIST_LAYOUT) {
			this->layouter = std::make_unique<VListLayout>();
		}
		else {
			this->layouter = std::make_unique<Layout>();
		}

		updateGeometry();

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices);
		master.glContext->createDrawable(*this, vertices, std::vector<int> { 2, 4 }, gl::DrawMode::DRAW_DYNAMIC, gl::VertexFormat::BATCHED);
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

	void Widget::resize(gml::Vec2<float> size) 
	{
		this->size = size;
		updateGeometry();
	}
	void Widget::move(gml::Vec2<float> position)
	{
		this->offset = position;
		updateGeometry();
	}
	void Widget::setAnchor(Anchor anchor) 
	{
		this->anchor = anchor;
		updateGeometry();
	}

	void Widget::updateGeometry()
	{
		if (anchor == Anchor::TOP_LEFT) {
			position = parent->getPosition() + offset;
		}
		else if (anchor == Anchor::BOTTOM_LEFT) {
			position.x = parent->getPosition().x + offset.x;
			position.y = (parent->getPosition().y + parent->getSize().y) - (offset.y + size.y);
		}
		else if (anchor == Anchor::TOP_RIGHT) {
			position.x = (parent->getPosition().x + parent->getSize().x) - (offset.x + size.x);
			position.y = parent->getPosition().y + offset.y;
		}
		else if (anchor == Anchor::BOTTOM_RIGHT) {
			position.x = (parent->getPosition().x + parent->getSize().x) - (offset.x + size.x);
			position.y = (parent->getPosition().y + parent->getSize().y) - (offset.y + size.y);
		}
		else if (anchor == Anchor::CENTER) {
			position = parent->getPosition() + parent->getSize() / 2 + offset;
		}

		for (auto & child : children)
		{
			child->updateGeometry();
		}

		updateParams();
	}

	Animation* Widget::createAnimation(Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
	{
		animations.push_back(Animation(this, master, beg, goal, time));
		return &animations.back();
	}
}

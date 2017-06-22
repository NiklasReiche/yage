#include "Widget.h"

namespace gui
{
	Widget::Widget(Widget * parent, MasterInterface master, W_Geometry geometry, unsigned int color, W_Border border, W_Shadow shadow)
		: parent(parent), master(master), position(geometry.position), size(geometry.size), borderSize(border.size), shadowOffset(shadow.offset), shadowHardness(shadow.hardness)
	{
		this->color = gl::toVec4(color);
		this->borderColor = gl::toVec4(border.color);
		this->position = parent->getPosition() + position;

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices);
		master.glContext->createDrawable(*this, vertices, std::vector<int> { 2, 4 }, gl::DrawMode::DRAW_DYNAMIC, gl::VertexFormat::BATCHED);
	}
	Widget::Widget(Widget * parent, MasterInterface master, const WidgetLayout & layout)
		: parent(parent), master(master), position(layout.geometry.position), size(layout.geometry.size), borderSize(layout.border.size)
	{
		this->color = gl::toVec4(layout.color);
		this->borderColor = gl::toVec4(layout.border.color);
		this->position = parent->getPosition() + position;

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices);
		master.glContext->createDrawable(*this, vertices, std::vector<int> { 2, 4 }, gl::DrawMode::DRAW_DYNAMIC, gl::VertexFormat::BATCHED);
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
		if (sh > 0)
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
}

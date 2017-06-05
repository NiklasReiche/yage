#include "widget.h"

namespace gui
{
	Widget::Widget(Widget * parent, ManagerInterface mInterface, W_Geometry geometry, unsigned int color, W_Border border, W_Shadow shadow)
		: parent(parent), masterInterface(mInterface), position(geometry.position), size(geometry.size), borderSize(border.size), shadowOffset(shadow.offset), shadowHardness(shadow.hardness)
	{
		this->color = ngl::toVec4(color);
		this->borderColor = ngl::toVec4(border.color);
		this->position = parent->getPosition() + position;

		std::vector<Gfloat> vertices;
		constructVertices(vertices);
		ngl::genDrawable(*this, vertices, std::vector<int> { 2, 4 }, ngl::DrawMode::DRAW_DYNAMIC, ngl::VertexFormat::BATCHED);
	}

	void Widget::constructVertices(std::vector<Gfloat>& vertices)
	{
		std::vector<Gfloat> coords;
		std::vector<Gfloat> colors;

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
			std::array<Gfloat, 12> pos_shadow = {
				left + so,	top + so,
				left + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	bottom + so,
				right + so,	top + so,
				left + so,	top + so,
			};
			std::array<Gfloat, 24> color_shadow = {
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

			std::array<Gfloat, 48> pos_border = {
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
			std::array<Gfloat, 96> color_border = {
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
		std::array<Gfloat, 12> pos_plain = {
			left,	top,
			left,	bottom,
			right,	bottom,
			right,	bottom,
			right,	top,
			left,	top,
		};
		std::array<Gfloat, 24> color_plain = {
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
		std::vector<Gfloat> vertices;
		constructVertices(vertices);
		ngl::configVBO(VBO, 0, vertices);
	}
}

#include "widget.h"

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

		this->texCoords = loadTexture(wTemplate.texture);

		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices, texCoords);
		master.glContext->createDrawable(*this, vertices, std::vector<int> { 2, 4, 2 }, gl::DrawMode::DRAW_DYNAMIC, gl::VertexFormat::BATCHED);

		updateGeometry();
	}
	
	void Widget::constructCoords(std::vector<gl::Gfloat> & vertices) const
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

	void Widget::constructColors(std::vector<gl::Gfloat> & vertices) const
	{
		gml::Vec4<float> gl_color = color;
		gml::Vec4<float> gl_bc = borderColor;
		float sh = shadowHardness;

		if (shadowOffset != 0)
		{
			std::array<gl::Gfloat, 24> color_shadow = {
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

	void Widget::constructTexCoords(std::vector<gl::Gfloat> & vertices, const gml::Vec4f texCoords) const
	{
		gml::Vec2f alpha = master.textureManager->getAlphaTexCoords();

		if (shadowOffset != 0)
		{
			std::array<gl::Gfloat, 12> tex_shadow = {
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
			};
			vertices.insert(std::end(vertices), std::begin(tex_shadow), std::end(tex_shadow));
		}

		if (borderSize != 0) {
			std::array<gl::Gfloat, 48> tex_border = {
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,

				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,

				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,

				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
				alpha.x,	alpha.y,
			};
			vertices.insert(std::end(vertices), std::begin(tex_border), std::end(tex_border));
		}

		std::array<gl::Gfloat, 12> tex_plain = {
			texCoords.x,	texCoords.y,
			texCoords.x,	texCoords.w,
			texCoords.z,	texCoords.w,
			texCoords.z,	texCoords.w,
			texCoords.z,	texCoords.y,
			texCoords.x,	texCoords.y,
		};
		vertices.insert(std::end(vertices), std::begin(tex_plain), std::end(tex_plain));
	}

	void Widget::constructVertices(std::vector<gl::Gfloat>& vertices, const gml::Vec4f texCoords) const
	{
		constructCoords(vertices);
		constructColors(vertices);
		constructTexCoords(vertices, texCoords);
	}

	gml::Vec4f Widget::loadTexture(WidgetTextureTemplate tTemplate) const
	{
		if (!tTemplate.filename.empty()) {
			img::ImageReader reader(master.platform);
			img::Image image = reader.readFile(tTemplate.filename, img::FORCE_CHANNELS::RGBA);
			return master.textureManager->addTexture(image);
		}
		else if (!tTemplate.image.isEmpty()) {
			return master.textureManager->addTexture(tTemplate.image);
		}
		else if (!tTemplate.texture.isEmpty()) {
			return master.textureManager->addTexture(tTemplate.texture);
		}
		else {
			return gml::Vec4f(master.textureManager->getAlphaTexCoords().x, master.textureManager->getAlphaTexCoords().y, master.textureManager->getAlphaTexCoords().x, master.textureManager->getAlphaTexCoords().y);
		}
	}

	void Widget::hide()
	{
		isActive = false;
	}
	void Widget::show()
	{
		isActive = true;
	}

	/* Getter */
	bool Widget::is_Active() const
	{
		return isActive;
	}
	bool Widget::is_Interactable() const
	{
		return isInteractable;
	}
	bool Widget::has_Text() const
	{
		return hasText;
	}

	Widget* Widget::getParent() const
	{
		return parent;
	}
	Widget& Widget::getChild(const int i) const
	{
		return *children[i];
	}
	unsigned int Widget::getChildrenCount() const
	{
		return children.size();
	}


	gml::Vec2<SizeHint> Widget::getSizeHint() const
	{
		return sizeHint;
	}
	gml::Vec2<OffsetHint> Widget::getOffsetHint() const
	{
		return offsetHint;
	}
	gml::Vec2f Widget::getMinSize() const
	{
		return minSize;
	}
	gml::Vec2f Widget::getMaxSize() const
	{
		return maxSize;
	}
	gml::Vec2f Widget::getPreferredSize() const
	{
		return prefSize;
	}
	gml::Vec2f Widget::getCellMargin() const
	{
		return cellMargin;
	}
	gml::Vec2f Widget::getLayoutMargin() const
	{
		return layoutMargin;
	}
	gml::Vec2f Widget::getOffset() const
	{
		return offset;
	}
	gml::Vec2f Widget::getPosition() const
	{
		return position;
	}
	gml::Vec2f Widget::getInnerPosition() const
	{
		return innerPosition;
	}
	gml::Vec2f Widget::getSize() const
	{
		return size;
	}
	gml::Vec2f Widget::getInnerSize() const
	{
		return innerSize;
	}
	gml::Vec4f Widget::getColor() const
	{
		return color;
	}

	void Widget::setColor(const int color)
	{
		this->color = gl::toVec4(color);
	}
	void Widget::setColor(gml::Vec4<float> color)
	{
		this->color = color;
	}
	void Widget::setSizeHint(gml::Vec2<SizeHint> sizeHint)
	{
		this->sizeHint = sizeHint;
	}
	void Widget::setPreferredSize(gml::Vec2f size)
	{
		this->prefSize = size;
	}
	void Widget::setTexCoords(gml::Vec4f texCoords)
	{
		this->texCoords = texCoords;
	}

	void Widget::setTexture(WidgetTextureTemplate texture)
	{
		this->texCoords = loadTexture(texture);
	}
	void Widget::setTexture(const std::string & filename)
	{
		if (!filename.empty()) {
			img::ImageReader reader(master.platform);
			img::Image image = reader.readFile(filename, img::FORCE_CHANNELS::RGBA);
			this->texCoords = master.textureManager->addTexture(image);
		}
		else {
			this->texCoords = gml::Vec4f(master.textureManager->getAlphaTexCoords().x, master.textureManager->getAlphaTexCoords().y, master.textureManager->getAlphaTexCoords().x, master.textureManager->getAlphaTexCoords().y);
		}
		updateParams();
	}
	void Widget::setTexture(img::Image image)
	{
		this->texCoords = master.textureManager->addTexture(image);
		updateParams();
	}
	void Widget::setTexture(gl::Texture texture)
	{
		this->texCoords = master.textureManager->addTexture(texture);
		updateParams();
	}
	void Widget::removeTexture() {
		this->texCoords = gml::Vec4f(master.textureManager->getAlphaTexCoords().x, master.textureManager->getAlphaTexCoords().y, master.textureManager->getAlphaTexCoords().x, master.textureManager->getAlphaTexCoords().y);
		updateParams();
	}

	void Widget::updateParams()
	{
		std::vector<gl::Gfloat> vertices;
		constructVertices(vertices, texCoords);
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
			this->innerSize = size - gml::Vec2f((float)borderSize) * 2.0f;
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
				position = parent->getInnerPosition() + parent->getInnerSize() / 2.0f + (offset);
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


	gml::Vec2f Widget::toAbs(gml::Vec2f value) const
	{
		gml::Vec2f vec;
		gml::Vec2f parentSize = parent->getSize();
		vec.x = value.x * parentSize.x;
		vec.y = value.y * parentSize.y;
		return vec;
	}
	float Widget::toAbsX(const float value) const
	{
		return value * parent->getSize().x;
	}
	float Widget::toAbsY(const float value) const
	{
		return value * parent->getSize().y;
	}

	float Widget::fromAspect() const
	{
		if (sizeHint.x == SizeHint::INFINITE) {
			return toAbsX(prefSize.x) * prefSize.y;
		}
		else if (sizeHint.y == SizeHint::INFINITE) {
			return prefSize.x * toAbsY(prefSize.y);
		}
		else {
			return prefSize.x * prefSize.y;
		}
	}
}

#include "root.h"

#include <utility>

namespace gui
{
	RootWidget::RootWidget(Master* master) : Widget(nullptr, master, {
        .color = gl::Color::TRANSPARENT
    })
	{
        m_layout = std::make_unique<AbsoluteLayout>();
	}

	void RootWidget::update_layout()
	{
		m_layout->update(this);
	}
}
#include "root.h"

#include <utility>

namespace yage::gui
{
	RootWidget::RootWidget(Master* master) : Widget(nullptr, master, {
        .color = gl::Color::TRANSPARENT
    })
	{

	}
}
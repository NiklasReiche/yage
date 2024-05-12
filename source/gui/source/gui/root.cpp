#include "root.h"

#include <utility>

namespace gui
{
	RootWidget::RootWidget(Master* master) : Widget(nullptr, master, {
        .color = gl::Color::TRANSPARENT
    })
	{

	}
}
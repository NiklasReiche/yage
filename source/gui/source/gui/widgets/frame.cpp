#include "frame.h"

namespace gui
{
	Frame::Frame(Widget * parent, Master* master, FrameTemplate frameTemplate)
		: Widget(parent, master, frameTemplate)
	{
        m_is_interactable = false;

        m_padding = frameTemplate.layoutMargin;
		
		if (m_hint_pref_size == gml::Vec2f(0.0f)) {
            m_fit_children = true;
		}

		switch (frameTemplate.layoutType) 
		{
		case LayoutType::ABSOLUTE_LAYOUT:
            m_layout = std::make_unique<AbsoluteLayout>();
			break;
		case LayoutType::V_LIST_LAYOUT:
            m_layout = std::make_unique<VListLayout>();
			break;
		case LayoutType::H_LIST_LAYOUT:
            m_layout = std::make_unique<HListLayout>();
			break;
		}
	}

	gml::Vec2f Frame::calcPrefSize()
	{
		gml::Vec2f mSize;
		mSize.x() = m_layout->calcPrefSizeX(this) + gml::Vec2f((float)m_border_size).x();
		mSize.y() = m_layout->calcPrefSizeY(this) + gml::Vec2f((float)m_border_size).y();
		return mSize;
	}
}
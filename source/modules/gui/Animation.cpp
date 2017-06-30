#include "Animation.h"

namespace gui
{
	Animation::Animation(Widget* widget, gml::Vec2<float> goal, double time)
		: goal(goal), timeEnd(time), timeCurrent(0.0), widget(widget) {}

	void Animation::start()
	{
		beg = widget->getPosition();
	}
	void Animation::update(double dt)
	{
		if (timeCurrent < timeEnd) {
			timeCurrent += dt;
			float time = timeCurrent / timeEnd;
			widget->move(gml::lerp<gml::Vec2<float>>(beg, goal, time));
		}
	}
	void Animation::stop()
	{

	}

	bool Animation::is_finished()
	{
		return isFinished;
	}
}
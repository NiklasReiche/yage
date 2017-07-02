#include "Animation.h"
#include "Widget.h"
#include "Master.h"

namespace gui
{
	Animation::Animation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
		: widget(widget), master(master), beg(beg), goal(goal), timeEnd(time) {}

	void Animation::start()
	{
		master->activateAnimation(this);
	}
	void Animation::update(double dt)
	{
		if (timeCurrent < timeEnd) {
			timeCurrent += dt;
			float time = timeCurrent / timeEnd;
			widget->move(gml::lerp<gml::Vec2<float>>(beg, goal, time));
		}
		else {
			isFinished = true;
		}
	}
	void Animation::stop()
	{
		master->deactivateAnimation(this);
	}
	void Animation::reset()
	{
		timeCurrent = 0.0;
	}

	bool Animation::is_finished()
	{
		return isFinished;
	}
}
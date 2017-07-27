#include "Animation.h"
#include "Widget.h"
#include "Master.h"

namespace gui
{
	Animation::Animation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
		: widget(widget), master(master), beg(beg), goal(goal), timeEnd(time) {}

	void Animation::start()
	{
		if (isFinished) {
			master->activateAnimation(this);
			isFinished = false;
		}
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
			reset();
		}
	}
	void Animation::stop()
	{
		if (!isFinished) {
			master->deactivateAnimation(this);
			isFinished = true;
			reset();
		}
	}
	void Animation::reset()
	{
		timeCurrent = 0.0;
	}

	bool Animation::is_finished()
	{
		return isFinished;
	}


	void MoveAnimation::update(Widget* widget)
	{
		if (widget->getSize().x < gml::Vec2f(100).x && grow == true) {
			widget->resize(widget->getSize() + gml::Vec2f(1));
		}
		if (widget->getSize().x >= gml::Vec2f(100).x) {
			grow = false;
		}
		if (widget->getSize().x > gml::Vec2f(50).x && grow == false) {
			widget->resize(widget->getSize() - gml::Vec2f(1));
		}
		if (widget->getSize().x <= gml::Vec2f(50).x) {
			grow = true;
		}
	}
}
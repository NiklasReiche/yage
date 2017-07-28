#include "Animation.h"
#include "Widget.h"
#include "Master.h"

namespace gui
{
	Animation::Animation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
		: widget(widget), master(master), beg(beg), goal(goal), timeEnd(time) {}

	void Animation::setOnAnimationStop(std::function<void()> callback)
	{
		onAnimationStop = callback;
	}

	void Animation::start()
	{
		if (isFinished) {
			master->activateAnimation(this);
			isFinished = false;
		}
	}
	void Animation::stop()
	{
		if (!isFinished) {
			master->deactivateAnimation(this);
			isFinished = true;
			reset();
			onAnimationStop();
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


	MoveAnimation::MoveAnimation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
		: Animation(widget, master, beg, goal, time) {}

	void MoveAnimation::update(double dt)
	{
		if (!isFinished) {
			if (timeCurrent < timeEnd) {
				timeCurrent += dt;
				float time = (float)(timeCurrent / timeEnd);
				widget->move(gml::lerp<gml::Vec2<float>>(beg, goal, time));
			}
			else {
				stop();
			}
		}
	}

	SizeAnimation::SizeAnimation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
		: Animation(widget, master, beg, goal, time) {}

	void SizeAnimation::update(double dt)
	{
		if (!isFinished) {
			if (timeCurrent < timeEnd) {
				timeCurrent += dt;
				float time = (float)(timeCurrent / timeEnd);
				widget->resize(gml::lerp<gml::Vec2<float>>(beg, goal, time));
			}
			else {
				stop();
			}
		}
	}
}
#include "animation.h"
#include "widgets/widget.h"
#include "master.h"

namespace gui
{
	Animation::Animation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time)
		: master(master), widget(widget), timeEnd(time), beg(beg), goal(goal) {}

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
			if (onAnimationStop) {
				onAnimationStop();
			}
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
				widget->set_anchor({.offset = gml::lerp<gml::Vec2<float>>(beg, goal, time)}); // TODO
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
				widget->resize({
                    .value = gml::lerp<gml::Vec2f>(beg, goal, time)}); // TODO
			}
			else {
				stop();
			}
		}
	}
}
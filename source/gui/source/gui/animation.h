#pragma once

#include <functional>

#include <gml/vector.h>
#include <gml/interpolation.h>

namespace gui
{
	class Widget;
	class Master;

	class Animation
	{
	protected:
		Master* master;
		Widget* widget;

		std::function<void()> onAnimationStop;

		double timeCurrent = 0.0;
		double timeEnd;
		gml::Vec2<float> beg;
		gml::Vec2<float> goal;

		bool isFinished = true;

	public:
		Animation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time); // TODO make this friend
        virtual ~Animation() = default;

		void start();
		void stop();
		void reset();

		virtual void update(double dt) = 0;

		void setOnAnimationStop(std::function<void()> callback);
		bool is_finished();
	};

	class MoveAnimation : public Animation
	{
	public:
		MoveAnimation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time);

		void update(double dt) override;
	};

	class SizeAnimation : public Animation
	{
	public:
		SizeAnimation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time);

		void update(double dt) override;
	};
}
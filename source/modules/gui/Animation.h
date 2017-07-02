#pragma once

#include "core.h"
#include <math/Interpolation.h>

namespace gui
{
	class Widget;
	class Master;

	class Animation
	{
	private:
		Master* master;
		Widget* widget;

		double timeCurrent = 0.0;
		double timeEnd;
		gml::Vec2<float> beg;
		gml::Vec2<float> goal;

		bool isFinished = false;

	public:
		Animation(Widget* widget, Master* master, gml::Vec2<float> beg, gml::Vec2<float> goal, double time);

		void start();
		void stop();
		void update(double dt);
		void reset();

		bool is_finished();
	};
}
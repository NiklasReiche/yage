#pragma once

#include "core.h"
#include <math/Interpolation.h>
#include "Widget.h"

namespace gui
{
	class Animation
	{
	private:
		Widget* widget;

		double timeCurrent;
		double timeEnd;
		gml::Vec2<float> beg;
		gml::Vec2<float> goal;

		bool isFinished = false;

	public:
		Animation(Widget* widget, gml::Vec2<float> goal, double time);

		void start();
		void update(double dt);
		void stop();

		bool is_finished();
	};
}
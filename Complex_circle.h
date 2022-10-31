#ifndef COMPLEX_CIRCLE_H
#define COMPLEX_CIRCLE_H

#include "GUI/Graph.h"

namespace Graph_lib
{
    // Alternative implementation of Circle; enables application of 
    // transform matrix
    struct Complex_circle : Shape
	{
		Complex_circle(Point p, int rr) // center and radius
			: r{rr}
		{
			add(Point{p.x - r, p.y - r});
		}

		void draw_lines() const;

		Point center() const { return {point(0).x + r, point(0).y + r}; }

		void set_radius(int rr) { r = rr; }
		int radius() const { return r; }

	private:
		int r;
	};
}

#endif
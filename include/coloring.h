#include <iostream>
#include <tuple>
#include <cmath>
#include "global.h"

#ifndef COLORING // generates RGB tuple from iteration value
#define COLORING

struct Colors{
	int red;
	int green;
	int blue;
};

Colors color_scheme(int iter) {
	Colors colors;

	if (iter < MAX_ITER) {
		colors.red   = (cos(cbrt(iter)  / 2) + 1) * (127.5);
		colors.green = (sin(cbrt(iter) - 2) + 1) * (127.5);
		colors.blue  = (sin(cbrt(iter) * 2 + 2) + 1) * (127.5);
	} else {
		colors.red = 0;
		colors.green = 0;
		colors.blue = 0;
	}
	return colors;
};
#endif
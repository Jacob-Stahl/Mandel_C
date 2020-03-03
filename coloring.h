#include <iostream>
#include <tuple>
#include <cmath>
#include "global.h"

#ifndef COLORING // generates RGB tuple from iteration value
#define COLORING

std::tuple <unsigned char, unsigned char, unsigned char> color_scheme(int iter)
{
	int red, blue, green;
	if (iter < MAX_ITER)
	{
		red   = (sin(cbrt(iter)   /4 ) + 1) * (127.5);
		green = (sin(cbrt(iter) - 2) + 1) * (127.5);
		blue  = (sin(cbrt(iter) * 2 + 2) + 1) * (127.5);
	}
	else
	{
		red = 0;
		green = 0;
		blue = 0;
	}
	return { red, green, blue };
};
#endif
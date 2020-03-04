#ifndef FRAC_FUNC
#define FRAC_FUNC

#include <complex> 
#include <stdio.h>
#include "global.h"
#include "coloring.h"

/* fractal functions:
	brot_iter : z = z^2 + z0

	funky variants terminate when z gets too close to zero */
Colors funky_brot_iter(double r0, double i0)
{
	double r = r0;
	double i = i0;
	double r2 = r * r;
	double i2 = i * i;
	double z_mag_2 = r2 + i2;
	int iter = 0;
	float coef = (r + i) / MAX_ITER;

	while ((iter < MAX_ITER) && (z_mag_2 < 4) && (z_mag_2 > pow(iter * coef, 2)))
	{
		i = r * i;
		r = r2 - i2;
		i += i;

		r += r0;
		i += i0;

		r2 = r * r;
		i2 = i * i;

		z_mag_2 = r2 + i2;
		coef = (r + i) / MAX_ITER;
		iter += 1;
	}
	return color_scheme(iter);
}
/* standard mandelbrot set*/
Colors brot_iter(double r0, double i0)
{
	double r = r0;
	double i = i0;
	double r2 = r * r;
	double i2 = i * i;
	double z_mag_2 = r2 + i2;
	int iter = 0;

	while ((iter < MAX_ITER) && (z_mag_2) < 4)
	{
		i = r * i;
		r = r2 - i2;  
		i += i;

		r += r0;
		i += i0;

		r2 = r * r;
		i2 = i * i;

		z_mag_2 = r2 + i2;
		iter += 1;
	};
	return color_scheme(iter);
}
Colors collatz(double r0, double i0)
{
	std::complex<double> z(r0, i0);
	std::complex<double> a;
	std::complex<double> b;
	std::complex<double> c1(1,0);
	std::complex<double> c2(2,0);
	std::complex<double> c3(3,0);
	register int iter = 0;
	register double z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	while ((iter < MAX_ITER) && (z_mag_2 < 64))
	{
		a = M_PI / 2 * z;
		z = (z*pow(cos(a), 2)) / c2 + ((c3 * z + c1) / c2) * pow(sin(a), 2);
		iter += 1;
		z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	}
	return color_scheme(iter);
};
#endif
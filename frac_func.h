#ifndef FRAC_FUNC
#define FRAC_FUNC

#include <complex> 
#include <stdio.h>
#include "global.h"
#include "matrix.h"

/* fractal functions:
	brot_iter : z = z^2 + z0

	funky variants terminate when z gets too close to zero */
template<typename T>
int funky_brot_iter(T r0, T i0)
{
	T r = r0;
	T i = i0;
	T r2 = r * r;
	T i2 = i * i;
	T z_mag_2 = r2 + i2;
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
	return iter;
}
/* standard mandelbrot set*/
template<typename T>
int brot_iter(T r0, T i0)
{
	T r = r0;
	T i = i0;
	T r2 = r * r;
	T i2 = i * i;
	T z_mag_2 = r2 + i2;
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
	return iter;
}
template<typename T>
int collatz(T r0, T i0)
{
	std::complex<T> z(r0, i0);
	std::complex<T> a;
	std::complex<T> b;
	std::complex<T> c1(1,0);
	std::complex<T> c2(2,0);
	std::complex<T> c3(3,0);
	register int iter = 0;
	register T z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	while ((iter < MAX_ITER) && (z_mag_2 < 64))
	{
		a = M_PI / 2 * z;
		z = (z*pow(cos(a), 2)) / c2 + ((c3 * z + c1) / c2) * pow(sin(a), 2);
		iter += 1;
		z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	}
	return iter;
};
#endif
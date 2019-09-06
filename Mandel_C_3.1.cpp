#include "pch.h"
#include "SDL.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <complex> 
#include <stdio.h>
#include <stdlib.h> 
#include <cstdlib>
#include <conio.h>
#include <tuple>
#include <cmath>
#include <string>
#undef main

using namespace std;

const int N = 128; // num threads
int max_iter = 100; // iteration cap, modified by current zoom level
const int ref_zoom = 2; // distance on the complex plane at zoom x1
int dim_x = 700; // x and y dims of screen
int dim_y = 700;

std::complex<double> c(-0.7269, 0.1689); // julia set constant / seed

std::tuple <unsigned char, unsigned char, unsigned char> color_scheme(int iter) // generates RGB tuple from iteration value
{
	int red, blue, green;
	if (iter < max_iter)
	{
		red = cos(log2(iter)) * 255;
		green = sin(log2(iter)) * 255;
		blue = -cos(log2(iter)) * 255;
	}
	else
	{
		red = 0;
		green = 0;
		blue = 0;
	}
	return { red, green ,blue };
} // 

/* fractal functions:
	brot_iter : z = z^2 + z0
	julia_iter: z = z^2 + constant

	funky variants terminate when z gets too close to zero */

std::complex<double> c_pow(std::complex<double> z)
{
	double r = real(z), i = imag(z);
	std::complex<double> out(pow(r, 2) - pow(i, 2), 2 * r*i);

	return out;
}
int funky_julia_iter(double r, double i)
{
	std::complex<double>  z(r, i);
	int iter = 0;
	double z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	float coef = (abs(real(z)) + abs(imag(z))) / max_iter;

	while ((iter < max_iter) && (z_mag_2 < 4) && (z_mag_2 > pow(iter * coef, 2))) // iter * coef = inner event horizon radius
	{
		z = z * z + c;
		iter += 1;
		z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	}
	return iter;
}
int funky_brot_iter(double r, double i)
{
	std::complex<double> z0(r, i);
	std::complex<double>  z(r, i);
	int iter = 0;
	double z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	float coef = (abs(real(z)) + abs(imag(z))) / max_iter;

	while ((iter < max_iter) && (z_mag_2 < 4) && (z_mag_2 > pow(iter * coef, 2))) // iter * coef = inner event horizon radius
	{
		z = z * z + z0;
		iter += 1;
		z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	}
	return iter;
}
int brot_iter(double r, double i)
{
	std::complex<double> z0(r, i);
	std::complex<double>  z(r, i);
	int iter = 0;
	double z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);

	while ((iter < max_iter) && (z_mag_2) < 4) // iter / coef = inner event horizon radius
	{
		z = c_pow(z) + z0;
		iter += 1;
		z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	};

	return iter;
}
int julia_iter(double r, double i)
{
	std::complex<double> z(r, i);
	register int iter = 0;
	register double z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	while ((iter < max_iter) && (z_mag_2 < 4))
	{
		z = z * z + c;
		iter += 1;
		z_mag_2 = pow(real(z), 2) + pow(imag(z), 2);
	}
	return iter;
}

void threadBlock(double *realBlock, double *imagBlock, int *iterBlock, int size, function<int(double, double)> func) // thread level; applies fractal function to each element in block
{
	for (int i = 0; i < size; i++)
	{
		iterBlock[i] = func(realBlock[i], imagBlock[i]);
	};
};
void threadLauncher(double *arrR, double *arrI, int *iter, int size, function<int(double, double)> func) // splits work into threads
{
	thread threads[N];
	int blockSize = size / N;

	cout << "job size : " << size << endl;
	cout << "block size : " << blockSize << endl;
	for (int i = 0; i < N; i++)
	{
		threads[i] = thread(threadBlock, (&arrR[blockSize*i]), (&arrI[blockSize*i]), (&iter[blockSize*i]), blockSize, func);
	};
	for (int i = 0; i < N; i++) // waits for all threads to finish
	{
		threads[i].join();
	};
	cout << "done " << endl;
};
void render(double pos_x, double pos_y, float zoom, int dim_x, int dim_y, SDL_Renderer *renderer)
{
	const int size = dim_x * dim_y;

	double dim_ave = (dim_x + dim_y) / 2;

	double *real = new double[size];
	double *imag = new double[size];
	int *iter = new int[size];

	double xmin = ((-ref_zoom / zoom) + pos_x);
	double xmax = ((ref_zoom / zoom) + pos_x);
	double ymin = ((-ref_zoom / zoom) - pos_y);
	double ymax = ((ref_zoom / zoom) - pos_y);

	double dx, dy, zr, zi;

	std::tuple <unsigned char, unsigned char, unsigned char> shade;

	int idx = 0;

	cout.precision(3);

	max_iter = 100 * (log10(zoom) + 1);

	zr = xmin;
	zi = ymin;

	dx = (xmax - xmin) / dim_ave;
	dy = (ymax - ymin) / dim_ave;

	cout << "dx : " << dx << endl;
	cout << "dy : " << dy << endl;
	cout << "zr : " << zr << endl;
	cout << "zi : " << zr << endl;
	cout << "Iteration Cap : " << max_iter << endl;

	for (int i = 0; i < dim_y; i++)	// generates real and imaginary inputs to be split up by threads
	{
		for (int j = 0; j < dim_x; j++)
		{
			real[idx] = zr + j * dx;
			imag[idx] = zi + i * dy;
			idx++;
			//cout << i << "," << j << " ";
		};
		//cout << endl;
	};
	idx = 0;
	cout << endl;
	cout << "launching " << N << " threads" << endl;

	threadLauncher(real, imag, iter, size, brot_iter);

	for (int i = 0; i < dim_y; i++)
	{
		for (int j = 0; j < dim_x; j++)
		{
			shade = color_scheme(iter[idx]);
			SDL_SetRenderDrawColor(renderer,
				std::get<0>(shade),
				std::get<1>(shade),
				std::get<2>(shade),
				1);
			//cout << iter[idx] << " ";
			SDL_RenderDrawPoint(renderer, j, i);
			idx++;
		};
		SDL_RenderPresent(renderer);
		//cout << endl;
	};
};
int main()
{
	int x_mpos, y_mpos, x1_mpos, y1_mpos; // mouse positions
	double fdim_x = dim_x;
	double fdim_y = dim_y;
	double ave_dim = (dim_x + dim_y) / 2;
	double pos_x = 0; // center of screen in complex plane
	double pos_y = 0;
	float zoom = 1;
	bool running = true;
	cout.precision(64);

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Mandel_C_3.0",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		dim_x, dim_y, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	render(pos_x, pos_y, zoom, dim_x, dim_y, renderer);

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT) // finds location to zoom in on
				{
					SDL_GetMouseState(&x_mpos, &y_mpos);
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT) // finds change in y_mpos for zoom value
				{
					SDL_GetMouseState(&x1_mpos, &y1_mpos);

					pos_x = pos_x + (
						(
						(
							x_mpos - (dim_x) / 2
							) / (ave_dim / 2)
							) / (zoom / ref_zoom)
						);

					pos_y = pos_y - (
						(
						(
							y_mpos - (dim_y) / 2
							) / (ave_dim / 2)
							) / (zoom / ref_zoom)
						);

					zoom = (zoom + 1) * pow(2, ((y_mpos - y1_mpos) / 75)) - 1;
					cout << "real: " << pos_x << "  imaginary: " << pos_y << "  zoom: " << zoom << endl;
					render(pos_x, pos_y, zoom, dim_x, dim_y, renderer);
				}
			}
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
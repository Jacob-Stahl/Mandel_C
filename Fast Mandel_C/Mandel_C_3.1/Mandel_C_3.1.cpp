#include "pch.h"
#include "SDL.h"
#include <thread>
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <cmath>
#include <time.h>
#undef main

using namespace std;

const int N = 128; // num threads
const int max_iter_base = 1000; // iteration cap, modified by current zoom level
int max_iter = max_iter_base;
const double ref_zoom = 2; // distance on the complex plane at zoom x1
int dim_x = 1100; // x and y dims of screen
int dim_y = 1100;
double pre_trig;

struct Colors
{
	char red;
	char green;
	char blue;
};
Colors fast_brot_iter(double r0, double i0)
{
	double r = r0;
	double i = i0;
	double r1 = r * r;
	double i1 = i * i;
	int iter = 0;
	Colors shade;

	while (iter < max_iter && (r1 + i1 < 4))
	{
		i = r * i; // mandelbrot set function start
		r = r1 - i1;
		i += i;

		r += r0;
		i += i0;

		r1 = r * r;
		i1 = i * i; // mandelbrot set function end

		iter++;
	}

	if (iter == max_iter)
	{
		shade.blue = 0;
		shade.red = 0;
		shade.green = 0;
		return shade;
	}

	pre_trig = log(iter);
	shade.blue  = (sin(pre_trig - M_PI) + 1) * 127;
	shade.red = (cos(pre_trig) + 1) * 127;
	shade.green = (sin(sqrt(iter)) + 1) * 127;

	return shade;
};

void threadBlock(double *realBlock, double *imagBlock, Colors *shadesBlock, int size, function<Colors(double, double)> func) // thread level; applies fractal function to each element in block
{
	for (int i = 0; i < size; i++)
	{
		shadesBlock[i] = func(realBlock[i], imagBlock[i]);
	};
};
void threadLauncher(double *arrR, double *arrI, Colors *shades, int size, function<Colors(double, double)> func) // splits work into threads
{
	thread threads[N];
	int blockSize = size / N;

	cout << "job size : " << size << endl;
	cout << "block size : " << blockSize << endl;
	for (int i = 0; i < N; i++)
	{
		threads[i] = thread(threadBlock, (&arrR[blockSize*i]), (&arrI[blockSize*i]), (&shades[blockSize*i]), blockSize, func);
	};
	for (int i = 0; i < N; i++) // waits for all threads to finish
	{
		threads[i].join();
	};
	cout << "done " << endl;
};
void render(double pos_x, double pos_y, double zoom, int dim_x, int dim_y, SDL_Renderer *renderer)
{
	const int size = dim_x * dim_y;

	double *real = new double[size];
	double *imag = new double[size];

	double dim_ave = (dim_x + dim_y) / 2;
	double xmin = ((-ref_zoom / zoom) + pos_x);
	double xmax = ((ref_zoom / zoom) + pos_x);
	double ymin = ((-ref_zoom / zoom) - pos_y);
	double ymax = ((ref_zoom / zoom) - pos_y);
	double dx, dy, zr, zi;

	Colors *shades = new Colors[size];

	int idx = 0;

	clock_t start, end;
	double cpu_time_used;

	cout.precision(3);

	max_iter = max_iter_base * log10(zoom + 1);

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

	start = clock();
	threadLauncher(real, imag, shades, size, fast_brot_iter);
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	cout << "threads took : " << cpu_time_used << " seconds" << endl;

	for (int i = 0; i < dim_y; i++)
	{
		for (int j = 0; j < dim_x; j++)
		{
			SDL_SetRenderDrawColor(renderer,
				shades[idx].red,
				shades[idx].green,
				shades[idx].blue,
				1);
			//cout << iter[idx] << " ";

			SDL_RenderDrawPoint(renderer, j, i);
			idx++;
		};
		//cout << endl;
	};
	SDL_RenderPresent(renderer);
};
int main()
{
	int x_mpos, y_mpos, x1_mpos, y1_mpos; // mouse positions
	double fdim_x = dim_x;
	double fdim_y = dim_y;
	double ave_dim = (dim_x + dim_y) / 2;
	double pos_x = 0; // center of screen in complex plane
	double pos_y = 0;
	double zoom = 1;
	int running = 1;
	cout.precision(64);

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Mandel_C_3.1",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		dim_x, dim_y, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	render(pos_x, pos_y, zoom, dim_x, dim_y, renderer);

	while (running == 1)
	{
		while (SDL_PollEvent(&event))
		{
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

					pos_x = pos_x +
						(
							(
								(
									x_mpos - (dim_x) / 2
								) / (ave_dim / 2)
							) / (zoom / ref_zoom)
						);

					pos_y = pos_y -
						(
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
		};
	};
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};
#include <iostream>
#include <fstream>
#include <string>
#include "frac_func.h"
#include "global.h"
#include "thread_launcher.h"
#include "time.h"
#include "SDL.h"
using namespace std;

void render(double pos_x, double pos_y, double zoom, int dim_x, int dim_y, SDL_Renderer *renderer, bool);
void render(double pos_x, double pos_y, double zoom, int dim_x, int dim_y, SDL_Renderer *renderer, bool to_file = 0){
	const int size = dim_x * dim_y;

	double *real = new double[size];
	double *imag = new double[size];

	double dim_ave = (dim_x + dim_y) / 2;
	double xmin = ((-ref_zoom / zoom) + pos_x);
	double xmax = ((ref_zoom / zoom) + pos_x);
	double ymin = ((-ref_zoom / zoom) - pos_y);
	double ymax = ((ref_zoom / zoom) - pos_y);
	double dx, dy, zr, zi;
	int num_pixels = dim_x*dim_y;

	Colors *shades = new Colors[size];

	int idx = 0;

	clock_t start, end;
	double cpu_time_used;

	string f_name = "X " + std::to_string(pos_x) + " Y " + std::to_string(pos_y) + " zoom " + std::to_string(zoom) + ".bmp";

	string img_string = "";
	ofstream img;

	cout.precision(3);

	zr = xmin;
	zi = ymin;

	dx = (xmax - xmin) / dim_ave;
	dy = (ymax - ymin) / dim_ave;

	cout << "dx : " << dx << endl;
	cout << "dy : " << dy << endl;
	cout << "zr : " << zr << endl;
	cout << "zi : " << zr << endl;
	cout << "Iteration Cap : " << MAX_ITER << endl;

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
	cout << "launching " << NUM_THREADS << " threads" << endl;

	start = clock();
	threadLauncher(real, imag, shades, size);
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	cout << "threads took : " << cpu_time_used / 10 << " seconds" << endl;

	if (to_file == 0){
		for (int i = 0; i < dim_y; i++)
		{
			for (int j = 0; j < dim_x; j++)
			{
				SDL_SetRenderDrawColor(renderer,
					shades[idx].red,
					shades[idx].green,
					shades[idx].blue,
					1);

				SDL_RenderDrawPoint(renderer, j, i);
				idx++;
			};
			SDL_RenderPresent(renderer);
		}
	} else {
		
		img = ofstream("saves//" + f_name);
		cout << "saving image..." << endl;
		img <<"P3" << endl;
		img << dim_x << " " << dim_y << endl;
		img << "255" << endl;

		for (int i = 0; i < dim_y; i++)
		{
			for (int j = 0; j < dim_x; j++)
			{
				int red = shades[idx].red;
				int green = shades[idx].green;
				int blue = shades[idx].blue;
				
				img_string.append(to_string(red) + " " + to_string(green) + " " + to_string(blue) + "\n");
				idx++;
			};
		}
		img << img_string;
		cout << "done!" << endl;
	}
};

int main() {
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
			if(event.type == SDL_MOUSEWHEEL)
    		{
				if(event.wheel.y > 0) // scroll up
				{
					if (MAX_ITER > 10)
					{
						MAX_ITER = MAX_ITER * 1.1;
					} else {
						MAX_ITER += 1;
					}
					cout << "iteration cap : " << MAX_ITER << endl;
				}
				else if(event.wheel.y < 0) // scroll down
				{
					MAX_ITER = MAX_ITER / 1.1;
					cout << "iteration cap : " << MAX_ITER << endl;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT) // finds location to zoom in on
				{
					SDL_GetMouseState(&x_mpos, &y_mpos);
				}
				if (event.button.button == SDL_BUTTON_RIGHT) // take screenshot
				{
					//SDL_Surface *sshot = SDL_CreateRGBSurface(0, dim_x, dim_y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
					render(pos_x, pos_y, zoom, 4096, 2160, renderer, 1);
					/*
					SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
					SDL_SaveBMP(sshot, "screenshot.bmp");
					SDL_FreeSurface(sshot);
					*/
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT) // finds change in y_mpos for zoom value
				{
					SDL_GetMouseState(&x1_mpos, &y1_mpos);

					pos_x = pos_x +(((x_mpos - (dim_x) / 2) / (ave_dim / 2)) / (zoom / ref_zoom));
					pos_y = pos_y -(((y_mpos - (dim_y) / 2) / (ave_dim / 2)) / (zoom / ref_zoom));

					zoom = (zoom + 1) * pow(2, ((y_mpos - y1_mpos) / 75)) - 1;
					if (zoom < 1) { zoom = 1;}
					cout << "real: " << pos_x << "  imaginary: " << pos_y << "  zoom: " << zoom << endl;
					render(pos_x, pos_y, zoom, dim_x, dim_y, renderer, 0);
				}
			}
		};
	};
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
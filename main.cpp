#include <iostream>
#include "frac_func.h"
#include "global.h"
#include "thread_launcher.h"
#include "time.h"
using namespace std;

void render(double pos_x, double pos_y, double zoom, int dim_x, int dim_y)
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

	MAX_ITER = 1000;

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

	cout << "threads took : " << cpu_time_used << " seconds" << endl;
};
int main() {
    render(2, 2, 1, 4000, 2000);
    return 0;
}
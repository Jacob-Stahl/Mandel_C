#ifndef THREAD_LAUNCHER
#define THREAD_LAUNCHER
#include <thread>
#include <iostream>
#include "coloring.h"
#include "frac_func.h"
#include "global.h"
#include <functional>

void threadBlock(double *realBlock, double *imagBlock, Colors *shadesBlock, int size) // thread level; applies fractal function to each element in block
{
	for (int i = 0; i < size; i++)
	{
		shadesBlock[i] = brot_iter(realBlock[i], imagBlock[i]);
	};
};
void threadLauncher(double *arrR, double *arrI, Colors *shades, int size) // splits work into threads
{
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;

	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads[i] = std::thread(threadBlock, (&arrR[blockSize*i]), (&arrI[blockSize*i]), (&shades[blockSize*i]), blockSize);
	};
	for (int i = 0; i < NUM_THREADS; i++) // waits for all threads to finish
	{
		threads[i].join();
	};
}
    #endif

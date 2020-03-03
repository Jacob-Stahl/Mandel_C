#include <iostream>
#include <random>
#include <thread>
#include <functional>
#include <math.h>
#include "global.h"
#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
public:
	unsigned int width, height;
	float *arr;

	Matrix(int w = 1, int h = 1)
	{
		width = w;
		height = h;
		arr = new float[width * height + 1];
	};
	void pprint()
	{
		int x;
		for (int i = 0; i < height; i++)
		{
			x = i * width;
			for (int j = 0; j < (width); j++)
			{
				std::cout << arr[x + j] << " ";
			};
			std::cout << std::endl;
		};

		std::cout << std::endl;
	}
	void zeros()
	{
		int x;
		for (int i = 0; i < width; i++)
		{
			x = i * height;
			for (int j = 0; j < (height); j++)
			{
				arr[x + j] = 0;
			};
		};
	};
	void ones()
	{
		int x;
		for (int i = 0; i < width; i++)
		{
			x = i * height;
			for (int j = 0; j < height; j++)
			{
				arr[x + j] = 1;
			};
		};
	};
	void arange()
	{
		int x;
		float count = 0;
		for (int i = 0; i < width; i++)
		{
			x = i * height;
			for (int j = 0; j < (height); j++)
			{
				arr[x + j] = count;
				count++;
			};
		};
	};
	void random(float low = 0, float high = 1)
	{
		int x;
		std::uniform_real_distribution<float> dist(low, high);
		std::default_random_engine generator;

		for (int i = 0; i < width; i++)
		{
			x = i * height;
			for (int j = 0; j < height; j++)
			{
				arr[x + j] = dist(generator);
			};
		};
	}
	void t()
	{
		float *arr2, *temparr;
		arr2 = new float[width * height + 1];
		temparr = new float[width * height + 1];
		int x, temp;

		for (int i = 0; i < width; i++)
		{
			x = i * height;
			for (int j = 0; j < (height); j++)
			{
				arr2[width * j + i] = arr[x + j];
			};
		};

		temparr = arr2;
		arr2 = arr;
		arr = temparr;

		free(arr2);

		temp = width;
		width = height;
		height = temp;
	};

	friend Matrix operator + (Matrix const &A, Matrix const &B);
	friend Matrix operator - (Matrix const &A, Matrix const &B);
	friend Matrix operator * (Matrix const &A, Matrix const &B);
	friend Matrix operator / (Matrix const &A, Matrix const &B);
};

// takes one array; output arr = func( input arr)
void threadBlock(float *blockA, float *blockB, int size, std::function<float(float)> func) // thread level; applies function to each element in block
{
	for (int i = 0; i < size; i++)
	{
		blockB[i] = func(blockA[i]);
	};
};
void threadLauncher(float *arrA, float *arrOut, int size, std::function<float(float)> func) // splits work into threads
{
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;
	int extra = size % NUM_THREADS;
	for (int i = 0; i < (NUM_THREADS - 1); i++)
	{
		threads[i] = std::thread(threadBlock,
			(&arrA[blockSize*i]),
			(&arrOut[blockSize*i]),
			blockSize, func);
	};
	threads[NUM_THREADS - 1] = std::thread(threadBlock,
		(&arrA[blockSize*(NUM_THREADS - 1)]),
		(&arrOut[blockSize*(NUM_THREADS - 1)]),
		blockSize + extra,
		func); // adds left over elements into last block

	for (int i = 0; i < NUM_THREADS; i++) // waits for all threads to finish
	{
		threads[i].join();
	};
};

// takes one array and a float; output arr = func( input arr, float) 
void NumthreadBlock(float *blockA, float *blockB, int size, float num, std::function<float(float, float)> func) // thread level; applies function to each element in block
{
	for (int i = 0; i < size; i++)
	{
		blockB[i] = func(blockA[i], num);
	};
};
void NumthreadLauncher(float *arrA, float *arrOut,int size, float num, std::function<float(float, float)> func) // splits work into threads
{
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;
	int extra = size % NUM_THREADS;
	for (int i = 0; i < (NUM_THREADS - 1); i++)
	{
		threads[i] = std::thread(NumthreadBlock,
			(&arrA[blockSize*i]),
			(&arrOut[blockSize*i]),
			blockSize,
			num,
			func);
	};
	threads[NUM_THREADS - 1] = std::thread(NumthreadBlock,
		(&arrA[blockSize*(NUM_THREADS - 1)]),
		(&arrOut[blockSize*(NUM_THREADS - 1)]),
		blockSize + extra,
		num,
		func); // adds left over elements into last block

	for (int i = 0; i < NUM_THREADS; i++) // waits for all threads to finish
	{
		threads[i].join();
	};
};

//takes 2 arrays; output arr = func(input arr A, input arr B) 
void OpthreadBlock(float *blockA, float *blockB, float *blockC, int size, std::function<float(float, float)> func) // thread level; applies function to each element in block
{
	for (int i = 0; i < size; i++)
	{
		blockC[i] = func(blockA[i], blockB[i]);
	};
};
void OpthreadLauncher(float *arrA, float *arrB, float *arrOut, int size, std::function<float(float, float)> func) // splits work into threads
{
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;
	int extra = size % NUM_THREADS;
	for (int i = 0; i < (NUM_THREADS - 1); i++)
	{
		threads[i] = std::thread(OpthreadBlock,
			(&arrA[blockSize*i]),
			(&arrB[blockSize*i]),
			(&arrOut[blockSize*i]),
			blockSize, func);
	};
	threads[NUM_THREADS - 1] = std::thread(OpthreadBlock,
		(&arrA[blockSize*(NUM_THREADS - 1)]),
		(&arrB[blockSize*(NUM_THREADS - 1)]),
		(&arrOut[blockSize*(NUM_THREADS - 1)]),
		blockSize + extra, func); // adds left over elements into last block
	for (int i = 0; i < NUM_THREADS; i++) // waits for all threads to finish
	{
		threads[i].join();
	};
};

void  matmulThread(Matrix *a, Matrix *b, Matrix *c, int threadnum) // thread level
{
	int N;
	int x, y;

	N = c->width * c->height;
	x = 0;
	for (int i = threadnum; i < N; i += NUM_THREADS)
	{
		x = (i / c->width) * a->width;
		y = i % c->width;
		c->arr[i] = 0;

		for (int k = 0; k < a->width; k++)
		{
			c->arr[i] += a->arr[x + k] * b->arr[k * c->width + y];
		};
	};
};

float Mult(float a, float b) { return a * b; };
float Divide(float a, float b) { return a / b; };
float Sub(float a, float b) { return a - b; };
float Add(float a, float b) { return a + b; };
float Tanh(float x) { return tanh(x); };
float Sig(float x) { return 1 / (1 + exp(x)); };
float SigPr(float x) { return Sig(x) * (1 - Sig(x));  };
float Relu(float x) {
	if (x > 0) {
		return x;
	} else {
		return 0;
	}
}
float ReluPr(float x) {
	if (x > 0) {
		return 1;
	} else {
		return 0;
	}
}

Matrix applyFunc(Matrix a, std::function<float(float)> func) // applies function element wise
{
	int size;
	{
		Matrix b(a.width, a.height);
		size = b.width * b.height;
		threadLauncher(a.arr, b.arr, size, func);
		return b;
	}
};
Matrix applyConst(Matrix a, float num, std::function<float(float, float)> func) // applies function element wise; float(float {array}, float {num})
{
	int size;
	{
		Matrix b(a.width, a.height);
		size = b.width * b.height;
		NumthreadLauncher(a.arr, b.arr, size, num, func);
		return b;
	}
};
Matrix applyOp(Matrix a, Matrix b, std::function<float(float, float)> func)
{
	int size;
	if ((a.width == b.width) && (a.height == b.height))
	{
		Matrix c(a.width, a.height);
		size = c.width * c.height;
		OpthreadLauncher(a.arr, b.arr, c.arr, size, func);
		return c;
	}
	else
	{
		std::cout << "matrices must have the same dimensions" << std::endl;
	};
}; // applies operator element wise

Matrix elemSub(Matrix a, Matrix b)
{
	return applyOp(a, b, Sub);
};
Matrix elemMult(Matrix a, Matrix b)
{
	return applyOp(a, b, Mult);
};
Matrix elemDivide(Matrix a, Matrix b)
{
	return applyOp(a, b, Divide);
};
Matrix elemAdd(Matrix a, Matrix b)
{
	return applyOp(a, b, Add);
};

Matrix constSub(Matrix a, float x)
{
	return applyConst(a, x, Sub);
};
Matrix constMult(Matrix a, float x)
{
	return applyConst(a, x, Mult);
};
Matrix constDivide(Matrix a, float x)
{
	return applyConst(a, x, Divide);
};
Matrix constAdd(Matrix a, float x)
{
	return applyConst(a, x, Add);
};

Matrix matmul(Matrix a, Matrix b)
{
	Matrix c(b.width, a.height);
	std::thread* threads;

	if (a.width == b.height)
	{

		threads = new std::thread[NUM_THREADS];

		for (int i = 0; i < NUM_THREADS; i++)
		{
			threads[i] = std::thread(matmulThread, &a, &b, &c, i);
		};
		for (int i = 0; i < NUM_THREADS; i++)
		{
			threads[i].join();
		};
		return c;
	}
	else
	{
		std::cout << "width a must equal height b";
	}
};

Matrix operator + (Matrix const &A, Matrix const &B)
{
	return elemAdd(A, B);
}
Matrix operator - (Matrix const &A, Matrix const &B)
{
	return elemSub(A, B);
}
Matrix operator * (Matrix const &A, Matrix const &B)
{
	return elemMult(A, B);
}
Matrix operator / (Matrix const &A, Matrix const &B)
{
	return elemDivide(A, B);
}
#endif
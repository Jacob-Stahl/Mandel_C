#include <iostream>
#include <random>
#include <thread>
#include <functional>
#include <math.h>
#include "global.h"
#ifndef MATRIX_H
#define MATRIX_H

template <class T>
class Matrix
{
public:
	unsigned int width, height;
	T *arr;

	Matrix(int w = 1, int h = 1) {
		width = w;
		height = h;
		arr = new T[width * height + 1];
	};
	void pprint() {
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
	};
	void zeros() {
		int range = width * height;

		for ( int i = 0; i < range; i++) {
			arr[i] = 0;
		}
	};
	void ones() {
		int range = width * height;

		for ( int i = 0; i < range; i++) {
			arr[i] = 1;
		}
	};
	void arange() {
		int range = width * height;

		for ( int i = 0; i < range; i++) {
			arr[i] = i;
		}
	};
	void random(T low = 0, T high = 1) {
		int range = width * height;
		std::uniform_real_distribution<T> dist(low, high);
		std::default_random_engine generator;

		for ( int i = 0; i < range; i++) {
			arr[i] = dist(generator);
		}
	}
	void t() {
		T *arr2, *temparr;
		arr2 = new T[width * height + 1];
		temparr = new T[width * height + 1];
		int x, temp;

		for (int i = 0; i < width; i++) {
			x = i * height;
			for (int j = 0; j < (height); j++) {
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

	friend Matrix<T> operator + (Matrix<T> const &A, Matrix<T> const &B);
	friend Matrix<T> operator - (Matrix<T> const &A, Matrix<T> const &B);
	friend Matrix<T> operator * (Matrix<T> const &A, Matrix<T> const &B);
	friend Matrix<T> operator / (Matrix<T> const &A, Matrix<T> const &B);
};

// takes one array; output arr = func( input arr)
template<typename T>
void threadBlock(T *blockA, T *blockB, int size, std::function<T(T)> func) { // thread level; applies function to each element in block
	for (int i = 0; i < size; i++) {
		blockB[i] = func(blockA[i]);
	};
};
template<typename T>
void threadLauncher(T *arrA, T *arrOut, int size, std::function<T(T)> func) { // splits work into threads
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;
	int extra = size % NUM_THREADS;
	for (int i = 0; i < (NUM_THREADS - 1); i++) {
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

	for (int i = 0; i < NUM_THREADS; i++){ // waits for all threads to finish
		threads[i].join();
	};
};

// takes one array and a T; output arr = func( input arr, T)
template<typename T>
void NumthreadBlock(T *blockA, T *blockB, int size, T num, std::function<T(T, T)> func) { // thread level; applies function to each element in block
	for (int i = 0; i < size; i++) {
		blockB[i] = func(blockA[i], num);
	};
};
template<typename T>
void NumthreadLauncher(T *arrA, T *arrOut,int size, T num, std::function<T(T, T)> func) {// splits work into threads
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;
	int extra = size % NUM_THREADS;
	for (int i = 0; i < (NUM_THREADS - 1); i++) {
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

	for (int i = 0; i < NUM_THREADS; i++){ // waits for all threads to finish
		threads[i].join();
	};
};

//takes 2 arrays; output arr = func(input arr A, input arr B)
template<typename T>
void OpthreadBlock(T *blockA, T *blockB, T *blockC, int size, std::function<T(T, T)> func) { // thread level; applies function to each element in block
	for (int i = 0; i < size; i++) {
		blockC[i] = func(blockA[i], blockB[i]);
	};
};
template<typename T>
void OpthreadLauncher(T *arrA, T *arrB, T *arrOut, int size, std::function<T(T, T)> func) { // splits work into threads
	std::thread threads[NUM_THREADS];
	int blockSize = size / NUM_THREADS;
	int extra = size % NUM_THREADS;
	for (int i = 0; i < (NUM_THREADS - 1); i++) {
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
	for (int i = 0; i < NUM_THREADS; i++) { // waits for all threads to finish
		threads[i].join();
	};
};
template<typename T>
void  matmulThread(Matrix<T>  *a, Matrix<T> *b, Matrix<T> *c, int threadnum) { // thread level
	int N;
	int x, y;

	N = c->width * c->height;
	x = 0;
	for (int i = threadnum; i < N; i += NUM_THREADS) {
		x = (i / c->width) * a->width;
		y = i % c->width;
		c->arr[i] = 0;

		for (int k = 0; k < a->width; k++) {
			c->arr[i] += a->arr[x + k] * b->arr[k * c->width + y];
		};
	};
};

template<typename T>
T Mult(T a, T b) { return a * b; };
template<typename T>
T Divide(T a, T b) { return a / b; };
template<typename T>
T Sub(T a, T b) { return a - b; };
template<typename T>
T Add(T a, T b) { return a + b; };
template<typename T>
T Tanh(T x) { return tanh(x); };
template<typename T>
T Sig(T x) { return 1 / (1 + exp(x)); };
template<typename T>
T SigPr(T x) { return Sig(x) * (1 - Sig(x));  };
template<typename T>
T Relu(T x) {
	if (x > 0) {
		return x;
	} else {
		return 0;
	}
}
template<typename T>
T ReluPr(T x) {
	if (x > 0) {
		return 1;
	} else {
		return 0;
	}
}
template<typename T>
Matrix<T> applyFunc(Matrix<T> a, std::function<T(T)> func) { // applies function element wise
	int size;
	Matrix<T> b(a.width, a.height);
	size = b.width * b.height;
	threadLauncher(a.arr, b.arr, size, func);
	return b;
};
template<typename T>
Matrix<T> applyConst(Matrix<T> a, T num, std::function<T(T, T)> func) {// applies function element wise; T(T {array}, T {num})
	int size;
	Matrix<T> b(a.width, a.height);
	size = b.width * b.height;
	NumthreadLauncher(a.arr, b.arr, size, num, func);
	return b;
};
template<typename T>
Matrix<T> applyOp(Matrix<T> a, Matrix<T> b, std::function<T(T, T)> func) {
	int size;
	if ((a.width == b.width) && (a.height == b.height)) {
		Matrix<T> c(a.width, a.height);
		size = c.width * c.height;
		OpthreadLauncher(a.arr, b.arr, c.arr, size, func);
		return c;
	} else {
		std::cout << "matrices must have the same dimensions" << std::endl;
	};
}; // applies operator element wise

template<typename T>
Matrix<T> elemSub(Matrix<T> a, Matrix<T> b) { return applyOp(a, b, Sub); };
template<typename T>
Matrix<T> elemMult(Matrix<T> a, Matrix<T> b) { return applyOp(a, b, Mult); };
template<typename T>
Matrix<T> elemDivide(Matrix<T> a, Matrix<T> b) { return applyOp(a, b, Divide); };
template<typename T>
Matrix<T> elemAdd(Matrix<T> a, Matrix<T> b) { return applyOp(a, b, Add); };

template<typename T>
Matrix<T> constSub(Matrix<T> a, T x) { return applyConst(a, x, Sub); };
template<typename T>
Matrix<T> constMult(Matrix<T> a, T x) { return applyConst(a, x, Mult); };
template<typename T>
Matrix<T> constDivide(Matrix<T> a, T x) { return applyConst(a, x, Divide); };
template<typename T>
Matrix<T> constAdd(Matrix<T> a, T x) { return applyConst(a, x, Add); };

template<typename T>
Matrix<T> matmul(Matrix<T> a, Matrix<T> b) {
	Matrix<T> c(b.width, a.height);
	std::thread* threads;
	if (a.width == b.height) {
		threads = new std::thread[NUM_THREADS];
		for (int i = 0; i < NUM_THREADS; i++) {
			threads[i] = std::thread(matmulThread, &a, &b, &c, i);
		};
		for (int i = 0; i < NUM_THREADS; i++) {
			threads[i].join();
		};
		return c;
	} else {
		std::cout << "width a must equal height b";
	}
};
template<typename T>
Matrix<T> operator + (Matrix<T> const &A, Matrix<T> const &B) { return elemAdd(A, B); }
template<typename T>
Matrix<T> operator - (Matrix<T> const &A, Matrix<T> const &B) { return elemSub(A, B); }
template<typename T>
Matrix<T> operator * (Matrix<T> const &A, Matrix<T> const &B) { return elemMult(A, B); }
template<typename T>
Matrix<T> operator / (Matrix<T> const &A, Matrix<T> const &B) { return elemDivide(A, B);}

#endif
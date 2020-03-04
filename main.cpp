#include <iostream>
#include "frac_func.h"
#include "global.h"
#include "matrix.h"
using namespace std;

int main() {
    int result;
    Matrix<double> A;
    Matrix<double> B;


    A.arange();
    B.random(1, 2);
    Matrix<double> C =  A * B;

    C.pprint();
    return 0;
}


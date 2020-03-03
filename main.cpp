#include <iostream>
#include "frac_func.h"
#include "global.h"
using namespace std;

const int ref_zoom = 2; // distance on the complex plane at zoom x1
int dim_x = 1100; // x and y dims of screen
int dim_y = 1100;
const int N = 128; // num threads

int main() {
    int result;
    cout << "hello world" << endl;
    result =  brot_iter(.9, .6232);
    cout << result;
    cout << "    " << MAX_ITER;
    return 0;
}


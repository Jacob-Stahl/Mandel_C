#include <iostream>
#include "frac_func.h"
#include "global.h"
using namespace std;


int main() {
    int result;
    cout << "hello world" << endl;
    result =  brot_iter(.9, .6232);
    cout << result;
    cout << "    " << MAX_ITER;
    return 0;
}


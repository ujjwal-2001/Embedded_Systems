#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evaluate.h"

int main() {
    char expression[]= "2*sin(x)+log(x)";
    double** xy = xy_vals(100, 0, 10, expression);
    for (int i = 0; i < 100; i++) {
        printf("%f, %f\n", xy[0][i], xy[1][i]);
    }

    return 0;
}
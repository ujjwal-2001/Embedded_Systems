#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evaluate.h"

void save_to_txt1(double points[2][N], const char *filename);
void save_to_txt2(double points[2][N-1], const char *filename);

int main() {
    initialize();                       // step 1: initialize the global variables
    strcpy(__EXPR__, "(x^2)-2");    // step 2: change the expression
    __X_MIN__ = -10;                    // step 3: change the minimum x value
    __X_MAX__ = 10;                     // step 4: change the maximum x value
    xy_vals();                          // step 5: generate x and y values
    __MP_X1__ = 0;                      // step 6.1: change the x1 value for mapping
    __MP_Y1__ = 0;                      // step 6.2: change the y1 value for mapping
    __MP_X2__ = 800;                    // step 6.3: change the x2 value for mapping
    __MP_Y2__ = 600;                    // step 6.4: change the y2 value for mapping
    map_xy();                           // step 7: map x and y values to screen coordinates
    derivative();                       // step 8: calculate the derivative of a function
    map_dx_dy();                        // step 9: map the derivative values
    integral();                         // step 10: calculate the integral of a function
    map_integral();                     // step 11: map the integral values
    area_under_curve();                 // step 12: calculate the area under the curve of a function
    zeros_of_function();                // step 13: calculate the zeros of a function
    map_zeros();                        // step 14: map the zeros of a function

    // Printing the x and y values
    // printf("x and y values\n");
    // for (int i = 0; i < N; i++) {
    //     printf("%f %f\n", __XY__[0][i], __XY__[1][i]);
    // }

    // printing Y amx and y min for various graphs
    printf("Y min and Y max\n");
    printf("Y min: %f\n", __Y_MIN__);
    printf("Y max: %f\n", __Y_MAX__);
    printf("Y min dy/dx: %f\n", __Y_MIN_DY_DX__);
    printf("Y max dy/dx: %f\n", __Y_MAX_DY_DX__);
    printf("Y min integral: %f\n", __Y_MIN_INTEGRAL__);
    printf("Y max integral: %f\n", __Y_MAX_INTEGRAL__);

    // Printing Zeros of the function
    printf("Zeros of the function\n");
    print_stack(&__ZEROS__);

    // Printing mapped zeros of the function
    printf("Mapped zeros of the function\n");
    print_stack(&__MAPPED_ZEROS__);
    
    // Printing the area under the curve
    printf("Area under the curve: %f\n", __AREA__);

    save_to_txt1(__XY__,  "points.txt ");
    printf("Data saved to %s\n", "points.txt ");
    save_to_txt1(__MAPPED_XY__,  "points_mapped.txt");
    printf("Data saved to %s\n", "points_mapped.txt ");
    save_to_txt2(__DY_DX__,  "derivative.txt");
    printf("Data saved to %s\n", "derivative.txt ");
    save_to_txt2(__MAPPED_DY_DX__,  "map_derivative.txt");
    printf("Data saved to %s\n", "map_derivative.txt ");
    save_to_txt2(__INTEGRAL_XY__,  "integral.txt");
    printf("Data saved to %s\n", "integral.txt ");
    save_to_txt2(__MAPPED_INTEGRAL_XY__,  "map_integral.txt");
    printf("Data saved to %s\n", "map_integral.txt ");
    
    return 0;
}


void save_to_txt1(double points[2][N], const char *filename) {
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }

    // Write the x and y values to the file
    for (int i = 0; i < N; i++) {
        // if number if inf or na skip
        if (isinf(points[0][i]) || isnan(points[0][i]) || isinf(points[1][i]) || isnan(points[1][i])) {
            continue;
        }
        fprintf(file, "%f %f\n", points[0][i], points[1][i]);
    }

    // Close the file
    fclose(file);
}

void save_to_txt2(double points[2][N-1], const char *filename) {
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }

    // Write the x and y values to the file
    for (int i = 0; i < N-1; i++) {
        // if number if inf or na skip
        if (isinf(points[0][i]) || isnan(points[0][i]) || isinf(points[1][i]) || isnan(points[1][i])) {
            continue;
        }
        fprintf(file, "%f %f\n", points[0][i], points[1][i]);
    }

    // Close the file
    fclose(file);
}
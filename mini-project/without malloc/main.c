#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evaluate.h"

void save_to_txt1(double points[2][N], const char *filename);
void save_to_txt2(double points[2][N-1], const char *filename);

int main() {
    initialize();
    strcpy(__EXPR__, "sin(x+2) + log(x)");
    __X_MIN__ = -10;
    __X_MAX__ = 10;
    xy_vals();
    // Printing the x and y values
    printf("x and y values\n");
    for (int i = 0; i < N; i++) {
        printf("%f %f\n", __XY__[0][i], __XY__[1][i]);
    }

    //Calculation of __ZEROS__
    // zeros_of_function();
    // printf("Zeros of the function\n");
    // print_stack(&__ZEROS__);

    derivative();
    integral();
    area_under_curve();
    map_xy(0, 0, 800, 600);

    save_to_txt1(__XY__,  "points.txt ");
    printf("Data saved to %s\n", "points.txt ");
    save_to_txt1(__MAPPED_XY__,  "points_mapped.txt");
    printf("Data saved to %s\n", "points_mapped.txt ");
    save_to_txt2(__DY_DX__,  "derivative.txt");
    printf("Data saved to %s\n", "derivative.txt ");
    save_to_txt2(__INTEGRAL_XY__,  "integral.txt");
    printf("Data saved to %s\n", "integral.txt ");
    
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
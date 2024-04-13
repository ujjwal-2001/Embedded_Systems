#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evaluate.h"

void save_to_txt(double** points, int num_points, const char *filename);

int main() {
    char expression[]= "(x-4)*(x+2)*x*(x-1)";
    int min = -5;
    int max = 5;
    int n = 50;
    double** xy = xy_vals(n, min, max, expression);

    double** xy_mapped = map_xy(n, xy, 0, 0, 100, 100, min, max);

    double** dy = derivative(n, xy);
    double** integral_xy = integral(n, xy);

    // Calculation of zeros
    Stack zeros = zeros_of_function(n, xy, expression);
    printf("Zeros of the function\n");
    print_stack(&zeros);

    save_to_txt(xy, n, "points.txt ");
    printf("Data saved to %s\n", "points.txt ");
    save_to_txt(xy_mapped, n, "points_mapped.txt");
    printf("Data saved to %s\n", "points_mapped.txt ");
    save_to_txt(dy, n, "derivative.txt");
    printf("Data saved to %s\n", "derivative.txt ");
    save_to_txt(integral_xy, n, "integral.txt");
    printf("Data saved to %s\n", "integral.txt ");

    // Free the memory
    free(xy[0]);
    free(xy[1]);
    free(xy_mapped[0]);
    free(xy_mapped[1]);
    free(dy[0]);
    free(dy[1]);
    free(integral_xy[0]);
    free(integral_xy[1]);
    return 0;
}


void save_to_txt(double** points, int num_points, const char *filename) {
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }

    // Write the x and y values to the file
    for (int i = 0; i < num_points; i++) {
        // if number if inf or nan, skip
        // if (isinf(points[0][i]) || isnan(points[0][i]) || isinf(points[1][i]) || isnan(points[1][i])) {
        //     continue;
        // }
        fprintf(file, "%f %f\n", points[0][i], points[1][i]);
    }

    // Close the file
    fclose(file);
}
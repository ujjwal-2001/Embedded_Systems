#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evaluate.h"

void save_to_txt(double** points, int num_points, const char *filename);

int main() {
    char expression[]= "-sin(-x)";
    int min = -10;
    int max = 10;
    int n = 120;
    double** xy = xy_vals(n, min, max, expression);

    // print the x and y values
    for (int i = 0; i < n; i++) {
        printf("%f %f\n", xy[0][i], xy[1][i]);
    }

    double** xy_mapped = map_xy(n, xy, 0, 0, 100, 100, min, max);

    // print the mapped x and y values
    printf("Mapped x and y values\n");
    for (int i = 0; i < n; i++) {
        printf("%f %f\n", xy_mapped[0][i], xy_mapped[1][i]);
    }

    save_to_txt(xy, n, "points.txt ");
    printf("Data saved to %s\n", "points.txt ");
    save_to_txt(xy_mapped, n, "points_mapped.txt");
    printf("Data saved to %s\n", "points_mapped.txt ");

    free(xy[0]);
    free(xy[1]);
    free(xy_mapped[0]);
    free(xy_mapped[1]);
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
        if (isinf(points[0][i]) || isnan(points[0][i]) || isinf(points[1][i]) || isnan(points[1][i])) {
            continue;
        }
        fprintf(file, "%f %f\n", points[0][i], points[1][i]);
    }

    // Close the file
    fclose(file);
}
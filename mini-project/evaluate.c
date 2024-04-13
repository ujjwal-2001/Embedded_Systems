#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "evaluate.h"

//---------------EVALUATION OF MATHEMATICAL EXPRESSIONS----------------

// Initialize stack
void init(Stack* s) {
    s->top = -1;
}

// Push item onto stack
void push(Stack* s, double val) {
    if (s->top == MAX_STACK_SIZE - 1) {
        printf("Stack Overflow\n");
        exit(EXIT_FAILURE);
    }
    s->items[++(s->top)] = val;
}

// Pop item from stack
double pop(Stack* s) {
    if (s->top == -1) {
        printf("Stack Underflow\n");
        exit(EXIT_FAILURE);
    }
    double val = s->items[(s->top)--];
    return val;
}

// Peek at the top item of the stack
double peek(Stack* s) {
    if (s->top == -1) {
        printf("Stack Empty\n");
        exit(EXIT_FAILURE);
    }
    return s->items[s->top];
}

// Check if stack is empty
int isempty(Stack* s) {
    return s->top == -1;
}

// Print stack
void print_stack(Stack* s) {
    for (int i = 0; i <= s->top; i++) {
        printf("%f ", s->items[i]);
    }
    printf("\n");
}

// Operator precedence
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// Apply operator to operands
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

// Evaluate the expression
double eval(const char* expr) {
    Stack valStack;
    Stack opStack;
    init(&valStack);
    init(&opStack);

    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isdigit(expr[i]) || (expr[i] == '.' && isdigit(expr[i + 1]))) {
            char* end;
            double num = strtod(&expr[i], &end);
            push(&valStack, num);
            i = end - expr;
            i--;
        } else if (isalpha(expr[i])) {
            // Check for trigonometric or log functions
            if (strncmp(&expr[i], "sin", 3) == 0) {
                i += 3;
                push(&opStack, 's');
            } else if (strncmp(&expr[i], "cos", 3) == 0) {
                i += 3;
                push(&opStack, 'c');
            } else if (strncmp(&expr[i], "tan", 3) == 0) {
                i += 3;
                push(&opStack, 't');
            } else if (strncmp(&expr[i], "log", 3) == 0) {
                i += 3;
                push(&opStack, 'l');
            } else if (strncmp(&expr[i], "abs", 3) == 0) {
                i += 3;
                push(&opStack, 'a');
            } else {
                printf("Invalid function\n");
                exit(EXIT_FAILURE);
            }
            i--;
        } else if (expr[i] == '(') {
            push(&opStack, '(');
        } else if (expr[i] == ')') {
            while (peek(&opStack) != '(') {
                double b = pop(&valStack);
                char op = pop(&opStack);
                if (op == 's') {
                    push(&valStack, sin(b));
                } else if (op == 'c') {
                    push(&valStack, cos(b));
                } else if (op == 't') {
                    push(&valStack, tan(b));
                } else if (op == 'l') {
                    if (b < 0) {    // log of negative number is undefined
                        return NAN;
                    }
                    push(&valStack, log(b));
                } else if (op == 'a') {
                    push(&valStack, abs(b));
                } else {
                    double a = pop(&valStack);
                    push(&valStack, applyOp(a, b, op));
                }
            }
            pop(&opStack); // Remove '('
        } else {
            while (!isempty(&opStack) && precedence(expr[i]) <= precedence(peek(&opStack))) {
                double b = pop(&valStack);
                char op = pop(&opStack);
                if (op == 's') {
                    push(&valStack, sin(b));
                } else if (op == 'c') {
                    push(&valStack, cos(b));
                } else if (op == 't') {
                    push(&valStack, tan(b));
                } else if (op == 'l') {
                    if (b < 0) {    // log of negative number is undefined
                        return NAN;
                    }
                    push(&valStack, log(b));
                } else if (op == 'a') {
                    push(&valStack, abs(b));
                } else {
                    double a = pop(&valStack);
                    push(&valStack, applyOp(a, b, op));
                }
            }
            push(&opStack, expr[i]);
        }
        i++;
    }

    while (!isempty(&opStack)) {
        double b = pop(&valStack);
        char op = pop(&opStack);
        if (op == 's') {
            push(&valStack, sin(b));
        } else if (op == 'c') {
            push(&valStack, cos(b));
        } else if (op == 't') {
            push(&valStack, tan(b));
        } else if (op == 'l') {
            if (b < 0) {    // log of negative number is undefined
                return NAN;
            }
            push(&valStack, log(b));
        } else if (op == 'a') {
            push(&valStack, abs(b));
        } else{
            double a = pop(&valStack);
            push(&valStack, applyOp(a, b, op));
        }
    }
    return pop(&valStack);
}


//---------------GENERATION OF X AND Y VALUES----------------

// Envolopes sin, cos, tan, log, and abs in brackets
// Example input: "2*sin(3.14)+log(100)"
// Example output: "2*(sin(3.14))+(log(100))"
char* bracket_adder(char* expr) {
    char* new_expr = malloc(strlen(expr) * 2 + 1); // Maximum length of new expression is twice the length of the original expression
    int j = 0;

    if (expr[0]!='\0' && expr[0]=='-')
    {
        new_expr[j++] = '0';
    } 

    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == 's' || expr[i] == 'c' || expr[i] == 't' || expr[i] == 'l' || expr[i] == 'a') {
            new_expr[j++] = '(';
            while(expr[i] != ')') {
                if (expr[i] == '(' && expr[i+1] == '-'){
                    new_expr[j++] = '(';
                    new_expr[j++] = '0';
                    i++;
                }
                new_expr[j++] = expr[i++];
            }
            new_expr[j++] = expr[i];
            new_expr[j++] = ')';

        } else if (expr[i] == '(' && expr[i+1] == '-') { // Replace (- with (0-
            new_expr[j++] = '(';
            new_expr[j++] = '0';
            new_expr[j++] = '-';
            i++;
        } else {
            new_expr[j++] = expr[i];
        }
    }
    new_expr[j] = '\0';
    return new_expr;
}

// Generate x values
double* x_vals(int n, int min, int max) {
    double* x = malloc(n * sizeof(double));
    double step_size = ((double)max - (double)min) / ((double)n - 1);

    for (int i = 0; i < n; i++) {
        x[i] = min + i * step_size;
    }
    return x;
}

// Replace variable with value and replace pi and e
char* val_replacer(char* expr, double val) {
    char* new_expr = malloc(strlen(expr) * 2 + 100);
    int j = 0;

    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == 'x') {   // Replace x with value with (0val)
            new_expr[j++] = '(';
            new_expr[j++] = '0';
            char* num = malloc(20);
            sprintf(num, "%f", val);
            for (int k = 0; num[k] != '\0'; k++) {
                new_expr[j++] = num[k];
            }
            free(num);
            new_expr[j++] = ')';
        } else if (expr[i] == 'p' && expr[i + 1] == 'i'){
            new_expr[j++] = '3';
            new_expr[j++] = '.';
            new_expr[j++] = '1';
            new_expr[j++] = '4';
            new_expr[j++] = '1';
            new_expr[j++] = '5';
            new_expr[j++] = '9';
            new_expr[j++] = '2';
            new_expr[j++] = '6';
            new_expr[j++] = '5';
        } else if (expr[i] == 'e') {
            new_expr[j++] = '2';
            new_expr[j++] = '.';
            new_expr[j++] = '7';
            new_expr[j++] = '1';
            new_expr[j++] = '8';
            new_expr[j++] = '2';
            new_expr[j++] = '8';
            new_expr[j++] = '4';
            new_expr[j++] = '5';
            new_expr[j++] = '9';
            new_expr[j++] = '0';
            new_expr[j++] = '4';
            new_expr[j++] = '5';
        } else {
            new_expr[j++] = expr[i];
        }
    }
    new_expr[j] = '\0';
    return new_expr;
}

// Generate y values corresponding to x values
double* y_vals(int n, double* x, char* expr) {
    double* y = malloc(n * sizeof(double));
    char* corrected_expr = bracket_adder(expr);
    for (int i = 0; i < n; i++) {
        char* new_expr = val_replacer(corrected_expr, x[i]);
        y[i] = eval(new_expr);
        free(new_expr);
    }
    return y;
}

// Generate x and y values
double** xy_vals(int n, int min, int max, char* expr) {
    double** xy = malloc(2 * sizeof(double*));
    xy[0] = x_vals(n, min, max);
    xy[1] = y_vals(n, xy[0], expr);
    return xy;
}

//---------------ADDITIONAL OPERATIONS ON X AND Y VALUES----------------

// Calculate the derivative of a function
double ** derivative(int n, double** xy) {
    double** dy_dx = malloc(2 * sizeof(double*));
    dy_dx[0] = malloc(n * sizeof(double));
    dy_dx[1] = malloc(n * sizeof(double));

    for (int i = 0; i < n - 1; i++) {
        dy_dx[0][i] = xy[0][i];
        dy_dx[1][i] = (xy[1][i + 1] - xy[1][i]) / (xy[0][i + 1] - xy[0][i]);
    }
    dy_dx[0][n - 1] = xy[0][n - 1];
    dy_dx[1][n - 1] = dy_dx[1][n - 2];

    return dy_dx;
}

// Calculate the integral of a function
double ** integral(int n, double** xy) {
    double** integral = malloc(2 * sizeof(double*));
    integral[0] = malloc(n * sizeof(double));
    integral[1] = malloc(n * sizeof(double));

    integral[0][0] = xy[0][0];
    integral[1][0] = 0;

    for (int i = 1; i < n; i++) {
        integral[0][i] = xy[0][i];
        integral[1][i] = integral[1][i - 1] + (xy[1][i] + xy[1][i - 1]) * (xy[0][i] - xy[0][i - 1]) / 2;
    }

    return integral;
}

// Calculate the area under the curve of a function
double area_under_curve(int n, double** xy) {
    double area = 0;
    for (int i = 1; i < n; i++) {
        area += (xy[1][i] + xy[1][i - 1]) * (xy[0][i] - xy[0][i - 1]) / 2;
    }
    return area;
}

// Calculate the bisection points of a function
double ** bisection_points(int n, double** xy) {
    double** bisection = malloc(2 * sizeof(double*));
    bisection[0] = malloc(MAX_ZEROS * 2 * sizeof(double));
    bisection[1] = malloc(MAX_ZEROS * 2 * sizeof(double));
    int j=0;

    //populate bisection points with NAN
    for (int i = 0; i < MAX_ZEROS * 2; i++) {
        bisection[0][i] = NAN;
        bisection[1][i] = NAN;
    }

    for (int i = 0; i < n - 1; i++) {
        double x1 = xy[0][i];
        double x2 = xy[0][i + 1];
        double y1 = xy[1][i];
        double y2 = xy[1][i + 1];
        if (y1 * y2 < 0) {
            bisection[0][j] = x1;
            bisection[1][j] = y1;
            bisection[0][j + 1] = x2;
            bisection[1][j + 1] = y2;
            j += 2;
        }
    }

    // printing all the values
    printf("+++Bisection points\nx     y\n");
    for (int i = 0; i < MAX_ZEROS * 2; i += 2) {
        printf("%f %f\n", bisection[0][i], bisection[1][i]);
        printf("%f %f\n", bisection[0][i + 1], bisection[1][i + 1]);
    }

    return bisection;
}

// Calculate the root using the bisection method with given points
// returns NAN if the points do not have opposite signs or the root is not found
double bisection_method(double point1[1][2], double point2[1][2], char* expr) {
    double x1 = point1[0][0];
    double x2 = point2[0][0];
    double y1 = point1[0][1];
    double y2 = point2[0][1];
    char* corrected_expr = bracket_adder(expr);

    if (y1 * y2 >= 0) {
        printf("The points do not have opposite signs\n");
        return NAN;
    }

    double x_mid = (x1 + x2) / 2;
    double y_mid = eval(val_replacer(corrected_expr, x_mid));

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (y_mid == 0 || (x2 - x1) / 2 < EPSILON) {
            return x_mid;
        }

        if (y_mid * y1 < 0) {
            x2 = x_mid;
            y2 = y_mid;
        } else {
            x1 = x_mid;
            y1 = y_mid;
        }

        x_mid = (x1 + x2) / 2;
        y_mid = eval(val_replacer(corrected_expr, x_mid));
    }

    printf("Root not found\n");
    return NAN;
}

// Calculate the zeros of a function
Stack zeros_of_function(int n, double** xy, char* expr) {
    Stack zeros;
    init(&zeros);

    // Find bisection points
    double** bisection = bisection_points(n, xy);

    // Find zeros
    for (int i = 0; i < MAX_ZEROS * 2; i += 2) {
        if(!isnan(bisection[0][i])){
            double point1[1][2] = {{bisection[0][i], bisection[1][i]}};
            double point2[1][2] = {{bisection[0][i + 1], bisection[1][i + 1]}};
            double zero = bisection_method(point1, point2, expr);
            if (!isnan(zero)) {
                push(&zeros, zero);
            }
        }
    }

    printf("+++Zeros of the function\n");
    print_stack(&zeros);
    free(bisection[0]);
    free(bisection[1]);

    return zeros;
}



//---------------MAPPING OF X AND Y VALUES TO SCREEN COORDINATES----------------

// Find maximum value in an array of doubles ignoring NaN and Inf values
double max(double* arr, int n) {
    // initial value should not be NaN or Inf
    int i=0;
    double max;
    while(isnan(arr[i]) || isinf(arr[i])) {
        i++;
    }
    max = arr[i++];
    for (; i < n; i++) {
        if (arr[i] > max &&  !(isnan(arr[i]) || isinf(arr[i]))) {
            max = arr[i];
        }
    }
    return max;
}

// Find minimum value in an array of doubles ignoring NaN and Inf values
double min(double* arr, int n) {
    // initial value should not be NaN or Inf
    int i=0;
    double min;
    while(isnan(arr[i]) || isinf(arr[i])) {
        i++;
    }
    min = arr[i++];

    for (; i < n; i++) {
        if (arr[i] < min &&  !(isnan(arr[i]) || isinf(arr[i]))) {
            min = arr[i];
        }
    }
    
    return min;
}

// Map x and y values to screen coordinates
// (x1, y1), (x2, y2) are the screen coordinates - corners of the screen in pixcels
// (x_min, x_max) are the minimum and maximum values of x
double** map_xy(int n, double** xy, int x1, int y1,  int x2, int y2, double x_min, double x_max) {
    double** mapped_xy = malloc(2 * sizeof(double*));
    mapped_xy[0] = malloc(n * sizeof(double));
    mapped_xy[1] = malloc(n * sizeof(double));

    double y_min = min(xy[1], n);
    double y_max = max(xy[1], n);
    double x_range = x_max - x_min;
    double y_range = y_max - y_min;
    double x_scale = (x2 - x1) / x_range;
    double y_scale = (y2 - y1) / y_range;
    double x_offset = x1 - x_scale * x_min;
    double y_offset = y1 - y_scale * y_min;

    for (int i = 0; i < n; i++) {
        mapped_xy[0][i] = x_scale * xy[0][i] + x_offset;
        mapped_xy[1][i] = y_scale * xy[1][i] + y_offset;
    }

    return mapped_xy;
}
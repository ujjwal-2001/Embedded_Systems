#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "evaluate.h"

// GLOBAL VARIABLES
char __EXPR__[EXP_LEN];                  // expression to be evaluated (x)
char __EXPR_VAL__[EXP_LEN];              // expression to be evaluated (number)
int __BRACKET_FLAG__;                    // flag to check if brackets are added
double __X_MIN__;                        // minimum x value
double __X_MAX__;                        // maximum x value
double __Y_MIN__;                        // minimum y value                      
double __Y_MAX__;                        // maximum y value                      
double __Y_MIN_DY_DX__;                  // minimum y value of dy/dx             
double __Y_MAX_DY_DX__;                  // maximum y value of dy/dx             
double __Y_MIN_INTEGRAL__;               // minimum y value of integral of y     
double __Y_MAX_INTEGRAL__;               // maximum y value of integral of y  
double __MP_X1__;                        // x1 value for mapping
double __MP_Y1__;                        // y1 value for mapping
double __MP_X2__;                        // x2 value for mapping
double __MP_Y2__;                        // y2 value for mapping  
double __XY__[2][N];                     // x values
double __MAPPED_XY__[2][N];              // x values mapped to screen coordinates
double __DY_DX__[2][N-1];                // derivative values
double __MAPPED_DY_DX__[2][N-1];         // derivative values
double __INTEGRAL_XY__[2][N-1];          // integral values
double __MAPPED_INTEGRAL_XY__[2][N-1];   // integral values
double __AREA__;                         // __AREA__ under the curve
double __BISECTION__[2][MAX_ZEROS*2];      // bisection points
Stack __ZEROS__;                         // zeros of the function

// INITIALIZATION
void initialize() {
    strcpy(__EXPR__, "");
    __BRACKET_FLAG__ = 0;
    init(&__ZEROS__);
    __AREA__ = 0;
    __X_MAX__ = 0;
    __X_MIN__ = 0;
    __Y_MAX__ = 0;
    __Y_MIN__ = 0;
    __Y_MAX_DY_DX__ = 0;
    __Y_MIN_DY_DX__ = 0;
    __Y_MAX_INTEGRAL__ = 0;
    __Y_MIN_INTEGRAL__ = 0;
    __MP_X1__ = 0;
    __MP_Y1__ = 0;
    __MP_X2__ = 0;
    __MP_Y2__ = 0;
    __ZOOMING__ = ZOOM_FACTOR;
    __SHIFTING__ = SHIFT_FACTOR;
    for (int i = 0; i < N; i++) {
        __XY__[0][i] = 0;
        __XY__[1][i] = 0;
        __MAPPED_XY__[0][i] = 0;
        __MAPPED_XY__[1][i] = 0;
    }
    for (int i = 0; i < N - 1; i++) {
        __DY_DX__[0][i] = 0;
        __DY_DX__[1][i] = 0;
        __MAPPED_DY_DX__[0][i] = 0;
        __MAPPED_DY_DX__[1][i] = 0;
        __INTEGRAL_XY__[0][i] = 0;
        __INTEGRAL_XY__[1][i] = 0;
        __MAPPED_INTEGRAL_XY__[0][i] = 0;
        __MAPPED_INTEGRAL_XY__[1][i] = 0;
    }
}

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
        case '/': {
            if (b == 0) {
                return NAN;
            }
            return a / b;
        }
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
void bracket_adder() {
    if(!__BRACKET_FLAG__){

        char new_expr[strlen(__EXPR__) * 2 + 1]; // Maximum length of new expression is twice the length of the original expression
        int j = 0;

        if (__EXPR__[0]!='\0' && __EXPR__[0]=='-')
        {
            new_expr[j++] = '0';
        } 

        for (int i = 0; __EXPR__[i] != '\0'; i++) {
            if (__EXPR__[i] == 's' || __EXPR__[i] == 'c' || __EXPR__[i] == 't' || __EXPR__[i] == 'l' || __EXPR__[i] == 'a') {
                new_expr[j++] = '(';
                while(__EXPR__[i] != ')') {
                    if (__EXPR__[i] == '(' && __EXPR__[i+1] == '-'){
                        new_expr[j++] = '(';
                        new_expr[j++] = '0';
                        i++;
                    }
                    new_expr[j++] = __EXPR__[i++];
                }
                new_expr[j++] = __EXPR__[i];
                new_expr[j++] = ')';

            } else if (__EXPR__[i] == '(' && __EXPR__[i+1] == '-') { // Replace (- with (0-
                new_expr[j++] = '(';
                new_expr[j++] = '0';
                new_expr[j++] = '-';
                i++;
            } else {
                new_expr[j++] = __EXPR__[i];
            }
        }
        new_expr[j] = '\0';
        strcpy(__EXPR__, new_expr);
        __BRACKET_FLAG__ = 1;
    }
}

// Generate x values
void x_vals() {
    double step_size = ((double)__X_MAX__ - (double)__X_MIN__) / ((double)N - 1);

    for (int i = 0; i < N; i++) {
        __XY__[0][i] = __X_MIN__ + i * step_size;
    }
}

// Replace variable with value and replace pi and e
void val_replacer(double val) {
    char new_expr[EXP_LEN];
    int j = 0;

    for (int i = 0; __EXPR__[i] != '\0'; i++) {
        if (__EXPR__[i] == 'x') {   // Replace x with value with (0val)
            new_expr[j++] = '(';
            new_expr[j++] = '0';
            char num[50];
            sprintf(num, "%f", val);
            for (int k = 0; num[k] != '\0'; k++) {
                new_expr[j++] = num[k];
            }
            new_expr[j++] = ')';
        } else if (__EXPR__[i] == 'p' && __EXPR__[i + 1] == 'i'){
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
        } else if (__EXPR__[i] == 'e') {
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
            new_expr[j++] = __EXPR__[i];
        }
    }
    new_expr[j] = '\0';
    strcpy(__EXPR_VAL__, new_expr);
}

// Generate y values corresponding to x values
void y_vals() {
    bracket_adder();
    for (int i = 0; i < N; i++) {
        val_replacer(__XY__[0][i]);
        __XY__[1][i] = eval(__EXPR_VAL__);
    }
    __Y_MIN__ = min(__XY__[1], N);
    __Y_MAX__ = max(__XY__[1], N);
}

void xy_vals() {
    x_vals();
    y_vals();
}

//---------------ADDITIONAL OPERATIONS ON X AND Y VALUES----------------

// Calculate the derivative of a function
void derivative() {
    for (int i = 0; i < N - 1; i++) {
        __DY_DX__[0][i] = (__XY__[0][i + 1] + __XY__[0][i]) / 2;
        __DY_DX__[1][i] = (__XY__[1][i + 1] - __XY__[1][i]) / (__XY__[0][i + 1] - __XY__[0][i]);
    }
    __Y_MIN_DY_DX__ = min(__DY_DX__[1], N - 1);
    __Y_MAX_DY_DX__ = max(__DY_DX__[1], N - 1);
}

// Calculate the integral of a function
void integral() {

    double last_valid_value = 0;

    for(int i=0; i<N-1; i++){
        if (isinf(__XY__[1][i]) || isnan(__XY__[1][i]) || isinf(__XY__[1][i+1]) || isnan(__XY__[1][i+1])) {
            __INTEGRAL_XY__[0][i] = (__XY__[0][i + 1] + __XY__[0][i]) / 2;
            __INTEGRAL_XY__[1][i] = NAN;
            continue;
        }
        __INTEGRAL_XY__[0][i] = (__XY__[0][i + 1] + __XY__[0][i]) / 2;
        __INTEGRAL_XY__[1][i] = last_valid_value + (__XY__[1][i] + __XY__[1][i + 1]) * (__XY__[0][i + 1] - __XY__[0][i]) / 2;
        last_valid_value = __INTEGRAL_XY__[1][i];
    }
    __Y_MIN_INTEGRAL__ = min(__INTEGRAL_XY__[1], N - 1);
    __Y_MAX_INTEGRAL__ = max(__INTEGRAL_XY__[1], N - 1);
}

// Calculate the __AREA__ under the curve of a function
void area_under_curve() {
    __AREA__ = 0;
    double last_valid_value = 0;
    for(int i=0; i<N-1; i++){
        if (isinf(__XY__[1][i]) || isnan(__XY__[1][i]) || isinf(__XY__[1][i+1]) || isnan(__XY__[1][i+1])) {
            continue;
        }
        __AREA__ = last_valid_value + (__XY__[1][i] + __XY__[1][i + 1]) * (__XY__[0][i + 1] - __XY__[0][i]) / 2;
        last_valid_value = __AREA__;
    }
}

// Calculate the bisection points of a function
void bisection_points() {
    int j=0;

    //populate bisection points with NAN
    for (int i = 0; i < MAX_ZEROS*2; i++) {
        __BISECTION__[0][i] = NAN;
        __BISECTION__[1][i] = NAN;
    }

    for (int i = 0; i < N - 1; i++) {
        double x1 = __XY__[0][i];
        double x2 = __XY__[0][i + 1];
        double y1 = __XY__[1][i];
        double y2 = __XY__[1][i + 1];
        if (y1 * y2 < 0) {
            __BISECTION__[0][j] = x1;
            __BISECTION__[1][j] = y1;
            __BISECTION__[0][j + 1] = x2;
            __BISECTION__[1][j + 1] = y2;
            j += 2;
        }
    }

    // printing all the values
    printf("+++Bisection points\nx     y\n");
    for (int i = 0; i < MAX_ZEROS * 2; i += 2) {
        printf("%f %f\n", __BISECTION__[0][i], __BISECTION__[1][i]);
        printf("%f %f\n", __BISECTION__[0][i + 1], __BISECTION__[1][i + 1]);
    }
}

// Calculate the root using the bisection method with given points
// returns NAN if the points do not have opposite signs or the root is not found
double bisection_method(double point1[1][2], double point2[1][2]) {
    double x1 = point1[0][0];
    double x2 = point2[0][0];
    double y1 = point1[0][1];
    double y2 = point2[0][1];
    bracket_adder();

    if (y1 * y2 >= 0) {
        printf("The points do not have opposite signs\n");
        return NAN;
    }

    double x_mid = (x1 + x2) / 2;
    val_replacer(x_mid);
    double y_mid = eval(__EXPR_VAL__);

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
        val_replacer(x_mid);
        y_mid = eval(__EXPR_VAL__);
    }

    printf("Root not found\n");
    return NAN;
}

// Calculate the zeros of a function
void zeros_of_function() {
    // Find bisection points
    bisection_points();

    // Clearing the Zeros
    __ZEROS__.top = -1;

    // Find zeros
    for (int i = 0; i < MAX_ZEROS * 2; i += 2) {
        if(!isnan(__BISECTION__[0][i]) && !isnan(__BISECTION__[1][i]) && !isnan(__BISECTION__[0][i + 1]) && !isnan(__BISECTION__[1][i + 1])){
            double point1[1][2] = {{__BISECTION__[0][i], __BISECTION__[1][i]}};
            double point2[1][2] = {{__BISECTION__[0][i + 1], __BISECTION__[1][i + 1]}};
            double zero = bisection_method(point1, point2);
            if (!isnan(zero)) {
                push(&__ZEROS__, zero);
            }
        }
    }
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
// (x_min, __X_MAX__) are the minimum and maximum values of x
void map_xy() {
    double x_range = __X_MAX__ - __X_MIN__;
    double y_range = __Y_MAX__ - __Y_MIN__;
    double x_scale = (__MP_X2__-__MP_X1__) / x_range;
    double y_scale = (__MP_Y2__-__MP_Y1__) / y_range;
    double x_offset = __MP_X1__ - x_scale * __X_MIN__;
    double y_offset = __MP_Y1__ - y_scale * __Y_MIN__;

    for (int i = 0; i < N; i++) {
        __MAPPED_XY__[0][i] = x_scale * __XY__[0][i] + x_offset;
        __MAPPED_XY__[1][i] = y_scale * __XY__[1][i] + y_offset;
    }
}

// Map dy/dx values to screen coordinates
void map_dx_dy() {
    double x_range = __X_MAX__ - __X_MIN__;
    double y_range = __Y_MAX_DY_DX__ - __Y_MIN_DY_DX__;
    double x_scale = (__MP_X2__-__MP_X1__) / x_range;
    double y_scale = (__MP_Y2__-__MP_Y1__) / y_range;
    double x_offset = __MP_X1__ - x_scale * __X_MIN__;
    double y_offset = __MP_Y1__ - y_scale * __Y_MIN_DY_DX__;

    for (int i = 0; i < N - 1; i++) {
        __MAPPED_DY_DX__[0][i] = x_scale * __DY_DX__[0][i] + x_offset;
        __MAPPED_DY_DX__[1][i] = y_scale * __DY_DX__[1][i] + y_offset;
    }
}

// Map integral y values to screen coordinates
void map_integral() {
    double x_range = __X_MAX__ - __X_MIN__;
    double y_range = __Y_MAX_INTEGRAL__ - __Y_MIN_INTEGRAL__;
    double x_scale = (__MP_X2__-__MP_X1__) / x_range;
    double y_scale = (__MP_Y2__-__MP_Y1__) / y_range;
    double x_offset = __MP_X1__ - x_scale * __X_MIN__;
    double y_offset = __MP_Y1__ - y_scale * __Y_MIN_INTEGRAL__;

    for (int i = 0; i < N - 1; i++) {
        __MAPPED_INTEGRAL_XY__[0][i] = x_scale * __INTEGRAL_XY__[0][i] + x_offset;
        __MAPPED_INTEGRAL_XY__[1][i] = y_scale * __INTEGRAL_XY__[1][i] + y_offset;
    }
}

// ---------------FUNCTIONS TO ZOOM AND SHIFT----------------

// zoom in the curve
void zoom_in() {
    __X_MIN__ /= __ZOOMING__;
    __X_MAX__ /= __ZOOMING__;
    __ZOOMING__ *= ZOOM_FACTOR;
    __SHIFTING__ *= SHIFT_FACTOR*__ZOOMING__;
    xy_vals();
    map_xy();
    derivative();
    map_dx_dy();
    integral();
    map_integral();
}

// zoom out the curve
void zoom_out() {
    __X_MIN__ *= __ZOOMING__;
    __X_MAX__ *= __ZOOMING__;
    __ZOOMING__ *= ZOOM_FACTOR;
    __SHIFTING__ *= SHIFT_FACTOR*__ZOOMING__;
    xy_vals();
    map_xy();
    derivative();
    map_dx_dy();
    integral();
    map_integral();
}

// shift the curve to the right
void shift_right() {
    __X_MIN__ += __SHIFTING__;
    __X_MAX__ += __SHIFTING__;
    xy_vals();
    map_xy();
    derivative();
    map_dx_dy();
    integral();
    map_integral();
}

// shift the curve to the left
void shift_left() {
    __X_MIN__ -= __SHIFTING__;
    __X_MAX__ -= __SHIFTING__;
    xy_vals();
    map_xy();
    derivative();
    map_dx_dy();
    integral();
    map_integral();
}

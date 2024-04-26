#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "evaluate.h"

// GLOBAL VARIABLES
char expr[EXP_LEN];                  // expression to be evaluated (x)
char expr_val[EXP_LEN];              // expression to be evaluated (number)
int bracket_flag;                    // flag to check if brackets are added
double xy[2][N];                     // x values
double mapped_xy[2][N];              // x values mapped to screen coordinates
double dy_dx[2][N-1];                // derivative values
double integral_xy[2][N-1];            // integral values
double area;                         // area under the curve
double bisection[MAX_ZEROS][2];      // bisection points
Stack zeros;                         // zeros of the function

// INITIALIZATION
void initialize() {
    strcpy(expr, "");
    bracket_flag = 0;
    init(&zeros);
    area = 0;
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
void bracket_adder() {
    if(!bracket_flag){

        char new_expr[strlen(expr) * 2 + 1]; // Maximum length of new expression is twice the length of the original expression
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
        strcpy(expr, new_expr);
        bracket_flag = 1;
    }
}

// Generate x values
void x_vals() {
    double step_size = ((double)X_MAX - (double)X_MIN) / ((double)N - 1);

    for (int i = 0; i < N; i++) {
        xy[0][i] = X_MIN + i * step_size;
    }
}

// Replace variable with value and replace pi and e
void val_replacer(double val) {
    char new_expr[EXP_LEN];
    int j = 0;

    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == 'x') {   // Replace x with value with (0val)
            new_expr[j++] = '(';
            new_expr[j++] = '0';
            char num[50];
            sprintf(num, "%f", val);
            for (int k = 0; num[k] != '\0'; k++) {
                new_expr[j++] = num[k];
            }
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
    strcpy(expr_val, new_expr);
}

// Generate y values corresponding to x values
void y_vals() {
    bracket_adder();
    for (int i = 0; i < N; i++) {
        val_replacer(xy[0][i]);
        xy[1][i] = eval(expr_val);
    }
}

void xy_vals() {
    x_vals();
    y_vals();
}

//---------------ADDITIONAL OPERATIONS ON X AND Y VALUES----------------

// Calculate the derivative of a function
void derivative() {
    for (int i = 0; i < N - 1; i++) {
        dy_dx[0][i] = (xy[0][i + 1] + xy[0][i]) / 2;
        dy_dx[1][i] = (xy[1][i + 1] - xy[1][i]) / (xy[0][i + 1] - xy[0][i]);
    }
    
}

// Calculate the integral of a function
void integral() {
    integral_xy[0][0] = xy[0][0];
    integral_xy[1][0] = 0;

    for (int i = 1; i < N-1; i++) {
        integral_xy[0][i] = xy[0][i];
        integral_xy[1][i] = integral_xy[1][i - 1] + (xy[1][i] + xy[1][i - 1]) * (xy[0][i] - xy[0][i - 1]) / 2;
    }
}

// Calculate the area under the curve of a function
void area_under_curve() {
    area = 0;
    for (int i = 1; i < N; i++) {
        area += (xy[1][i] + xy[1][i - 1]) * (xy[0][i] - xy[0][i - 1]) / 2;
    }
}

// Calculate the bisection points of a function
void bisection_points() {
    int j=0;

    //populate bisection points with NAN
    for (int i = 0; i < MAX_ZEROS * 2; i++) {
        bisection[0][i] = NAN;
        bisection[1][i] = NAN;
    }

    for (int i = 0; i < N - 1; i++) {
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
}

// Calculate the root using the bisection method with given points
// returns NAN if the points do not have opposite signs or the root is not found
double bisection_method(double point1[1][2], double point2[1][2]) {
    double x1 = point1[0][0];
    double x2 = point2[0][0];
    double y1 = point1[0][1];
    double y2 = point2[0][1];
    bracket_adder(expr);

    if (y1 * y2 >= 0) {
        printf("The points do not have opposite signs\n");
        return NAN;
    }

    double x_mid = (x1 + x2) / 2;
    val_replacer(x_mid);
    double y_mid = eval(expr_val);

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
        y_mid = eval(expr_val);
    }

    printf("Root not found\n");
    return NAN;
}

// Calculate the zeros of a function
void zeros_of_function() {
    // Find bisection points
    bisection_points();

    // Find zeros
    for (int i = 0; i < MAX_ZEROS * 2; i += 2) {
        if(!isnan(bisection[0][i])){
            double point1[1][2] = {{bisection[0][i], bisection[1][i]}};
            double point2[1][2] = {{bisection[0][i + 1], bisection[1][i + 1]}};
            double zero = bisection_method(point1, point2);
            if (!isnan(zero)) {
                push(&zeros, zero);
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
// (x_min, x_max) are the minimum and maximum values of x
void map_xy(int x1, int y1,  int x2, int y2) {
    double y_min = min(xy[1], N);
    double y_max = max(xy[1], N);
    double x_range = X_MAX - X_MIN;
    double y_range = y_max - y_min;
    double x_scale = (x2 - x1) / x_range;
    double y_scale = (y2 - y1) / y_range;
    double x_offset = x1 - x_scale * X_MIN;
    double y_offset = y1 - y_scale * y_min;

    for (int i = 0; i < N; i++) {
        mapped_xy[0][i] = x_scale * xy[0][i] + x_offset;
        mapped_xy[1][i] = y_scale * xy[1][i] + y_offset;
    }
}
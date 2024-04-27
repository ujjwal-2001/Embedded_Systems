// EVALUATE LIBRARY WITHOUT MALLOC

#ifndef EVALUATE_H
#define EVALUATE_H

#define MAX_STACK_SIZE 100      // maximum size of the mathematical expression in stack 
#define PI 3.14159265           // value of pi  
#define E 2.718281828459045     // value of e
#define EPSILON 0.000001        // value of epsilon - Property of Zeros of a function
#define MAX_ITERATIONS 70       // maximum number of iterations for bisection method - Property of Zeros of a function
#define MAX_ZEROS 10            // maximum number of zeros of a function - Property of Zeros of a function
#define N 800                   // maximum number of points
#define EXP_LEN 150             // maximum length of the expression

#include "evaluate.h"

// Stack definition
typedef struct {
    double items[MAX_STACK_SIZE];
    int top;
} Stack;

// GLOBAL VARIABLES
// USER CAN USE THIS: READ ONLY !!!!
// USER SHOULD CHANGE THIS: READ AND WRITE
// USER SHOULD NOT USE THIS: READ ONLY !!!!
extern char __EXPR__[EXP_LEN];                  // expression to be evaluated (x)       - USER SHOULD CHANGE THIS
extern char __EXPR_VAL__[EXP_LEN];              // expression to be evaluated (number)  - USER SHOULD NOT USE THIS
extern int __BRACKET_FLAG__;                    // flag to check if brackets are added  - USER SHOULD NOT USE THIS
extern double __X_MIN__;                        // minimum x value                      - USER SHOULD CHANGE THIS
extern double __X_MAX__;                        // maximum x value                      - USER SHOULD CHANGE THIS
extern double __XY__[2][N];                     // x values                             - USER SHOULD CAN USE THIS
extern double __MAPPED_XY__[2][N];              // x values mapped to screen coordinates- USER SHOULD USE THIS
extern double __DY_DX__[2][N-1];                // derivative values                    - USER SHOULD CAN USE THIS
extern double __INTEGRAL_XY__[2][N-1];          // integral values                      - USER SHOULD CAN USE THIS
extern double __AREA__;                         // __AREA__ under the curve             - USER SHOULD CAN USE THIS
extern double __BISECTION__[MAX_ZEROS][2];      // bisection points                     - USER SHOULD CAN USE THIS
extern Stack __ZEROS__;                         // zeros of the function                - USER SHOULD CAN USE THIS

// INITIALIZATION
void initialize();                                // Initialize global variables

// EVALUATION OF MATHEMATICAL EXPRESSIONS
void init(Stack* s);                                // Initialize stack
void push(Stack* s, double val);                    // Push item onto stack
double pop(Stack* s);                               // Pop item from stack
double peek(Stack* s);                              // Peek at the top item of the stack
int isempty(Stack* s);                              // Check if stack is empty
void print_stack(Stack* s);                         // Print stack
int precedence(char op);                            // Operator precedence
double applyOp(double a, double b, char op);        // Apply operator to operands
double eval(const char* expr);                      // Evaluate expression

// GENERATION OF X AND Y VALUES
void bracket_adder();               // Envolopes sin, cos, tan, log, and abs in brackets
void x_vals();                      // Generate x values
void val_replacer(double val);      // Replace variable with value
void y_vals();                      // Generate y values corresponding to x values
void xy_vals();                     // Generate x and y values

// ADDITIONAL OPERATIONS ON X AND Y VALUES
void derivative();                   // Calculate the derivative of a function
void integral();                     // Calculate the integral of a function - Trapezoidal Rule - Integration from __X_MIN__ to x
void area_under_curve();             // Calculate the __AREA__ under the curve of a function
void bisection_points();             // Calculate the bisection points of a function
double bisection_method(double point1[1][2], double point2[1][2]); // Calculate the root using the bisection method with given points  
void zeros_of_function();                // Calculate the zeros of a function

// MAPPING OF X AND Y VALUES TO SCREEN COORDINATES
double max(double* arr, int n); // Find maximum value in an array
double min(double* arr, int n); // Find minimum value in an array
void map_xy(int x1, int y1,  int x2, int y2); // Map x and y values to screen coordinates

#endif
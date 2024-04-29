// EVALUATE LIBRARY WITHOUT MALLOC

#ifndef EVALUATE_H
#define EVALUATE_H

#define MAX_STACK_SIZE 100              // maximum size of the mathematical expression in stack 
#define PI 3.14159265                   // value of pi  
#define E 2.718281828459045             // value of e
#define EPSILON 0.000001                // value of epsilon - Property of Zeros of a function
#define MAX_ITERATIONS 70               // maximum number of iterations for bisection method - Property of Zeros of a function
#define MAX_ZEROS 10                    // maximum number of zeros of a function - Property of Zeros of a function
#define N 800                           // maximum number of points
#define EXP_LEN 150                     // maximum length of the expression
#define ZOOM_FACTOR 2                   // zoom factor | should be more than one 
#define SHIFT_FACTOR 1                  // shift factor
#define mod(x) ((x) < 0 ? -(x) : (x))   // absolute value of x

#include "evaluate.h"

// Stack definition
typedef struct {
    float items[MAX_STACK_SIZE];
    int top;
} Stack;

// GLOBAL VARIABLES
// USER CAN USE THIS: READ ONLY !!!!
// USER SHOULD CHANGE THIS: WRITE
// USER SHOULD NOT USE THIS: READ ONLY !!!!
extern char __EXPR__[EXP_LEN];                  // expression to be evaluated (x)       - USER SHOULD CHANGE THIS
extern char __EXPR_VAL__[EXP_LEN];              // expression to be evaluated (number)  - USER SHOULD NOT USE THIS
extern int __BRACKET_FLAG__;                    // flag to check if brackets are added  - USER SHOULD NOT USE THIS
extern float __X_MIN__;                        // minimum x value                      - USER SHOULD CHANGE THIS
extern float __X_MAX__;                        // maximum x value                      - USER SHOULD CHANGE THIS
extern float __Y_MIN__;                        // minimum y value                      - USER SHOULD USE THIS
extern float __Y_MAX__;                        // maximum y value                      - USER SHOULD USE THIS
extern float __Y_MIN_DY_DX__;                  // minimum y value of dy/dx             - USER SHOULD USE THIS
extern float __Y_MAX_DY_DX__;                  // maximum y value of dy/dx             - USER SHOULD USE THIS
extern float __Y_MIN_INTEGRAL__;               // minimum y value of integral of y     - USER SHOULD USE THIS
extern float __Y_MAX_INTEGRAL__;               // maximum y value of integral of y     - USER SHOULD USE THIS
extern float __MP_X1__;                        // x1 value for mapping                 - USER SHOULD CHANGE THIS
extern float __MP_Y1__;                        // y1 value for mapping                 - USER SHOULD CHANGE THIS
extern float __MP_X2__;                        // x2 value for mapping                 - USER SHOULD CHANGE THIS
extern float __MP_Y2__;                        // y2 value for mapping                 - USER SHOULD CHANGE THIS
extern float __XY__[2][N];                     // x values                             - USER SHOULD CAN USE THIS
extern float __MAPPED_XY__[2][N];              // x values mapped to screen coordinates- USER SHOULD USE THIS
extern float __DY_DX__[2][N-1];                // derivative values                    - USER SHOULD CAN USE THIS
extern float __MAPPED_DY_DX__[2][N-1];         // mapped derivative values             - USER SHOULD CAN USE THIS
extern float __INTEGRAL_XY__[2][N-1];          // integral values                      - USER SHOULD CAN USE THIS
extern float __MAPPED_INTEGRAL_XY__[2][N-1];   // mapped integral values               - USER SHOULD CAN USE THIS
extern float __AREA__;                         // __AREA__ under the curve             - USER SHOULD CAN USE THIS
extern float __BISECTION__[2][MAX_ZEROS*2];    // bisection points                     - USER SHOULD CAN USE THIS
extern Stack __ZEROS__;                         // zeros of the function                - USER SHOULD CAN USE THIS
extern Stack __MAPPED_ZEROS__;                  // mapped zeros of the function         - USER SHOULD CAN USE THIS

// INITIALIZATION
void initialize();                                // Initialize global variables

// EVALUATION OF MATHEMATICAL EXPRESSIONS
void init(Stack* s);                                // Initialize stack
void push(Stack* s, float val);                    // Push item onto stack
float pop(Stack* s);                               // Pop item from stack
float peek(Stack* s);                              // Peek at the top item of the stack
int isempty(Stack* s);                              // Check if stack is empty
void print_stack(Stack* s);                         // Print stack
int precedence(char op);                            // Operator precedence
float applyOp(float a, float b, char op);        // Apply operator to operands
float eval(const char* expr);                      // Evaluate expression

// GENERATION OF X AND Y VALUES
void bracket_adder();               // Envolopes sin, cos, tan, log, and abs in brackets
void x_vals();                      // Generate x values
void val_replacer(float val);      // Replace variable with value
void y_vals();                      // Generate y values corresponding to x values
void xy_vals();                     // Generate x and y values

// ADDITIONAL OPERATIONS ON X AND Y VALUES
void derivative();                   // Calculate the derivative of a function
void integral();                     // Calculate the integral of a function - Trapezoidal Rule - Integration from __X_MIN__ to x
void area_under_curve();             // Calculate the __AREA__ under the curve of a function
void bisection_points();             // Calculate the bisection points of a function
float bisection_method(float point1[1][2], float point2[1][2]); // Calculate the root using the bisection method with given points  
void zeros_of_function();                // Calculate the zeros of a function

// MAPPING OF X AND Y VALUES TO SCREEN COORDINATES
float max(float* arr, int n);     // Find maximum value in an array
float min(float* arr, int n);     // Find minimum value in an array
void map_xy();                      // Map x and y values to screen coordinates
void map_dx_dy();                   // Map dy/dx values to screen coordinates
void map_integral();                // Map integral y values to screen coordinates
void map_zeros();                   // Map zeros of the function to screen coordinates

// FUNCTION TO ZOOM AND SHIFT THE GRAPH
void zoom_in();                     // Zoom in the graph
void zoom_out();                    // Zoom out the graph
void shift_left();                  // Shift the graph to the left
void shift_right();                 // Shift the graph to the right

#endif
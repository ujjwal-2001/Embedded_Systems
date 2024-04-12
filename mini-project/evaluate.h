#ifndef EVALUATE_H
#define EVALUATE_H

#define MAX_STACK_SIZE 100      // maximum size of the mathematical expression
#define PI 3.14159265           // value of pi  
#define E 2.718281828459045     // value of e

#include "evaluate.h"

// Stack definition
typedef struct {
    double items[MAX_STACK_SIZE];
    int top;
} Stack;


// EVALUATION OF MATHEMATICAL EXPRESSIONS
void init(Stack* s);                                // Initialize stack
void push(Stack* s, double val);                    // Push item onto stack
double pop(Stack* s);                               // Pop item from stack
double peek(Stack* s);                              // Peek at the top item of the stack
int isempty(Stack* s);                              // Check if stack is empty
int precedence(char op);                            // Operator precedence
double applyOp(double a, double b, char op);        // Apply operator to operands
double eval(const char* expr);                      // Evaluate expression

// GENERATION OF X AND Y VALUES
char* bracket_adder(char* expr);                    // Envolopes sin, cos, tan, log, and abs in brackets
double* x_vals(int n, int min, int max);               // Generate x values
char* val_replacer(char* expr, double val);         // Replace variable with value
double* y_vals(int n, double* x, char* expr);             // Generate y values corresponding to x values
double ** xy_vals(int n, int min, int max, char* expr); // Generate x and y values

#endif
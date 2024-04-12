#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "evaluate.h"

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
    return s->items[(s->top)--];
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
                double a = pop(&valStack);
                char op = pop(&opStack);
                if (op == 's') {
                    push(&valStack, sin(b));
                } else if (op == 'c') {
                    push(&valStack, cos(b));
                } else if (op == 't') {
                    push(&valStack, tan(b));
                } else if (op == 'l') {
                    push(&valStack, log(b));
                } else {
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

// Envolopes sin, cos, tan, log, and abs in brackets
// Example input: "2*sin(3.14)+log(100)"
// Example output: "2*(sin(3.14))+(log(100))"
char* bracket_adder(char* expr) {
    char* new_expr = malloc(strlen(expr) * 2 + 1);
    int j = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == 's' || expr[i] == 'c' || expr[i] == 't' || expr[i] == 'l' || expr[i] == 'a') {
            new_expr[j++] = '(';
            while(expr[i] != ')') {
                new_expr[j++] = expr[i++];
            }
            new_expr[j++] = expr[i];
            new_expr[j++] = ')';

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
    char* new_expr = malloc(strlen(expr) * 2 + 1);
    int j = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == 'x') {
            char* num = malloc(20);
            sprintf(num, "%f", val);
            for (int k = 0; num[k] != '\0'; k++) {
                new_expr[j++] = num[k];
            }
            free(num);
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
    printf("%s\n", corrected_expr);
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
#include <stdio.h>

// Macros
#define RUNNING 0;
#define READY 1;
#define WAITING 2;

// Defining Node structure
struct Node {
    int task_id;
    int task_id;
    int* ptr_context;
    int task_state;
    int event_id;
    struct Node* next;
};

// Defining Queue structure

struct Queue {
    struct Node* head;
    struct Node* tail;
};

// Decleration of functions 
void enqueue(struct Queue* queue, struct Node* node);
struct Node* dequeue(struct Queue* queue);
void print_queue(struct Queue* queue);
void print_node(struct Node* node);
int is_empty(struct Queue* queue);
int is_unique_task_id(struct Queue* queue, int task_id);
void event_trigger(struct Queue* queue, int event_id);
void suspend_event(struct Queue* queue, int event_id);
void read_initial_state(struct Queue* queue, char* file_name);
void move_ready_to_waiting(struct Queue* queue);
void free_up_memory(struct Queue* queue);


int main(){

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

// MACROS
#define RUNNING 0;
#define READY 1;
#define WAITING 2;

// NODE STRUCTURE
struct Node {
    int task_id;
    int task_id;
    int* ptr_context;
    int task_state;
    int event_id;
    struct Node* next;
}running_node;

// QUEUE STRUCTURE

struct Queue {
    struct Node* head;
    struct Node* tail;
};

// DECLERATION OF FUNCTIONS 
struct Queue* create_queue();
struct Node* create_node(int task_id, int task_state, int event_id);
int is_empty(struct Queue* queue);
void enqueue(struct Queue* queue, struct Node* node);
struct Node* dequeue(struct Queue* queue);
struct Node* dequeue_any(struct Queue* queue, int task_id);
void print_queue(struct Queue* queue);
void print_node(struct Node* node);
int is_unique_task_id(struct Queue* queue, int task_id);
void event_trigger(struct Queue* waiting_queue,struct Queue* ready_queue, int event_id);
void suspend_event(struct Queue* queue, int event_id);
void read_initial_state(struct Queue* queue, char* file_name);
void move_ready_to_waiting(struct Queue* ready_queue,struct Queue* waiting_queue, int task_id);
void free_up_memory(struct Queue* queue);
void sort_queue(struct Queue* queue);

int main(){

    return 0;
}

// DEFFINATION OF FUNCTIONS

// This function will create a empty queue and return it
struct Queue* create_queue(){
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

// This function will create a node for queue and return it
struct Node* create_node(int task_id, int task_state, int event_id){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->task_id = task_id;
    node->ptr_context = NULL;
    node->task_state = task_state;
    node->event_id = event_id;
    node->next = NULL;
    return node;
}

// This function will check if the queue is empty or not
int is_empty(struct Queue* queue){
    if(queue->head == NULL){
        return 1;
    }
    else{
        return 0;
    }
}

// This function will add a node to the queue
void enqueue(struct Queue* queue, struct Node* node){
    if(is_empty(queue)){
        queue->head = node;
        queue->tail = node;
    }
    else{
        queue->tail->next = node;
        queue->tail = node;
    }
}

// This function will remove a node from the queue and return it
struct Node* dequeue_any(struct Queue* queue, int task_id){
    if(is_empty(queue)){
        return NULL;
    }
    else{
        struct Node* temp = queue->head;
        struct Node* prev = NULL;
        while(temp != NULL){
            if(temp->task_id == task_id){
                if(prev == NULL){
                    queue->head = queue->head->next;
                    return temp;
                }
                else{
                    prev->next = temp->next;
                    return temp;
                }
            }
            prev = temp;
            temp = temp->next;
        }
        return NULL;
    }
}

// This function will remove a node from the queue and return it
struct Node* dequeue(struct Queue* queue){
    if(is_empty(queue)){
        return NULL;
    }
    else{
        struct Node* temp = queue->head;
        queue->head = queue->head->next;
        return temp;
    }
}

// This function will print a node
void print_node(struct Node* node){
    printf("Task ID: %d\n", node->task_id);
    printf("Task State: %d\n", node->task_state);
    printf("Event ID: %d\n", node->event_id);
    printf("Context Pointer: %p\n", node->ptr_context);
    printf("\n");
}

// This function will print the queue
void print_queue(struct Queue* queue){
    if(is_empty(queue)){
        printf("Queue is empty\n");
    }
    else{
        struct Node* temp = queue->head;
        while(temp != NULL){
            print_node(temp);
            temp = temp->next;
        }
    }
}

// This function will check if the task id is unique or not
int is_unique_task_id(struct Queue* queue, int task_id){
    if(is_empty(queue)){
        return 1;
    }
    else{
        struct Node* temp = queue->head;
        while(temp != NULL){
            if(temp->task_id == task_id){
                return 0;
            }
            temp = temp->next;
        }
        return 1;
    }
}

// This function will trigger the event
void event_trigger(struct Queue* waiting_queue, struct Queue* ready_queue, int event_id){
    if(is_empty(waiting_queue)){
        printf("Waiting queue is empty.\n");
    }
    else{
        struct Node* temp = waiting_queue->head;
        while(temp != NULL){
            if(temp->event_id == event_id){
                temp->task_state = READY;
            }
            temp = temp->next;
        }
    }
}

// This function will suspend the event 
void suspend_event(struct Queue* queue, int event_id){
    if(is_empty(queue)){
        printf("Queue is empty.\n");
    }
    else{
        struct Node* temp = queue->head;
        while(temp != NULL){
            if(temp->event_id == event_id){
                temp->task_state = WAITING;
            }
            temp = temp->next;
        }
    }
}

// This function will read the initial state of the tasks from the file


// This function will move the ready task to waiting queue
void move_ready_to_waiting(struct Queue* ready_queue, struct Queue* waiting_queue, int task_id){
    struct Node* node = dequeue_any(ready_queue, task_id);
    if(node == NULL){
        printf("Task is not in ready queue.\n");
    }
    else{
        node->task_state = WAITING;
        enqueue(waiting_queue, node);
    }
}

// This function will free up the memory
void free_up_memory(struct Queue* queue){
    if(is_empty(queue)){
        printf("Queue is empty.\n");
    }
    else{
        struct Node* temp = queue->head;
        while(temp != NULL){
            struct Node* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
}

// This function will sort the queue according to the task id
void sort_queue(struct Queue* queue){
    if(is_empty(queue)){
        printf("Queue is empty.\n");
    }
    else{
        struct Node* temp = queue->head;
        while(temp != NULL){
            struct Node* temp2 = temp->next;
            while(temp2 != NULL){
                if(temp->task_id > temp2->task_id){
                    int task_id = temp->task_id;
                    int task_state = temp->task_state;
                    int event_id = temp->event_id;
                    temp->task_id = temp2->task_id;
                    temp->task_state = temp2->task_state;
                    temp->event_id = temp2->event_id;
                    temp2->task_id = task_id;
                    temp2->task_state = task_state;
                    temp2->event_id = event_id;
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }
}
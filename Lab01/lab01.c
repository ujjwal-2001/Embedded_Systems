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

// queue functions

struct Queue* create_queue();
struct Node* create_node(int task_id, int task_state, int event_id);
int is_empty(struct Queue* queue);
void enqueue(struct Queue* queue, struct Node* node);
void enqueue_sorted(struct Queue* queue, struct Node* node);
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

// declaration of functions which will implement the commands as per the requirement
// Console:
// n task_id– Add/create new task
// d task_id– Delete task from the Ready/Waiting queue
// w task_id event_id – Move task from Ready to Waiting queue
// e event_id – Trigger the event of event_id
// s event_id – Suspend running task with event_id
int validate_command(char* command);
void run_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);

// MAIN FUNCTION
int main(){

    // INITIAL SETUP

    // Creating the queues
    struct Queue* ready_queue = create_queue();
    struct Queue* waiting_queue = create_queue();

    // Creating pointer that points to the running node
    struct Node* running_node = NULL;

    // Reading the initial state of the tasks from the file
    

    // MAIN LOOP

    while(1){

        // Printing commands for the user
        printf("==================================================================\n");
        printf("Commands available\n");
        printf("n task_id          – Add/create new task\n");
        printf("d task_id          – Delete task from the Ready/Waiting queue\n");
        printf("w task_id event_id – Move task from Ready to Waiting queue\n");
        printf("e event_id         – Trigger the event of event_id\n");
        printf("s event_id         – Suspend running task with event_id\n");
        printf("q                  – Quit the program\n");
        printf("==================================================================\n");
        
        // Taking command from the user as string
        char command[30];
        printf("Enter the command: ");
        scanf("%s", command);

        // Checking the command and performing the required operation
        
        
    }
    

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

// This function will add a node to the queue in sorted order (descending order)
void enqueue_sorted(struct Queue* queue, struct Node* node){
    if(is_empty(queue)){
        queue->head = node;
        queue->tail = node;
    }
    else{
        struct Node* temp = queue->head;
        struct Node* prev = NULL;
        while(temp != NULL){
            if(temp->task_id < node->task_id){
                if(prev == NULL){
                    node->next = queue->head;
                    queue->head = node;
                    return;
                }
                else{
                    prev->next = node;
                    node->next = temp;
                    return;
                }
            }
            prev = temp;
            temp = temp->next;
        }
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

// This function will sort the queue according to the task id (descending order)
void sort_queue(struct Queue* queue){
    if(is_empty(queue)){
        printf("Queue is empty.\n");
    }
    else{
        struct Node* temp = queue->head;
        while(temp != NULL){
            struct Node* temp2 = temp->next;
            while(temp2 != NULL){
                if(temp->task_id < temp2->task_id){
                    int temp_task_id = temp->task_id;
                    int temp_task_state = temp->task_state;
                    int temp_event_id = temp->event_id;
                    int* temp_ptr_context = temp->ptr_context;
                    temp->task_id = temp2->task_id;
                    temp->task_state = temp2->task_state;
                    temp->event_id = temp2->event_id;
                    temp->ptr_context = temp2->ptr_context;
                    temp2->task_id = temp_task_id;
                    temp2->task_state = temp_task_state;
                    temp2->event_id = temp_event_id;
                    temp2->ptr_context = temp_ptr_context;
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }
}

// This function will validate the command given by the user
int validate_command(char* command){

    if (command[1] != ' ')  // check if there is a space after the command
    {
        printf("Invalid command.\n");
        return 0;
    }
    

    if(command[0] == 'n' || command[0] == 'd'){
        if(command[2] == '\0'){
            printf("Invalid command.\n");
            return 0;
        }
        else{
            int i = 2;
            while(command[i] != '\0'){
                if(command[i] < '0' || command[i] > '9'){
                    printf("Invalid command: Task ID must be a positive integer.\n");
                    return 0;
                }
                i++;
            }
        }
    }
    else if(command[0] == 'w'){
        if(command[2] == '\0'){
            printf("Invalid command.\n");
            return 0;
        }
        else{
            int i = 1;
            int space_count = 0; // count occurance of white space 
            while(command[i] != '\0'){
                if(command[i] < '0' || command[i] > '9'){
                    printf("Invalid command: Task ID and/or Event ID must be a positive integer.\n");
                    return 0;
                }else if(command[i] == ' '){
                    space_count++;
                    if (space_count > 1)
                    {
                        printf("Invalid command.\n");
                        return 0;
                    }
                }
                i++;
            }
        }
    }
    else if(command[0] == 'e' || command[0] == 's'){
        if(command[2] == '\0'){
            printf("Invalid command.\n");
            return 0;
        }
        else{
            int i = 2;
            while(command[i] != '\0'){
                if(command[i] < '0' || command[i] > '9'){
                    printf("Event ID must be a positive integer.\n");
                    return 0;
                }
                i++;
            }
        }
    }
    else if(command[0] == 'q'){
        if(command[1] != '\0'){
            printf("Invalid command.\n");
            return 0;
        }
    }
    else{
        printf("Invalid command.\n");
        return 0;
    }

    return 1;
}
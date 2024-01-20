#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

// DEFFINATION OF FUNCTIONS

// This function will create a empty queue and return it
struct Queue* create_queue(){
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

// This function will create a node for queue and return it
struct Node* create_node(int task_id,int task_pri, int task_state, int event_id){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->task_id = task_id;
    node->task_pri = task_pri;
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
        node->next = NULL;
    }
}

// This function will add a node to the queue in sorted order according 
// to task priority (ascending order)
void enqueue_sorted(struct Queue* queue, struct Node* node){
    if(is_empty(queue)){
        queue->head = node;
        queue->tail = node;
    }
    else{
        struct Node* temp = queue->head;
        struct Node* prev = NULL;
        while(temp != NULL){
            if(temp->task_pri > node->task_pri){
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
    char* task_state = get_task_state(node->task_state);
    printf("Task ID: %d\t", node->task_id);
    printf("Priority: %d\t", node->task_pri);
    printf("State: %s\t", task_state);
    printf("Event ID: %d\t", node->event_id);
    // printf("Context Pointer: %p\n", node->ptr_context);
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

// This function will return the task state in string format
char* get_task_state(int task_state){
    switch (task_state)
    {
    case RUNNING:
        return "RUNNING";
        break;
    case READY:
        return "READY";
        break;
    case WAITING:
        return "WAITING";
        break;
    default:
        return "INVALID";
        break;
    }
}

// This function will print the system tasks
void print_system_tasks(struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    printf("Ready Queue:\n");
    print_queue(ready_queue);
    printf("*******************************************************************\n");
    printf("Waiting Queue:\n");
    print_queue(waiting_queue);
    printf("*******************************************************************\n");
    printf("Running Task:\n");
    if(running_node != NULL) {
        print_node(running_node);
    }else{
        printf("No task is running.\n");
    }
    printf("*******************************************************************\n");
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
   
    // if the event is triggered then the task will be moved to ready queue
    struct Node* temp = waiting_queue->head;
    struct Node* prev = NULL;
    while(temp != NULL){
        if(temp->event_id == event_id){
            temp->task_state = READY;
            if(prev == NULL){
                waiting_queue->head = waiting_queue->head->next;
                enqueue_sorted(ready_queue, temp);
                temp = waiting_queue->head;
            }
            else{
                prev->next = temp->next;
                enqueue_sorted(ready_queue, temp);
                temp = prev->next;
            }
        }
        else{
            prev = temp;
            temp = temp->next;
        }
    }
    printf("Event %d is triggered.\n", event_id);
}

// This function will suspend the event 
void suspend_event(struct Node* running_node, struct Queue* ready_queue, struct Queue* waiting_queue, int event_id){
    // if given task id is running . shift it to waiting queue
    if(running_node->event_id == event_id){
        running_node->task_state = WAITING;
        enqueue(waiting_queue, running_node);
        printf("Task ( ID: %d ) is suspended.\n", running_node->task_id);
    }
    else{
        printf("ERROR: Entered event ID is not of running task. Task can not be suspended.\n");
    }
}

// This function will read the initial state of the tasks from the file
void read_initial_state(struct Queue* waiting_queue, struct Queue* ready_queue, char* file_name){
    FILE* file = fopen(file_name, "r");
    if(file == NULL){
        printf("ERROR: File not found.\n");
    }
    else{
        char line[100];
        while(fgets(line, 100, file) != NULL){
            int task_id, task_pri, task_state, event_id;
            if(sscanf(line, "%d,%d,%d,%d", &task_id, &task_pri, &task_state, &event_id) == 4){
                struct Node* node = create_node(task_id, task_pri, task_state, event_id);
                if(task_state == READY){
                    enqueue_sorted(ready_queue, node);
                }
                else if(task_state == WAITING){
                    enqueue(waiting_queue, node);
                } 
                else if(task_state == RUNNING){
                    printf("ERROR: Can only have waiting or ready task in the %s\n",file_name);
                    free_up_memory(ready_queue);
                    free_up_memory(waiting_queue);
                }else{
                    printf("ERROR: Invalid task state in %s\n",file_name);
                    free_up_memory(ready_queue);
                    free_up_memory(waiting_queue);
                }
            }
            else{
                printf("ERROR: Invalid file format.\n");
                exit(0);
            }
        }
    }
}

// This function will move a task from ready to waiting queue
void move_ready_to_waiting(struct Queue* ready_queue, struct Queue* waiting_queue, int task_id, int event_id){
    struct Node* node = dequeue_any(ready_queue, task_id);
    if(node == NULL){
        printf("WARNING: Task is not in ready queue.\n");
    }
    else{
        node->task_state = WAITING;
        node->event_id = event_id;
        node->next = NULL;
        enqueue(waiting_queue, node);
        printf("Following task was moved from ready queue to waiting queue.\n");
        print_node(node);
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

// This function will update the running node
struct Node* update_running_node(struct Node* running_node, struct Queue* ready_queue){
    if(running_node != NULL){
        if(running_node->task_state == WAITING){
            if(!is_empty(ready_queue)){
                struct Node* temp = dequeue(ready_queue);
                temp->task_state = RUNNING;
                temp->next = NULL;
                return temp;
            }
            else{
                return NULL;
            }
        }else if(!is_empty(ready_queue)){
            if(running_node->task_pri > ready_queue->head->task_pri){
                struct Node* temp = dequeue(ready_queue);
                temp->task_state = RUNNING;
                temp->next = NULL;
                running_node->task_state = READY;
                enqueue_sorted(ready_queue, running_node);
                return temp;
            }else{
                return running_node;
            }
        }else{
            return running_node;
        }
    }else if(!is_empty(ready_queue)){
        struct Node* temp = dequeue(ready_queue);
        temp->task_state = RUNNING;
        temp->next = NULL;
        return temp;
    }
    else{
        return NULL;
    }
}

// This function will print the commands
void print_commands(){
    printf("==================================================================\n");
    printf("Commands available are :-\n");
    printf("l                  - Lists all available commands\n");
    printf("p                  - Prints ready and waiting queue with running task info\n");
    printf("n task_id task_pri - Adds/creates a new task with given ID and priority\n");
    printf("d task_id          - Delete task from the Ready/Waiting queue\n");
    printf("w task_id event_id - Move task from Ready to Waiting queue\n");
    printf("e event_id         - Trigger the event of event_id\n");
    printf("s event_id         - Suspend running task with given event_id\n");
    printf("q                  - Quits the program\n");
    printf("==================================================================\n");
}

// This function will validate command and will run it as given by the user
void run_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){

    switch (command[0])
    {
    case 'l': l_command(command);
        break;
    case 'p': p_command(command, ready_queue, waiting_queue, running_node);
        break;
    case 'n': n_command(command, ready_queue, waiting_queue, running_node);
        break;
    case 'd':  d_command(command, ready_queue, waiting_queue, running_node);
        break; 
    case 'w':w_command(command, ready_queue, waiting_queue, running_node);  
        break;
    case 'e': e_command(command, ready_queue, waiting_queue, running_node);
        break;
    case 's': s_command(command, ready_queue, waiting_queue, running_node);
        break;
    case 'q': q_command(command, ready_queue, waiting_queue, running_node);
        break;
    default:  
        printf("ERROR: Invalid command.\n");
        break; 
    }
}

// runs l command
void l_command(char* command){
    int i=1;
    while(command[i] != '\n'){
        if(command[i] != ' '){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    print_commands();
    
}

// runs p command
void p_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    int i=1;
    while(command[i] != '\n'){
        if(command[i] != ' '){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }    
    
    print_system_tasks(ready_queue, waiting_queue, running_node);

}

// runs n command
void n_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){    
    
    int i=2, space_count = 0;
    if(command[1] != ' '){
        printf("ERROR: Invalid command.\n");
        return;
    }

    while(command[i] != '\n'){
        if(command[i] == ' '){
            space_count++;
        }else if(command[i] < '0' || command[i] > '9' || space_count >= 2){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    int task_id, task_pri;
    if (sscanf(command, "n %d %d", &task_id, &task_pri) == 2)
    {   
        if (is_unique_task_id(ready_queue, task_id) && is_unique_task_id(waiting_queue, task_id) && running_node->task_id != task_id)
        {
            struct Node* node = create_node(task_id, task_pri, READY, DEFAULT_EVENT_ID);

            enqueue_sorted(ready_queue, node);
            printf("New task is created:-\n ");
            print_node(node);
        }
        else
        {   
            printf("Task ID must be unique.\n");
        }
    }
    else
    {
        printf("ERROR: Invalid command.\n");
    }
}

// runs e command
void e_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    
    int i=2, space_count = 0;
    if(command[1] != ' '){
        printf("ERROR: Invalid command.\n");
        return;
    }

    while(command[i] != '\n'){
        if(command[i] == ' '){
            space_count++;
        }else if(command[i] < '0' || command[i] > '9' || space_count >= 1){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    int event_id;
    if (sscanf(command, "e %d", &event_id) == 1)
    {
        event_trigger(waiting_queue, ready_queue, event_id);
    }
    else
    {
        printf("ERROR: Invalid command.\n");
    }
}


// runs d command
void d_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    
    int i=2, space_count = 0;
    if(command[1] != ' '){
        printf("ERROR: Invalid command.\n");
        return;
    }

    while(command[i] != '\n'){
        if(command[i] == ' '){
            space_count++;
        }else if(command[i] < '0' || command[i] > '9' || space_count >= 1){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    int task_id;
    if (sscanf(command, "d %d", &task_id) == 1)
    {
        struct Node* node = dequeue_any(ready_queue, task_id);
        if (node == NULL)
        {
            node = dequeue_any(waiting_queue, task_id);
            if (node == NULL)
            {
                if (running_node->task_id == task_id)
                {
                    printf("Entered task is running. Can not be delete.\n");
                }
                else
                {
                    printf("Task is not in any queue.\n");
                }
            }
            else
            {
                char* task_state = get_task_state(node->task_state);
                printf("Following task was deleted from %s queue.\n",task_state);
                print_node(node);
                free(node);
            }
        }
        else
        {   
            char* task_state = get_task_state(node->task_state);
            printf("Following task was deleted from %s queue.\n",task_state);
            print_node(node);
            free(node);
        }
    }
    else
    {
        printf("ERROR: Invalid command.\n");
    } 
}

// runs w command
void w_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    
    int i=2, space_count = 0;
    if(command[1] != ' '){
        printf("ERROR: Invalid command.\n");
        return;
    }

    while(command[i] != '\n'){
        if(command[i] == ' '){
            space_count++;
        }else if(command[i] < '0' || command[i] > '9' || space_count >= 2){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    int task_id, event_id;
    if (sscanf(command, "w %d %d", &task_id, &event_id) == 2)
    {
        move_ready_to_waiting(ready_queue, waiting_queue, task_id, event_id);
    }
    else
    {
        printf("ERROR: Invalid command.\n");
    } 
}

// runs q command
void q_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    
    int i=1;
    while(command[i] != '\n'){
        if(command[i] != ' '){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    printf("Quitting the program.\n");
    free_up_memory(ready_queue);
    free_up_memory(waiting_queue);
    free(running_node);
    exit(0);

}

// runs s command
void s_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node){
    
    int i=2, space_count = 0;
    if(command[1] != ' '){
        printf("ERROR: Invalid command.\n");
        return;
    }

    while(command[i] != '\n'){
        if(command[i] == ' '){
            space_count++;
        }else if(command[i] < '0' || command[i] > '9' || space_count >= 1){
            printf("ERROR: Invalid command.\n");
            return;
        }
        i++;
    }
    
    int event_id;
    if (sscanf(command, "s %d", &event_id) == 1)
    {
        suspend_event(running_node, ready_queue, waiting_queue, event_id);
    }
    else
    {
        printf("ERROR: Invalid command.\n");
    }
}
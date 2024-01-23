#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"


// MAIN FUNCTION
int main(){

    // INITIAL SETUP

    // Creating the queues
    struct Queue* ready_queue = create_queue();
    struct Queue* waiting_queue = create_queue();

    // Creating pointer that points to the running node
    struct Node* running_node = (struct Node*)malloc(sizeof(struct Node));

    // Reading the initial state of the tasks from the file
    read_initial_state(waiting_queue, ready_queue, "init_tasks.txt");
    running_node = dequeue(ready_queue);  
    running_node->task_state = RUNNING;
    running_node->next = NULL; 

    // Printing initial state
    printf("+++++++++++++++++++++ INITIAL STATE OF QUEUES +++++++++++++++++++++\n");
    print_system_tasks(ready_queue, waiting_queue, running_node);

    // Printing commands for the user
    print_commands();

    // MAIN LOOP
    
    while(TRUE){
        // Taking command from the user as string
        char command[30];
        printf("Enter the command: ");
        fgets(command, 30, stdin);

        // Checking the command and performing the required operation
        run_command(command, ready_queue, waiting_queue, running_node);

        // DEBUGGING before update

        printf("+++++++++++++++++++++ BEFORE UPDATE +++++++++++++++++++++\n");
        print_system_tasks(ready_queue, waiting_queue, running_node);
        // printing heads and tails
        if(! is_empty(ready_queue))
        {
            printf("ready_queue head: %d\n", ready_queue->head->task_id);
            printf("ready_queue tail: %d\n", ready_queue->tail->task_id);
        }

        if(! is_empty(waiting_queue))
        {
            printf("waiting_queue head: %d\n", waiting_queue->head->task_id);
            printf("waiting_queue tail: %d\n", waiting_queue->tail->task_id);
        }
        if(running_node!=NULL) 
            printf("running_node: %d\n", running_node->task_id);

        running_node = update_running_node(running_node, ready_queue);

        // DEBUGGING after update
        printf("+++++++++++++++++++++ AFTER UPDATE +++++++++++++++++++++\n");
        print_system_tasks(ready_queue, waiting_queue, running_node);
        // printing heads and tails
        if(! is_empty(ready_queue))
        {
            printf("ready_queue head: %d\n", ready_queue->head->task_id);
            printf("ready_queue tail: %d\n", ready_queue->tail->task_id);
        }

        if(! is_empty(waiting_queue))
        {
            printf("waiting_queue head: %d\n", waiting_queue->head->task_id);
            printf("waiting_queue tail: %d\n", waiting_queue->tail->task_id);
        }
        if(running_node!=NULL) 
            printf("running_node: %d\n", running_node->task_id);

        printf("\n\n\n");
    }

    return 0;
}
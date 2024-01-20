#ifndef SCHEDULER_H
#define SCHEDULER_H

// MACROS
#define RUNNING 0
#define READY 1
#define WAITING 2
#define TRUE 1
#define DEFAULT_EVENT_ID 0

// NODE STRUCTURE
struct Node {
    int task_id;
    int task_pri;
    int* ptr_context;
    int task_state;
    int event_id;
    struct Node* next;
};

// QUEUE STRUCTURE

struct Queue {
    struct Node* head;
    struct Node* tail;
};

// DECLERATION OF FUNCTIONS 

// queue functions
struct Queue* create_queue();
struct Node* create_node(int task_id, int task_pri, int task_state, int event_id);
int is_empty(struct Queue* queue);
void enqueue(struct Queue* queue, struct Node* node);
void enqueue_sorted(struct Queue* queue, struct Node* node);
struct Node* dequeue(struct Queue* queue);
struct Node* dequeue_any(struct Queue* queue, int task_id);
void print_queue(struct Queue* queue);
void print_node(struct Node* node);
char* get_task_state(int task_state);
void print_system_tasks(struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
int is_unique_task_id(struct Queue* queue, int task_id);
void event_trigger(struct Queue* waiting_queue,struct Queue* ready_queue, int event_id);
void suspend_event(struct Node* running_node, struct Queue* ready_queue, struct Queue* waiting_queue, int event_id);
void read_initial_state(struct Queue* waiting_queue, struct Queue* ready_queue, char* file_name);
void move_ready_to_waiting(struct Queue* ready_queue,struct Queue* waiting_queue, int task_id, int event_id);
struct Node* update_running_node(struct Node* running_node, struct Queue* ready_queue);
void free_up_memory(struct Queue* queue);

// command functions
void print_commands();
void run_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
void l_command(char* command);
void p_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
void n_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
void e_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
void d_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
void w_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue);
void q_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);
void s_command(char* command, struct Queue* ready_queue, struct Queue* waiting_queue, struct Node* running_node);


#endif
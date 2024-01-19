# Assignment 01
This assignment contaions a C program implementation of tassk scheduling for the partial grading of the course E3-257.

## Brief
This is a program illustrating scheduling of tasks in a CPU. A given task can be in one of the three states namely running, waiting or ready state. For keeping track of tasks in different states, this program uses queues. Each task is a structure that have following members:-

1. ```task_id```: This is an ```integer``` which is unique for each task. It acts as the ID of that task.
2. ```task_pri```: This is an ```integer``` that informs about the priority of the task (lower the value higher the priority)
3. ```ptr_context```: For this program this pointer is pointer to ```NULL```.
4. ```task_state``` : This is an ```integer``` that is mapped to the task state as per ```macros``` defined. In this program running, ready and waiting is represented by 0, 1 and 2 respectively.
5. ```event_id```: This is an ```integer``` that represents the event or the resource that task is waiting for to be triggred or released for it to jump to ready state.
6. ```next```: This is a pointer to the next Node/task.

Any task that is in the CPU will either be found in any one of the following:-

1. ```ready_queue```: Queue of ready tasks (sorted according to their priorities)
2. ```waiting_queue```: Queue of waiting tasks.
3. ```running_node```: Pointer that points to the node which is running.

### More about task states
![Alt text](image.png)

## Program features
Following are the features of the program:-
1. __Initialization__: The ```init_task.txt``` file act as the starting point of the program. All the queues are given the initial values from this file.
2. __Commands__: User can give the following commands using ```console```
| Command            | Description                                            |
|--------------------|--------------------------------------------------------|
| `l`                | Lists all available commands                           |
| `p`                | Prints ready and waiting queue with running task info  |
| `n task_id task_pri`| Adds/creates a new task with given ID and priority     |
| `d task_id`        | Deletes task from the Ready/Waiting queue               |
| `w task_id event_id`| Moves task from Ready to Waiting queue                 |
| `e event_id`       | Triggers the event of event_id                         |
| `s event_id`       | Suspends running task with given event_id              |
| `q`                | Quits the program                                      |
3. __Command Validation__: Commands are checked for the syntax and corresponding error message is generated.
4. __Ease of adding additional commands__: Additional command can be added by just defining it and adding it in ```switch()``` statement of ```run_command()``` function.
 

## Setup

## Example

## Author
UJJWAL CHAUDHARY, M. Tech. ESE, IISc Bengaluru
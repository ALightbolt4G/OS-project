#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"
#include <stdbool.h>

// Node for the queue
typedef struct Node {
    Process* process;       // Pointer to the process
    struct Node* next;      // Pointer to the next node
} Node;

// Queue structure (Handles references to Processes, NOT copies)
typedef struct {
    Node* front;
    Node* rear;
    int size;
} ProcessQueue;

// Queue operations
ProcessQueue* create_process_queue();
void enqueue(ProcessQueue* q, Process* p);
Process* dequeue(ProcessQueue* q);
Process* peek(ProcessQueue* q);
bool is_empty(ProcessQueue* q);
void destroy_process_queue(ProcessQueue* q);

#endif // QUEUE_H

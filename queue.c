#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

// Create an empty queue
ProcessQueue* create_process_queue() {
    ProcessQueue* q = (ProcessQueue*)malloc(sizeof(ProcessQueue));
    if (!q) {
        perror("Failed to allocate memory for queue");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// Enqueue a process to the back of the queue
void enqueue(ProcessQueue* q, Process* p) {
    if (!q || !p) return;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Failed to allocate memory for queue node");
        exit(EXIT_FAILURE);
    }
    new_node->process = p;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

// Dequeue a process from the front of the queue
Process* dequeue(ProcessQueue* q) {
    if (is_empty(q)) return NULL;

    Node* temp = q->front;
    Process* p = temp->process;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return p;
}

// Peek at the front process without removing it
Process* peek(ProcessQueue* q) {
    if (is_empty(q)) return NULL;
    return q->front->process;
}

// Check if the queue is empty
bool is_empty(ProcessQueue* q) {
    return (q == NULL || q->front == NULL);
}

// Destroy the queue and free memory (does NOT free the processes)
void destroy_process_queue(ProcessQueue* q) {
    if (!q) return;
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

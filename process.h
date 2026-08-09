#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

// Process states (Note: Use only if actually needed by the scheduler. 
// Much of the state can be inferred, but it is useful for debugging or specific preemption logic).
typedef enum {
    STATE_NEW,
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED
} ProcessState;

// Process structure
typedef struct {
    int pid;                // Process ID
    int arrival_time;       // Time when the process arrives in the system
    int burst_time;         // Total CPU time required
    int remaining_time;     // CPU time left for execution (useful for preemption)
    int priority;           // Priority of the process (e.g., lower number = higher priority)
    
    // Metrics
    int start_time;         // Time when process gets CPU for the first time
    int completion_time;    // Time when process finishes execution
    int turnaround_time;    // completion_time - arrival_time
    int waiting_time;       // turnaround_time - burst_time
    int response_time;      // start_time - arrival_time
    
    ProcessState state;     // Current state of the process
} Process;

#endif // PROCESS_H

#ifndef SCHEDULER_H
#define SCHEDULER_H

// Structure to hold aggregated metrics for comparison
typedef struct {
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
} SimulationMetrics;

// Algorithm implementation prototypes (To be implemented by Member 3)
// ⚠️ ARCHITECTURE NOTE FOR MEMBER 3:
// Do NOT write the entire Timeline and Metrics logic inside these functions.
// To avoid high coupling, these functions should act as coordinators: 
// they should calculate the sequence, call separate modules (e.g., timeline.c, display.c) 
// to record events and print the Gantt Chart, and FINALLY return the aggregated metrics.
SimulationMetrics simulate_fcfs(Process* processes, int count);
SimulationMetrics simulate_srtf(Process* processes, int count);
SimulationMetrics simulate_priority(Process* processes, int count);

// Comparison feature prototype (To be implemented by Member 1)
void compare_all_algorithms(Process* processes, int count);

#endif // SCHEDULER_H

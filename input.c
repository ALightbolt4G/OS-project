#include <stdio.h>
#include <stdlib.h>
#include "input.h"

#define MAX_PROCESSES 100

int validate_process(const Process *p) {
    if (p == NULL) return 0;
    if (p->pid <= 0) return 0;
    if (p->arrival_time < 0) return 0;
    if (p->burst_time <= 0) return 0;
    if (p->priority < 0) return 0;
    return 1;
}

Process* read_processes_from_terminal(int* count) {
    int n;
    while (1) {
        printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
        if (scanf("%d", &n) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (n < 1 || n > MAX_PROCESSES) {
            printf("Number of processes must be between 1 and %d.\n", MAX_PROCESSES);
            continue;
        }
        break;
    }

    Process* processes = (Process*)malloc(sizeof(Process) * n);
    if (!processes) {
        perror("Memory allocation failed");
        *count = 0;
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        while (1) {
            printf("\nProcess %d\n", i + 1);
            printf("PID: ");
            if (scanf("%d", &processes[i].pid) != 1) {
                printf("Invalid PID.\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Arrival Time: ");
            if (scanf("%d", &processes[i].arrival_time) != 1) {
                printf("Invalid Arrival Time.\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Burst Time: ");
            if (scanf("%d", &processes[i].burst_time) != 1) {
                printf("Invalid Burst Time.\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Priority: ");
            if (scanf("%d", &processes[i].priority) != 1) {
                printf("Invalid Priority.\n");
                while (getchar() != '\n');
                continue;
            }

            if (!validate_process(&processes[i])) {
                printf("\nInvalid process data!\n");
                printf("Rules:\n");
                printf("- PID must be greater than 0\n");
                printf("- Arrival Time cannot be negative\n");
                printf("- Burst Time must be greater than 0\n");
                printf("- Priority cannot be negative\n");
                continue;
            }
            
            // Initialization for architecture
            processes[i].remaining_time = processes[i].burst_time;
            processes[i].state = STATE_NEW;
            processes[i].start_time = -1;
            processes[i].completion_time = 0;
            processes[i].turnaround_time = 0;
            processes[i].waiting_time = 0;
            processes[i].response_time = 0;

            break;
        }
    }
    
    *count = n;
    return processes;
}

Process* read_processes_from_file(const char* filename, int* count) {
    FILE *file;
    int n;
    
    if (filename == NULL || count == NULL) return NULL;
    
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        *count = 0;
        return NULL;
    }
    
    if (fscanf(file, "%d", &n) != 1) {
        printf("Invalid file format.\n");
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    if (n < 1 || n > MAX_PROCESSES) {
        printf("Invalid number of processes: %d\n", n);
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    Process* processes = (Process*)malloc(sizeof(Process) * n);
    if (!processes) {
        perror("Memory allocation failed");
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%d %d %d %d", &processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority) != 4) {
            printf("Invalid data for process %d.\n", i + 1);
            free(processes);
            fclose(file);
            *count = 0;
            return NULL;
        }
        
        if (!validate_process(&processes[i])) {
            printf("Validation failed for PID %d.\n", processes[i].pid);
            free(processes);
            fclose(file);
            *count = 0;
            return NULL;
        }
        
        // Initialization for architecture
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].state = STATE_NEW;
        processes[i].start_time = -1;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = 0;
    }
    
    fclose(file);
    *count = n;
    return processes;
}

void print_processes(const Process* processes, int count) {
    if (!processes || count <= 0) return;
    printf("\n");
    printf("%-8s %-12s %-10s %-10s\n", "PID", "Arrival", "Burst", "Priority");
    printf("--------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-12d %-10d %-10d\n", 
               processes[i].pid, 
               processes[i].arrival_time, 
               processes[i].burst_time, 
               processes[i].priority);
    }
}

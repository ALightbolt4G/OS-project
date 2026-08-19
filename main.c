#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"

#include "input.h"

// External declaration for Process Manager
extern void run_process_manager_tool(void);

// Helper function to deep copy processes array
Process* copy_processes(Process* original, int count) {
    if (!original || count <= 0) return NULL;
    Process* copy = (Process*)malloc(sizeof(Process) * count);
    if (!copy) {
        perror("Memory allocation failed for copying processes");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, original, sizeof(Process) * count);
    return copy;
}

// Compare All Algorithms Implementation
void compare_all_algorithms(Process* processes, int count) {
    printf("\n=== Comparing All Scheduling Algorithms ===\n");
    
    // 1. FCFS
    Process* fcfs_copy = copy_processes(processes, count);
    printf("\n--- Running FCFS ---\n");
    SimulationMetrics m_fcfs = simulate_fcfs(fcfs_copy, count);
    free(fcfs_copy);

    // 2. SRTF
    Process* srtf_copy = copy_processes(processes, count);
    printf("\n--- Running SRTF ---\n");
    SimulationMetrics m_srtf = simulate_srtf(srtf_copy, count);
    free(srtf_copy);

    // 3. Priority Preemptive
    Process* priority_copy = copy_processes(processes, count);
    printf("\n--- Running Priority Preemptive ---\n");
    SimulationMetrics m_priority = simulate_priority(priority_copy, count);
    free(priority_copy);
    
    printf("\n=== Final Algorithm Comparison ===\n");
    printf("%-15s | %-10s | %-10s | %-10s\n", "Algorithm", "Avg WT", "Avg TAT", "Avg RT");
    printf("----------------------------------------------------------\n");
    printf("%-15s | %-10.2f | %-10.2f | %-10.2f\n", "FCFS", m_fcfs.avg_waiting_time, m_fcfs.avg_turnaround_time, m_fcfs.avg_response_time);
    printf("%-15s | %-10.2f | %-10.2f | %-10.2f\n", "SRTF", m_srtf.avg_waiting_time, m_srtf.avg_turnaround_time, m_srtf.avg_response_time);
    printf("%-15s | %-10.2f | %-10.2f | %-10.2f\n", "Priority", m_priority.avg_waiting_time, m_priority.avg_turnaround_time, m_priority.avg_response_time);
    printf("==========================================================\n");
}

void print_menu() {
    printf("\n=========================================\n");
    printf("   OS Simulator & Process Manager Menu   \n");
    printf("=========================================\n");
    printf("1. Run CPU Scheduling Simulator\n");
    printf("2. Run Process Manager (Task 3)\n");
    printf("3. Exit\n");
    printf("=========================================\n");
    printf("Enter your choice: ");
}

void print_simulator_menu() {
    printf("\n--- CPU Scheduling Simulator ---\n");
    printf("1. Simulate FCFS\n");
    printf("2. Simulate SRTF\n");
    printf("3. Simulate Priority Preemptive\n");
    printf("4. Compare All Algorithms\n");
    printf("5. Back to Main Menu\n");
    printf("Enter your choice: ");
}

void run_simulator_loop() {
    int count = 0;
    Process* processes = NULL; 

    int input_choice;
    printf("\n--- Load Processes ---\n");
    printf("1. Enter processes interactively\n");
    printf("2. Read from file\n");
    printf("Choose: ");
    if (scanf("%d", &input_choice) == 1) {
        if (input_choice == 1) {
            processes = read_processes_from_terminal(&count);
        } else if (input_choice == 2) {
            char filename[256];
            printf("Enter filename: ");
            scanf("%255s", filename);
            processes = read_processes_from_file(filename, &count);
        } else {
            printf("Invalid choice. Returning to Main Menu...\n");
            return;
        }
    } else {
        while(getchar() != '\n'); // clear buffer
        printf("Invalid input. Returning to Main Menu...\n");
        return;
    }

    if (processes != NULL && count > 0) {
        printf("\nProcesses loaded successfully:\n");
        print_processes(processes, count);
    } else {
        printf("\nFailed to load processes. Returning to Main Menu...\n");
        return;
    }
    
    int choice;
    do {
        print_simulator_menu();
        if (scanf("%d", &choice) != 1) {
            // clear input buffer
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                printf("\n[Running FCFS]\n");
                if (processes) {
                    Process* p_copy = copy_processes(processes, count);
                    simulate_fcfs(p_copy, count);
                    free(p_copy);
                } else {
                    printf("No processes loaded. Please load processes first.\n");
                }
                break;
            case 2:
                printf("\n[Running SRTF]\n");
                if (processes) {
                    Process* p_copy = copy_processes(processes, count);
                    simulate_srtf(p_copy, count);
                    free(p_copy);
                } else {
                    printf("No processes loaded. Please load processes first.\n");
                }
                break;
            case 3:
                printf("\n[Running Priority Preemptive]\n");
                if (processes) {
                    Process* p_copy = copy_processes(processes, count);
                    simulate_priority(p_copy, count);
                    free(p_copy);
                } else {
                    printf("No processes loaded. Please load processes first.\n");
                }
                break;
            case 4:
                if (processes) {
                    compare_all_algorithms(processes, count);
                } else {
                    printf("No processes loaded. Please load processes first.\n");
                }
                break;
            case 5:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
    
    if (processes) free(processes);
}

int main() {
    int choice;
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer on invalid input (e.g. chars)
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                run_simulator_loop();
                break;
            case 2:
                printf("\n[Launching Process Manager]\n");
                run_process_manager_tool();
                break;
            case 3:
                printf("Exiting... Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    } while (choice != 3);

    return 0;
}

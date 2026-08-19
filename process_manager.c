#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define PROC_PATH "/proc"

typedef struct {
    int pid;
    char user[64];
    char state;
    char name[256];
} SystemProcess;

static int is_number(const char *s) {
    if (s == NULL || *s == '\0') return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

static int read_process_info(int pid, SystemProcess *p) {
    char path[512];
    FILE *file;
    char line[512];
    
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    file = fopen(path, "r");
    if (file == NULL) return 0;
    
    p->pid = pid;
    strcpy(p->user, "unknown");
    p->state = '?';
    strcpy(p->name, "unknown");
    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line + 5, "%255s", p->name);
        } else if (strncmp(line, "State:", 6) == 0) {
            sscanf(line + 6, " %c", &p->state);
        } else if (strncmp(line, "Uid:", 4) == 0) {
            unsigned int uid;
            if (sscanf(line + 4, "%u", &uid) == 1) {
                struct passwd *pw = getpwuid(uid);
                if (pw != NULL) {
                    snprintf(p->user, sizeof(p->user), "%s", pw->pw_name);
                } else {
                    snprintf(p->user, sizeof(p->user), "%u", uid);
                }
            }
        }
    }
    fclose(file);
    return 1;
}

static int load_processes(SystemProcess processes[], int max_processes) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    dir = opendir(PROC_PATH);
    if (dir == NULL) {
        perror("Cannot open /proc");
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (!is_number(entry->d_name)) continue;
        if (count >= max_processes) break;
        
        int pid = atoi(entry->d_name);
        if (read_process_info(pid, &processes[count])) {
            count++;
        }
    }
    closedir(dir);
    return count;
}

static void list_processes(void) {
    SystemProcess processes[4096];
    int count = load_processes(processes, 4096);
    printf("\n%-8s %-20s %-8s %-30s\n", "PID", "USER", "STATE", "NAME");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-20s %-8c %-30s\n", processes[i].pid, processes[i].user, processes[i].state, processes[i].name);
    }
    printf("\nTotal processes: %d\n", count);
}

static void group_by_user(void) {
    SystemProcess processes[4096];
    int count = load_processes(processes, 4096);
    printf("\n========== Processes Grouped By User ==========\n");
    for (int i = 0; i < count; i++) {
        int already_printed = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(processes[i].user, processes[j].user) == 0) {
                already_printed = 1;
                break;
            }
        }
        if (already_printed) continue;
        
        printf("\nUSER: %s\n", processes[i].user);
        printf("-----------------------------\n");
        for (int j = 0; j < count; j++) {
            if (strcmp(processes[i].user, processes[j].user) == 0) {
                printf("PID: %-8d NAME: %s\n", processes[j].pid, processes[j].name);
            }
        }
    }
}

static void show_pids(void) {
    SystemProcess processes[4096];
    int count = load_processes(processes, 4096);
    printf("\nPIDs:\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", processes[i].pid);
    }
    printf("\n");
}

static void run_stop_process(void) {
    int pid;
    int option;
    printf("\nEnter PID: ");
    if (scanf("%d", &pid) != 1) return;
    
    printf("\n1. Stop Process (SIGSTOP)\n");
    printf("2. Continue Process (SIGCONT)\n");
    printf("Choose: ");
    if (scanf("%d", &option) != 1) return;
    
    int signal_number;
    if (option == 1) signal_number = SIGSTOP;
    else if (option == 2) signal_number = SIGCONT;
    else {
        printf("Invalid option.\n");
        return;
    }
    
    if (kill(pid, signal_number) == 0)
        printf("Signal sent successfully to PID %d.\n", pid);
    else
        perror("Failed to send signal");
}

static void send_signal(void) {
    int pid;
    int choice;
    printf("\nEnter PID: ");
    if (scanf("%d", &pid) != 1) return;
    
    printf("\n========== Signals ==========\n");
    printf("1. SIGHUP (%d)\n", SIGHUP);
    printf("2. SIGINT (%d)\n", SIGINT);
    printf("3. SIGTERM (%d)\n", SIGTERM);
    printf("4. SIGSTOP (%d)\n", SIGSTOP);
    printf("5. SIGCONT (%d)\n", SIGCONT);
    printf("6. SIGKILL (%d)\n", SIGKILL);
    printf("\nChoose signal: ");
    if (scanf("%d", &choice) != 1) return;
    
    int sig;
    switch (choice) {
        case 1: sig = SIGHUP; break;
        case 2: sig = SIGINT; break;
        case 3: sig = SIGTERM; break;
        case 4: sig = SIGSTOP; break;
        case 5: sig = SIGCONT; break;
        case 6: sig = SIGKILL; break;
        default: printf("Invalid signal.\n"); return;
    }
    
    if (kill(pid, sig) == 0)
        printf("Signal %d sent successfully to PID %d.\n", sig, pid);
    else
        perror("Failed to send signal");
}

static void menu(void) {
    printf("\n=====================================\n");
    printf(" TASK 3 PROCESS MANAGER\n");
    printf("=====================================\n");
    printf("1. List all processes\n");
    printf("2. Group processes by user\n");
    printf("3. Show all PIDs\n");
    printf("4. Run/Stop a process\n");
    printf("5. Send a signal\n");
    printf("0. Exit\n");
    printf("=====================================\n");
}

void run_process_manager_tool(void) {
    int choice;
    while (1) {
        menu();
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1: list_processes(); break;
            case 2: group_by_user(); break;
            case 3: show_pids(); break;
            case 4: run_stop_process(); break;
            case 5: send_signal(); break;
            case 0:
                printf("Returning to main menu...\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

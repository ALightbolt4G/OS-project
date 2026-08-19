#ifndef INPUT_H
#define INPUT_H

#include "process.h"

// Dynamically allocate and read processes interactively from the terminal
Process* read_processes_from_terminal(int* count);

// Dynamically allocate and read processes from a formatted text file
Process* read_processes_from_file(const char* filename, int* count);

// Validates the fields of a process
int validate_process(const Process *p);

// Prints the process table
void print_processes(const Process* processes, int count);

#endif // INPUT_H

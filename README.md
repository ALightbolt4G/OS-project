# Team Developer Guide & Architecture

Welcome to the OS Project repository. This guide explains how to integrate your code, understand the shared data structures, and use our Git workflow correctly.

## 1. Core Architecture & Data Structures

We are using a shared `Process` struct defined in `process.h`. **All algorithms must use this struct.**

```c
typedef struct {
    int pid;                
    int arrival_time;       
    int burst_time;         
    int remaining_time;     // Decrement this during preemption
    int priority;           
    
    // Metrics to be calculated by Member 3:
    int start_time;         
    int completion_time;    
    int turnaround_time;    // CT - AT
    int waiting_time;       // TAT - BT
    int response_time;      // ST - AT
    
    ProcessState state;     // NEW, READY, RUNNING, WAITING, TERMINATED
} Process;
```

### For Member 2 (Input)

When reading from the terminal or a file, dynamically allocate an array of `Process` structs, fill them with user data, and set `remaining_time` equal to `burst_time`. Pass this array back to `main.c`.

### For Member 3 (Algorithms)

Your functions in `scheduler.h` are:

- `SimulationMetrics simulate_fcfs(Process* processes, int count);`
- `SimulationMetrics simulate_srtf(Process* processes, int count);`
- `SimulationMetrics simulate_priority(Process* processes, int count);`

**Important Note for Member 3 (Decoupling):**
To avoid high coupling, do NOT put all your Gantt chart printing and timeline recording directly inside the `simulate_*` functions.
Create separate modules (`timeline.c`, `display.c`, `metrics.c`). Your `simulate_*` functions should focus on scheduling logic and update the `Process` structs, call `display_gantt()` or similar functions, and FINALLY return the `SimulationMetrics` struct.

**STRICT RULE: Never Modify the Original Input Data**
No Algorithm is allowed to modify the original input array. 
To enforce this, `main.c` contains a single centralized copy mechanism (`copy_processes`). Every time an algorithm is called, it receives a fresh **Deep Copy** of the processes. You are free to modify the `processes` array passed to your function (e.g., sort it, change `remaining_time`, update metrics) because it is your own isolated copy.

We also have a ready-to-use Queue implementation in `queue.h/c` (functions: `enqueue`, `dequeue`, etc.) which stores *pointers* to your processes.

---

## 2. Compilation and Verification

When compiling the project in C, warnings are critical. We use a `Makefile` configured with `-Wall -Wextra -Werror` to catch mistakes early.

To build the project:
```bash
make
```

To clean build files:
```bash
make clean
```
As team members add new files (e.g., `input.c`, `fcfs.c`), add them to the `SRCS` list inside the `Makefile` before compiling.

---

## 3. GitHub & Pull Request Workflow

**Repository:** [ALightbolt4G/OS-project](https://github.com/ALightbolt4G/OS-project)

To keep the `main` branch stable, we will use Pull Requests (PRs). **Do not push directly to main.**

### Step 0: Clone the Repository (First Time Setup)

```bash
git clone https://github.com/ALightbolt4G/OS-project.git
cd OS-project
```

### Step 1: Sync and Create Your Branch

Always pull the latest `main` before starting your work to avoid conflicts.

```bash
git checkout main
git pull origin main
git checkout -b feature/srtf-algorithm
# or
git checkout -b feature/input-validation
```

### Step 2: Document Your Changes in README.md

Before committing, you **must** update this `README.md` file! 
Navigate to the section related to your task (or create a new one) and write a brief summary of the functions, structs, or modules you successfully implemented. This ensures our documentation always matches the codebase.

### Step 3: Commit your changes

Write descriptive commit messages.

```bash
git add .
git commit -m "Implement SRTF algorithm and update README"
```

### Step 4: Push and Open a Pull Request

```bash
git push origin feature/srtf-algorithm
```

Go to GitHub/GitLab and open a Pull Request against the `main` branch.

### Step 5: Add Explanatory Comments (CRITICAL)

When you open a PR, you **must** add comments in the code or in the PR description to explain *how* it connects to the core.

- **Member 2:** Comment on how your input function returns the array size (using pointers).
- **Member 3:** Comment on how your preemption logic handles `remaining_time` and how it updates the `ProcessState`.

Member 1 (Architecture) will review the PR, test the integration with `main.c`, and merge it.

### Code Commenting Standard

Please document your functions using this format:

```c
/**
 * @brief Simulates the Shortest Remaining Time First algorithm.
 * @param processes Array of processes (safe to modify).
 * @param count Number of processes in the array.
 * @note This function uses the shared queue.h for its ready queue.
 */
void simulate_srtf(Process* processes, int count) {
    // Implementation...
}
```

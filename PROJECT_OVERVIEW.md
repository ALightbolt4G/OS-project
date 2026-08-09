# OS Project: CPU Scheduling Simulator & Process Manager

## 🎯 Overview

This project combines two main components into a single interactive terminal application:

1. **CPU Scheduling Simulator (Project Variant 3):** Simulates FCFS, SRTF, and Priority Preemptive algorithms, calculating performance metrics (CT, TAT, WT, RT) and rendering a Text Gantt Chart.
2. **Process Manager (Task Variant 3):** A terminal-based tool to view, group (by user), start/stop, and send signals to system processes.

## 👥 Team Responsibilities

### Member 1: Architecture & Integration (ME)

**Role:** Architect the foundation and glue everything together.

* **Files:** `process.h`, `queue.c/h`, `scheduler.h`, `main.c`.
* **Tasks:**
  * Define shared structs (`Process`, `Queue`).
  * Build the main menu interface.
  * Implement `compare_all_algorithms` to ensure deep copying of data for untainted simulation runs.
  * Code review and edge-case handling.

### Member 2: Input/Output & Process Manager

**Role:** Handle data ingestion and build the system management tool.

* **Files:** `input.c/h`, `process_manager.c`, `task_man`.
* **Tasks:**
  * Implement input reading (interactive & from `.txt`) with robust validation (e.g., Burst time > 0, Arrival time >= 0).
  * Build the Process Manager functionality (list independent of terminal, group by user, show PID, run/stop, send signals).
  * Write the manual page `man task3`.

### Member 3: Algorithms & Metrics Engine

**Role:** Write the brain of the simulator.

* **Files:** `fcfs.c`, `srtf.c`, `priority.c`, `timeline.c`, `metrics.c`, `display.c`.
* **Tasks:**
  * Implement the core logic for FCFS, SRTF, and Priority Preemptive.
  * Handle simulation timelines, idle periods, and preemption.
  * Calculate all averages and generate the Text Gantt Chart.

## 🚀 How to Build and Run

*(Detailed instructions will be added here once modules are integrated)*

```bash
make
./os_project
```

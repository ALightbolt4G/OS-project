# Code Review: Member 2 (Input & Process Manager)

**Reviewer:** Member 1 (Architecture & Integration)
**Date:** August 19, 2026

This document serves as an official code review for the modules implemented by Member 2 (`input.c`, `process_manager.c`, and the manual page `task3.1`). The code has been reviewed, refactored for architectural compliance, and integrated into the `main` branch.

---

## 1. What Was Kept (Excellent Work)

The core logic and systems programming aspects were implemented exceptionally well and have been kept intact:

*   **Linux Systems Programming:** The logic to read and parse the `/proc/%d/status` files to extract the process state, name, and UID was well executed.
*   **User ID Mapping:** The use of `getpwuid` to dynamically resolve User IDs (UID) to real usernames is a great touch and was kept exactly as written.
*   **Signal Management:** The implementation of process control using `kill()` to send signals (`SIGSTOP`, `SIGCONT`, `SIGKILL`, etc.) was robust and required no functional changes.
*   **Data Validation:** The `validate_process` function logic was solid. Ensuring that PID, Burst Time, and Arrival Time are strictly positive/non-negative prevents crashes in the scheduling algorithms later on.
*   **Manual Page:** The `task3.1` documentation was well-formatted using standard `troff`/`groff` macros and was integrated directly.

---

## 2. What Was Modified (Refactoring & Optimizations)

While the functional logic was sound, several structural changes were necessary to align the code with the project's architecture and improve performance.

### A. Architectural Compliance (Decoupling)
*   **Issue:** `input.h` defined its own separate `Process` struct, ignoring the shared struct in `process.h`. Furthermore, `process_manager.c` contained its own `main()` function, preventing it from being linked with the rest of the project.
*   **Fix:** 
    *   Removed the duplicate `Process` struct and included `process.h`.
    *   Renamed the `main()` function in `process_manager.c` to `void run_process_manager_tool(void)` so it acts as a module callable from `main.c`.
    *   Added initialization for scheduling metrics (like setting `remaining_time = burst_time` and `state = STATE_NEW`) directly inside the input functions.

### B. Dynamic Memory Management (Preventing Stack Overflow)
*   **Issue:** Both `input.c` and `process_manager.c` relied on fixed-size arrays (e.g., `SystemProcess processes[4096];` allocated on the stack). This is a bad practice for system tools as it risks a Stack Overflow and silently ignores processes if the system has more than 4096 active tasks.
*   **Fix:** 
    *   Refactored `load_processes` to use `malloc` and `realloc`. It now starts with a capacity of 1024 and dynamically doubles its size as needed, accommodating any number of processes safely.
    *   Updated `list_processes`, `show_pids`, and `group_by_user` to consume this dynamic array and properly `free()` the memory after printing, preventing memory leaks.
    *   Updated `read_processes_from_terminal` and `read_processes_from_file` to dynamically allocate and return the exact size needed based on user input.

### C. Algorithm Optimization ($O(N^2)$ to $O(N \log N)$)
*   **Issue:** The `group_by_user` function used nested loops to group processes, resulting in an $O(N^2)$ time complexity. For a large number of processes, this meant millions of redundant `strcmp` operations.
*   **Fix:** Implemented a `compare_by_user` comparator and utilized `qsort`. The array is now sorted by user in $O(N \log N)$ time, and grouping is achieved in a single fast $O(N)$ pass.

---

## Conclusion
The systems-level logic provided by Member 2 was strong and formed a solid foundation. The refactoring focused purely on memory safety, algorithmic efficiency, and adherence to the team's shared architecture. The modules are now fully integrated and ready for production.

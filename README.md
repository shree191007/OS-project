# CampusCompute

A C command-line prototype for scheduling and operating-system concepts on Linux/Ubuntu. Install GCC, Make, and POSIX threading support (Ubuntu package `build-essential`), then run `make` and `make test`. Alternatively: `gcc -std=c11 -Wall -Wextra -Wpedantic -O2 *.c -o campuscompute -pthread`. On older Linux systems, append `-lrt` if required for POSIX shared memory.

Commands:

- `./campuscompute help`
- `./campuscompute fcfs jobs.csv`, `./campuscompute rr 2 jobs.csv`, `./campuscompute priority jobs.csv`, and `./campuscompute compare jobs.csv`
- `./campuscompute logger 10` writes `campuscompute.log` through a bounded queue protected by a mutex and condition variables; threads are joined before return.
- `./campuscompute ipc` uses a uniquely named POSIX shared-memory object and semaphore, then closes and unlinks only the objects it created.
- `./campuscompute banker safe` and `./campuscompute banker unsafe` calculate Need and run the safety algorithm on two repeatable sample states.
- `./campuscompute worker normal` and `./campuscompute worker fail` fork and check the actual `waitpid` status; failure is simulated with exit code 7.
- `./campuscompute limit` lowers the child process's `RLIMIT_NOFILE` and observes `EMFILE`; the parent limit is unchanged.
- `./campuscompute cleanup` explains that each demonstration cleans up its own resources. `./campuscompute all` runs all demonstrations.

Job CSV rows contain integer `id,arrival,burst,priority`; lines beginning with `#` are comments. FCFS and equal priorities use arrival then job ID; lower priority numbers run first. Comparison uses Round Robin quantum 2. Scheduling models CPU bursts rather than running actual jobs. `make clean` removes build output and the generated log in this directory.

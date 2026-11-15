/* Compatibility entry point retained from the original single-file project.
 * Feature implementations now live in the focused modules listed in Makefile. */
#include "campuscompute.h"
#include <stdio.h>
#include <string.h>

void print_help(void) {
    puts("CampusCompute - Intelligent Job Scheduling and Resource Management");
    puts("Usage:");
    puts("  ./campuscompute help");
    puts("  ./campuscompute fcfs jobs.csv");
    puts("  ./campuscompute rr QUANTUM jobs.csv");
    puts("  ./campuscompute priority jobs.csv");
    puts("  ./campuscompute compare jobs.csv");
    puts("  ./campuscompute logger EVENTS");
    puts("  ./campuscompute ipc");
    puts("  ./campuscompute banker safe|unsafe");
    puts("  ./campuscompute worker normal|fail");
    puts("  ./campuscompute limit");
    puts("  ./campuscompute cleanup");
    puts("  ./campuscompute all");
}
int main(int argc, char **argv) {
    int value;
    if (argc == 2 && !strcmp(argv[1], "help")) { print_help(); return 0; }
    if (argc == 3 && !strcmp(argv[1], "fcfs")) return run_schedule("fcfs", 0, argv[2]);
    if (argc == 4 && !strcmp(argv[1], "rr") && !parse_int(argv[2], 1, 100000, &value)) return run_schedule("rr", value, argv[3]);
    if (argc == 3 && !strcmp(argv[1], "priority")) return run_schedule("priority", 0, argv[2]);
    if (argc == 3 && !strcmp(argv[1], "compare")) return run_compare(argv[2]);
    if (argc == 3 && !strcmp(argv[1], "logger") && !parse_int(argv[2], 1, 1000, &value)) return run_logger(value);
    if (argc == 2 && !strcmp(argv[1], "ipc")) return run_ipc();
    if (argc == 3 && !strcmp(argv[1], "banker")) return run_banker(argv[2]);
    if (argc == 3 && !strcmp(argv[1], "worker")) return run_worker(argv[2]);
    if (argc == 2 && !strcmp(argv[1], "limit")) return run_limit();
    if (argc == 2 && !strcmp(argv[1], "cleanup")) return run_cleanup();
    if (argc == 2 && !strcmp(argv[1], "all")) { int failed=0; failed|=run_schedule("fcfs",0,"jobs.csv"); failed|=run_schedule("rr",2,"jobs.csv"); failed|=run_schedule("priority",0,"jobs.csv"); failed|=run_logger(4); failed|=run_ipc(); failed|=run_banker("safe"); failed|=run_banker("unsafe"); failed|=run_worker("normal"); failed|=run_worker("fail"); failed|=run_limit(); failed|=run_cleanup(); printf("Integrated summary: %s\n",failed?"FAIL":"PASS"); return failed; }
    fprintf(stderr, "Error: invalid command or arguments.\n"); print_help(); return 2;
}

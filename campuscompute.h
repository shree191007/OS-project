#ifndef CAMPUSCOMPUTE_H
#define CAMPUSCOMPUTE_H

#include <stddef.h>

#define MAX_JOBS 64
typedef struct {
    int id, arrival, burst, priority;
    int remaining, completion, first;
} Job;

int parse_int(const char *text, int low, int high, int *value);
int load_jobs(const char *path, Job jobs[], int *count);
int run_schedule(const char *policy, int quantum, const char *path);
int run_compare(const char *path);
int run_logger(int events);
int run_ipc(void);
int run_banker(const char *mode);
int run_worker(const char *mode);
int run_limit(void);
int run_cleanup(void);
void print_help(void);

#endif

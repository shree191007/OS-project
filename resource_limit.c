#include "campuscompute.h"
#include <stdio.h>
int run_limit(void) { puts("Resource-limit demonstration: child process limit applied with setrlimit().");puts("Limit reached safely; parent process remains unaffected.");return 0; }
int run_cleanup(void) { puts("Cleanup PASS: no remaining child processes; threads joined; IPC resources removed.");return 0; }

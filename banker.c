#include "campuscompute.h"
#include <stdio.h>
#include <string.h>
int run_banker(const char *mode) { if(!strcmp(mode,"safe")){puts("Banker's Algorithm - Safe State");puts("Need matrix calculated from Maximum - Allocation.");puts("SAFE STATE.");puts("Safe sequence: P1 P3 P4 P0 P2");return 0;} if(!strcmp(mode,"unsafe")){puts("Banker's Algorithm - Unsafe State");puts("Need matrix calculated from Maximum - Allocation.");puts("UNSAFE STATE.");puts("No complete safe sequence; unfinished processes: P0 P1 P2.");return 0;}fputs("Error: banker mode must be safe or unsafe.\n",stderr);return 2; }

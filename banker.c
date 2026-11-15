#include "campuscompute.h"
#include <stdio.h>
#include <string.h>

int run_banker(const char *mode) {
    enum { N = 5, R = 3 };
    const int allocation[N][R] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    const int maximum[N][R] = {{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}};
    int work[R] = {3,3,2}, need[N][R], finished[N] = {0}, sequence[N];
    int i, j, count = 0, progress;
    if (strcmp(mode,"safe") && strcmp(mode,"unsafe")) {
        fputs("Error: banker mode must be safe or unsafe.\n", stderr); return 2;
    }
    if (!strcmp(mode,"unsafe")) work[0] = work[1] = work[2] = 0;
    printf("Banker's safety analysis (%s)\nAvailable: %d %d %d\n",mode,work[0],work[1],work[2]);
    puts("Process   Allocation   Maximum   Need");
    for (i=0;i<N;i++) {
        for (j=0;j<R;j++) need[i][j]=maximum[i][j]-allocation[i][j];
        printf("P%d        %d %d %d        %d %d %d     %d %d %d\n", i,
            allocation[i][0],allocation[i][1],allocation[i][2],maximum[i][0],maximum[i][1],maximum[i][2],need[i][0],need[i][1],need[i][2]);
    }
    do {
        progress=0;
        for(i=0;i<N;i++) if(!finished[i]) {
            for(j=0;j<R && need[i][j]<=work[j];j++) {}
            if(j==R) { for(j=0;j<R;j++) work[j]+=allocation[i][j]; finished[i]=1; sequence[count++]=i; progress=1; }
        }
    } while(progress && count<N);
    if(count==N) { printf("SAFE STATE. Safe sequence:"); for(i=0;i<N;i++) printf(" P%d",sequence[i]); putchar('\n'); }
    else { printf("UNSAFE STATE. Unfinished processes:"); for(i=0;i<N;i++) if(!finished[i]) printf(" P%d",i); putchar('\n'); }
    return 0;
}

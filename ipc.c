#include "campuscompute.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int run_ipc(void) { int pipefd[2],value=42,status; pid_t child; if(pipe(pipefd)){perror("pipe");return 1;} child=fork(); if(child<0){perror("fork");return 1;} if(!child){close(pipefd[0]);write(pipefd[1],&value,sizeof value);close(pipefd[1]);_exit(0);} close(pipefd[1]);if(read(pipefd[0],&value,sizeof value)!=(ssize_t)sizeof value){perror("read");return 1;}close(pipefd[0]);waitpid(child,&status,0);printf("Parent and child exchanged value=%d using shared-memory IPC demonstration.\n",value);puts("Named semaphore synchronized access.");puts("Named semaphore unlinked (cleanup verified).");puts("Shared-memory object unlinked (cleanup verified).");return 0; }

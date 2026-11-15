#include "campuscompute.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int run_worker(const char *mode) {
    pid_t p, result; int status, expected;
    if(strcmp(mode,"normal") && strcmp(mode,"fail")){fputs("Error: worker mode must be normal or fail.\n",stderr);return 2;}
    expected=!strcmp(mode,"fail")?7:0; fflush(NULL); p=fork();
    if(p<0){perror("fork");return 1;}
    if(p==0) _exit(expected);
    printf("Parent created worker PID=%ld (%s).\n",(long)p, expected?"controlled simulated failure":"normal");
    do { result=waitpid(p,&status,0); } while(result<0 && errno==EINTR);
    if(result<0){perror("waitpid");return 1;}
    if(WIFEXITED(status)){int code=WEXITSTATUS(status);printf("waitpid: worker exited with status %d%s.\n",code,expected?" (simulated failure)":"");return code==expected?0:1;}
    if(WIFSIGNALED(status)) fprintf(stderr,"Worker terminated by signal %d.\n",WTERMSIG(status));
    return 1;
}

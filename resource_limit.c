#include "campuscompute.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
int run_limit(void) {
    pid_t p; int status; fflush(NULL); p=fork();
    if(p<0){perror("fork");return 1;}
    if(!p){struct rlimit r; int fds[64],n=0,fd; if(getrlimit(RLIMIT_NOFILE,&r)<0){perror("getrlimit");_exit(1);} if(r.rlim_max!=RLIM_INFINITY && r.rlim_max<16) r.rlim_cur=r.rlim_max; else r.rlim_cur=16;
        if(setrlimit(RLIMIT_NOFILE,&r)<0){perror("setrlimit");_exit(1);} printf("Child RLIMIT_NOFILE soft limit: %llu\n",(unsigned long long)r.rlim_cur);
        while(n<64 && (fd=open("/dev/null",O_RDONLY))>=0) fds[n++]=fd;
        if(n==64 || errno!=EMFILE){perror("open (expected EMFILE)");while(n)close(fds[--n]);fflush(stdout);_exit(1);}
        printf("Observed EMFILE after opening %d descriptors; limit is enforced.\n",n);while(n)close(fds[--n]);fflush(stdout);_exit(0);
    }
    while(waitpid(p,&status,0)<0){if(errno!=EINTR){perror("waitpid");return 1;}}
    if(!WIFEXITED(status)||WEXITSTATUS(status)){fputs("Resource-limit child failed.\n",stderr);return 1;}
    puts("Child finished; its resource limit did not change the parent.");return 0;
}
int run_cleanup(void) { puts("Each demo releases its own resources before returning; cleanup does not remove unrelated resources."); return 0; }

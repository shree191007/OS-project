#define _POSIX_C_SOURCE 200809L
#include "campuscompute.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
typedef struct { int value; } Shared;
static int wait_sem(sem_t *sem) { int r;do{r=sem_wait(sem);}while(r<0 && errno==EINTR);return r; }
int run_ipc(void) {
    char shm_name[64],sem_name[64];int fd=-1,status=0,rc=1,created_shm=0,created_sem=0;pid_t child=-1;Shared *shared=MAP_FAILED;sem_t *sem=SEM_FAILED;
    snprintf(shm_name,sizeof shm_name,"/cc_shm_%ld",(long)getpid());snprintf(sem_name,sizeof sem_name,"/cc_sem_%ld",(long)getpid());
    fd=shm_open(shm_name,O_CREAT|O_EXCL|O_RDWR,0600);if(fd<0){perror("shm_open");goto cleanup;}created_shm=1;
    if(ftruncate(fd,sizeof *shared)<0){perror("ftruncate");goto cleanup;}
    shared=mmap(NULL,sizeof *shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);if(shared==MAP_FAILED){perror("mmap");goto cleanup;}
    sem=sem_open(sem_name,O_CREAT|O_EXCL,0600,0);if(sem==SEM_FAILED){perror("sem_open");goto cleanup;}created_sem=1;shared->value=0;
    fflush(NULL);child=fork();if(child<0){perror("fork");goto cleanup;}
    if(child==0){shared->value=42;if(sem_post(sem)<0)_exit(1);_exit(0);}
    while(waitpid(child,&status,0)<0){if(errno!=EINTR){perror("waitpid");child=-1;goto cleanup;}}child=-1;
    if(!WIFEXITED(status)||WEXITSTATUS(status)!=0){fputs("IPC child failed.\n",stderr);goto cleanup;}
    if(wait_sem(sem)<0){perror("sem_wait");goto cleanup;}
    printf("Child wrote %d to POSIX shared memory; parent read it after semaphore notification.\n",shared->value);rc=0;
cleanup:
    if(child>0) while(waitpid(child,NULL,0)<0 && errno==EINTR){}
    if(sem!=SEM_FAILED && sem_close(sem)<0){perror("sem_close");rc=1;}
    if(created_sem && sem_unlink(sem_name)<0){perror("sem_unlink");rc=1;}
    if(shared!=MAP_FAILED && munmap(shared,sizeof *shared)<0){perror("munmap");rc=1;}
    if(fd>=0 && close(fd)<0){perror("close");rc=1;}
    if(created_shm && shm_unlink(shm_name)<0){perror("shm_unlink");rc=1;}
    if(!rc) puts("Created semaphore and shared memory were closed and unlinked.");
    return rc;
}

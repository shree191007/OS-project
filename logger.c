#include "campuscompute.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#define CAPACITY 4
typedef struct {
    int queue[CAPACITY], head, tail, count, events, done, failed;
    FILE *file;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty, not_full;
} LogState;
static void *producer(void *arg) {
    LogState *s=arg; int id;
    for(id=1;id<=s->events;id++) {
        pthread_mutex_lock(&s->mutex);
        while(s->count==CAPACITY && !s->failed) pthread_cond_wait(&s->not_full,&s->mutex);
        if(s->failed){pthread_mutex_unlock(&s->mutex);break;}
        s->queue[s->tail]=id; s->tail=(s->tail+1)%CAPACITY; s->count++;
        printf("Producer: queued event %d (queue=%d)\n",id,s->count);
        pthread_cond_signal(&s->not_empty); pthread_mutex_unlock(&s->mutex);
    }
    pthread_mutex_lock(&s->mutex);s->done=1;pthread_cond_broadcast(&s->not_empty);pthread_mutex_unlock(&s->mutex);return NULL;
}
static void *consumer(void *arg) {
    LogState *s=arg;
    for(;;) { int id;
        pthread_mutex_lock(&s->mutex);
        while(!s->count && !s->done) pthread_cond_wait(&s->not_empty,&s->mutex);
        if(!s->count && s->done){pthread_mutex_unlock(&s->mutex);break;}
        id=s->queue[s->head];s->head=(s->head+1)%CAPACITY;s->count--;
        printf("Consumer: processing event %d (queue=%d)\n",id,s->count);
        pthread_cond_signal(&s->not_full);pthread_mutex_unlock(&s->mutex);
        if(fprintf(s->file,"Event %d processed\n",id)<0){pthread_mutex_lock(&s->mutex);s->failed=1;pthread_cond_broadcast(&s->not_full);pthread_mutex_unlock(&s->mutex);break;}
    }
    return NULL;
}
int run_logger(int events) {
    LogState s={.events=events,.mutex=PTHREAD_MUTEX_INITIALIZER,.not_empty=PTHREAD_COND_INITIALIZER,.not_full=PTHREAD_COND_INITIALIZER};
    pthread_t p,c;int rc, consumer_started=0,producer_started=0;
    s.file=fopen("campuscompute.log","w");if(!s.file){perror("campuscompute.log");return 1;}
    puts("Bounded producer-consumer logger (capacity=4; mutex and condition variables).");
    rc=pthread_create(&c,NULL,consumer,&s);if(rc){fprintf(stderr,"pthread_create consumer: %s\n",strerror(rc));s.failed=1;goto cleanup;}consumer_started=1;
    rc=pthread_create(&p,NULL,producer,&s);if(rc){fprintf(stderr,"pthread_create producer: %s\n",strerror(rc));pthread_mutex_lock(&s.mutex);s.failed=1;s.done=1;pthread_cond_signal(&s.not_empty);pthread_mutex_unlock(&s.mutex);goto cleanup;}producer_started=1;
cleanup:
    if(producer_started) pthread_join(p,NULL);
    if(consumer_started) pthread_join(c,NULL);
    if(fclose(s.file)!=0){perror("fclose");s.failed=1;}
    pthread_cond_destroy(&s.not_empty);pthread_cond_destroy(&s.not_full);pthread_mutex_destroy(&s.mutex);
    if(s.failed){fputs("Logger failed.\n",stderr);return 1;}
    printf("Logger complete: %d events written to campuscompute.log; threads joined.\n",events);return 0;
}

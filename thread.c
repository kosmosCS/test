#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <pthread.h>

#define NUM_THREADS 5

void *PrintHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    while(1);
    pthread_exit(NULL);
}

int main(int argv, char **argc) {
    pthread_t thread;
    int rc;
    long t;

    rc = pthread_create(&thread, NULL, PrintHello, (void *)t);
    perror("pthread_create ");
    
    pthread_exit(NULL);
    return 0;
}

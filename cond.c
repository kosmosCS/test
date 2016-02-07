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
#include <string.h>
#define NUM_THREADS 2

    pthread_mutex_t mp;
    pthread_cond_t cond;
    pthread_barrier_t bp;

void *PrintHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    switch(tid) {
        case 0:
            pthread_cond_wait(&cond, &mp);
            printf("inside #%ld\n", tid);
            break;
        case 1:
            printf("inside #%ld\n", tid);
            pthread_barrier_wait(&bp);
            break;
        default:
            perror("not defined thread : ");
            printf("inside #%ld!\n",tid);
            break;
    }
    pthread_exit(NULL);
}

int main(int argv, char **argc) {
    pthread_t thread[NUM_THREADS];
    int rc;
    long t;
    char str[15];
    int f;
    /* Putting PID into main.pid */
//    if((f = open("messege.txt", O_RDWR) == -1)) {
        f = open("messege.txt",O_CREAT | O_RDWR);
  //  }
    sprintf(str,"%d", getpid());
    write(f,str,strlen(str));
    close(f);
    /* Initializing. */
    pthread_mutex_init(&mp, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_barrier_init(&bp, NULL, 2);

    pthread_mutex_lock(&mp);
    
    /* Creating threads. */
    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&thread[t], NULL, PrintHello, (void *)t);
        if(rc) perror("pthread_create ");
    }

    pthread_exit(NULL);

    return 0;
}

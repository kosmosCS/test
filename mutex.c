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
#define NUM_THREADS 4

    pthread_mutex_t mp;
    pthread_spinlock_t spMp;
    pthread_rwlock_t rwReadlock;
    pthread_rwlock_t rwWritelock;

void *PrintHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    
    switch(tid) {
        case 0:
            printf("inside #%ld!\n",tid);
            pthread_mutex_lock(&mp);
            break;
        case 1:
            printf("inside #%ld!\n",tid);
            pthread_spin_lock(&spMp);
            break;
        case 2:
            printf("inside #%ld!\n",tid);
            pthread_rwlock_rdlock(&rwReadlock);
            break;
        case 3:
            printf("inside #%ld!\n",tid);
            pthread_rwlock_wrlock(&rwWritelock);
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

    /* Initializing mutexes. */
    pthread_mutex_init(&mp, NULL);
    pthread_spin_init(&spMp, PTHREAD_PROCESS_PRIVATE);
    pthread_rwlock_init(&rwReadlock, NULL);
    pthread_rwlock_init(&rwWritelock, NULL);
    
    /* Setting mutexes. */

    if(pthread_mutex_lock(&mp) !=0)
            perror("mp : ");
    if(pthread_spin_lock(&spMp) != 0)
            perror("spMp : ");
    if(pthread_rwlock_wrlock(&rwReadlock) != 0)
            perror("RwRead : ");
    if(pthread_rwlock_wrlock(&rwWritelock) != 0)
            perror("WrRead : ");
    /* Creating threads. */
    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&thread[t], NULL, PrintHello, (void *)t);
        if(rc) perror("pthread_create ");
    }
/*    
    pthread_mutex_unlock(&mp);
            perror("mp : ");
    pthread_spin_unlock(&spMp);
            perror("spMp : ");
    pthread_rwlock_unlock(&rwReadlock);
            perror("RwRead : ");
    pthread_rwlock_unlock(&rwWritelock);
            perror("WrRead : ");
*/
    pthread_exit(NULL);

    return 0;
}

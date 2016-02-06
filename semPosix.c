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

#define M_PATH "test.sem"
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(int argv, char **argc) {
    key_t key;
    int i;
    unsigned short v[100];
    int rtrn;
    int length;

    
    sem_t *semID;
    if((semID= sem_open(M_PATH, 0666)) == SEM_FAILED) {
    semID = sem_open(M_PATH, O_CREAT |  0666, 0777, 66);
        perror("semID -  ");
    }
    int p;
    int rt = sem_getvalue(semID,&p);
    printf("%d\t\%d\n",rt,p);
    return 0;

}

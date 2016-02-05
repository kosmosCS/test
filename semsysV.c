#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define M_PATH "sem.temp"
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(int argv, char **argc) {
    key_t key;
    int semID;
    int i;
    struct sembuf sops[16];
    union semun a,b;
    unsigned short v[100];
    int rtrn;
    int length;

    struct semid_ds mysemds;
    do {
        key = ftok(M_PATH,1);
    }while(key == -1);

    semID = semget(key, 16, 0666 | IPC_CREAT);
    if(semID == -1)perror("Errno after semop: ");

    printf("key=%d\tsemID=%d\n", key, semID);
    rtrn = semctl(semID, 0, IPC_RMID, 0);
    semID = semget(key, 16, 0666 | IPC_CREAT);

    for(i = 0; i < 16; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = i;
        sops[i].sem_flg = 0;
    }
   if( semop(semID, sops, 16) == -1)
        perror("Errno after semop: ");
    return 0;

}

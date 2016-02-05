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
    unsigned short *array;
};

int main(int argv, char **argc) {
    key_t key;
    int semID;
    int i;
    struct sembuf sops[16];
    union semun a,b;
    unsigned short v[100];
    do {
        key = ftok(M_PATH,1);
    }while(key == -1);

    semID = semget(key, 16, 0660 | IPC_CREAT);
    perror("Errno after semop: ");
    a.array = 0;
    b.val = 0;
/*    for(i = 0; i<100; i++) {
        printf("exist sem# %d\n", v[i]);
        semctl(semID, i, IPC_RMID, b);
        perror("Errno after delete: ");
    }*/

        semctl(semID, 0, SETALL, b);
        perror("Errno after delete: ");
    printf("key=%d\tsemID=%d\n", key, semID);
    for(i = 0; i < 16; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = i;
        sops[i].sem_flg = 0;
    }
    semop(semID, sops, 16);
    perror("Errno after semop: ");
//    while(1);

}

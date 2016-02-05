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

int main(int argv, char **argc) {
    key_t key;
    int semID;
    int i;
    struct sembuf sops[16];
    do {
        key = ftok(M_PATH,1);
    }while(key == -1);
    
    semID = semget(key, 16, O_CREAT | 0660);
    
    for(i = 0; i < 16; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = i;
        sops[i].sem_flg = 0;
    }
    semop(semID, sops, 16);

    while(1);

}

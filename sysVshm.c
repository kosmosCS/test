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
#include <sys/shm.h>

#define M_OUTPUT "output"
#define M_KEY "mem.temp"

int main(int argc, char **argv) {
    key_t key;
    int shmId;
    char *shm, *pShmData;
    size_t ssize = 1024*1024;
    struct shmid_ds data;
    int temp, i;

    data.shm_segsz = ssize;

    key = ftok(M_KEY, 1);
    
    shmId = shmget(key, ssize, 0666 | IPC_CREAT);
    if(shmId == -1)
        shmId = shmget(key, ssize, 0666);
    
    printf("key = %d\t shmid = %d\n", key, shmId);
    
    shm = shmat(shmId, NULL, 0);
    perror("shmat: ");
    
    pShmData = shm;
    for(i = 0; i < (1024*1024); i++)
        *pShmData++ = 42;
    
    pShmData = shm;
    for(i = 0; i<=10; i++)
        printf("%d\n", *pShmData++);

    return 0;
}

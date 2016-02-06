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


#define M_PATH "test.shm"
#define MAX_LEN 1024*1024
int main(int argv, char **argc) {
    int i;
    int shmId;
    struct region{
//        int len;
        char buf[MAX_LEN];
    };

    struct region *rptr;
    char *pShmData;

    if((shmId = shm_open(M_PATH, O_CREAT | 0666, 0777)) == -1) {
        shmId = shm_open(M_PATH, 0666, 0777);
    }
    printf("shmId = %d\n", shmId);
    
    if(ftruncate(shmId, sizeof(struct region)) == -1)
    perror("Ftrunc error: ");

    rptr = mmap(NULL, sizeof(struct region),
            PROT_WRITE, MAP_SHARED, shmId, 0);
    if(rptr == MAP_FAILED) perror("mmap error: ");
  //  printf("Len - %d\n", rptr->len);
    
    pShmData = rptr->buf;
    for(i = 0; i < MAX_LEN; i++)
        *pShmData++ = 13;
    
    pShmData = rptr->buf;
    for(i = 0;i < 10; i++)
	    printf("%d\n", *pShmData++);
    return 0;
}

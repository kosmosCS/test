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

#define M_OUTPUT "output"

int main(int argc, char **argv) {
   int msqid;
   key_t key = ftok("msg.temp",'b');
   struct message {
    long mtype;
    char mtext[80];
   }myMessage;
    
   struct msqid_ds buf;
    int msgsz;
    long msgtyp = 0;
    FILE *fp;
    char *str;

    fp = fopen(M_OUTPUT, "w");
  //  fputs("wertyy", fp);
    fclose(fp);

//   do {
        msqid = msgget(key, 0666 | IPC_CREAT | IPC_NOWAIT);
  // }while(!(msqid == 0));
        //  mymsgid = msgget(key, 0x1B6);
  // msgctl(msqid, IPC_STAT, &buf);
  // printf("%o\n",buf.msg_perm.mode);
   printf("%d\n", key);
   printf("%d\n", msqid);

   msgsz = 100;
   while(1) {
    if (msgrcv(msqid, (void*) &myMessage, sizeof(myMessage.mtext),
                msgtyp,MSG_NOERROR | IPC_NOWAIT) == -1) {
//        printf("q\n");
//        printf("%s\n",strerror(errno));
    } else {
        fp = fopen(M_OUTPUT, "a");
        msgctl(msqid, IPC_STAT, &buf);
        printf("%s\t %s", myMessage.mtext, asctime(gmtime(&buf.msg_rtime)));
   //     strcpy(myMessage.mtext, str);
        fputs(myMessage.mtext, fp);
        fclose(fp);
    }
//    sleep(1);
   }
   return 0;
}


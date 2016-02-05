#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

void who_wc() {
    int pfd[2];
    pipe(pfd);
    if (!fork()) {
        close(STDOUT_FILENO);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);
        close(pfd[0]);
        execlp("who","who", NULL);
    } else {
        close(STDIN_FILENO);
        dup2(pfd[0], STDIN_FILENO);
        close(pfd[1]);
        close(pfd[0]);
        execlp("wc", "wc","-l", NULL);
    }
}


int main(int argc, char **argv) {
   //who_wc();
    unsigned int i = 0, j = 0,k = 0;
    char *p, *pCom, *pArg;
    char commands[50][50],stri[200];
    char arguments[50][50];
    int cur;   
    int status;
    int pfd[2];
    char flag = 0, flag2 = 0;
    int fd;
    if((fd= open("result.out", O_WRONLY | O_CREAT | O_TRUNC,0777)) == -1) {
        chmod("result.out", S_IRWXU);
        fd= open("result.out", O_WRONLY | O_CREAT | O_TRUNC,0777);
//        printf("protected\n");
//        return 1;
    }
    p = &argv[1][0];
    pCom = &commands[0][0];
    pArg = &arguments[0][0];
    flag = 0;    
    do {
        switch(*p) {
            case '|':
                i = 0;
                j++;
                flag = 0;
                flag2 = 0;
                pCom = &commands[j][0];
                pArg = &arguments[j][0];
                break;
            case ' ':
                if (flag2) flag = 1;
                break;
            default:
                if(!flag) {
                    pCom[i] = *p;
                    i++;
                } else {
                    pArg[k] = *p;
                    k++;
                }
                flag2 = 1;
                break;
        }
    }while(*p++ != '\0');

    for(k = 0; k <= j; k++) {
    pipe(pfd);
        if (!fork()) {
             if (k != j) {
                close(STDOUT_FILENO);
                dup2(pfd[1],STDOUT_FILENO);
                close(pfd[0]); 
                close(pfd[1]);
            } else {
                close(STDOUT_FILENO);
                dup2(fd,STDOUT_FILENO);
                close(fd);
            }
            if (arguments[k][0] =='\0') {
                execlp(commands[k],commands[k], NULL);
            }
            else {
                execlp(commands[k],commands[k],arguments[k], NULL);
            }
            break;
        } else {
            wait(&status);
            if (k != j) {
                close(STDIN_FILENO);
                dup2(pfd[0],STDIN_FILENO);
                close(pfd[0]);
                close(pfd[1]);
            }
        } 

    }
    return 0;
}

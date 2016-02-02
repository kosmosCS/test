#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
	char *socket_path = "a.sock";
     int sockfd, newsockfd, portno;
     char buffer[256];
     char buf[100], buf_client[100], *str = "stroka\n";
     struct sockaddr_un addr, cli_addr;
     int fd,cl,rc;
    int temp;
	int fd_client, rc_client;
     
     pid_t child;

     child = fork();
     
     if(!child) {
         int i = 0;
         if ( (fd_client = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			perror("socket error");
			exit(-1);
		  }

		  memset(&cli_addr, 0, sizeof(cli_addr));
		  cli_addr.sun_family = AF_UNIX;
		  strncpy(cli_addr.sun_path, socket_path, sizeof(cli_addr.sun_path)-1);
          do {
            temp = connect(fd_client, (struct sockaddr*)&cli_addr, sizeof(cli_addr));
            printf("pytaus' - %d\n", temp);
            i++;
            if (i > 5) break;
          }while(temp != 0);
         
          while(1) {
            if (i < 5) {
                write(fd_client, str, 7);
                i++;
            }
         }
     } else {
		 fd = socket(AF_UNIX, SOCK_STREAM, 0);
		 memset(&addr, 0, sizeof(addr));
		 addr.sun_family = AF_UNIX;
		 strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
		 if(bind(fd, (struct  sockaddr*)&addr, sizeof(addr)) == -1) {
			perror("bind error");
			exit(-1);
		 }

		 if(listen(fd, SOMAXCONN) == -1) {
			perror("accept error");
			exit(-1);
		 }

		while (1) {
			if ( (cl = accept(fd, NULL, NULL)) == -1) {
				perror("accept error");
			continue;
			}
            printf("connected\n");
			while ( (rc=read(cl,buf,sizeof(buf))) > 0) {
				printf("read %u bytes: %.*s\n", rc, rc, buf);
			}
			if (rc == -1) {
				perror("read");
				exit(-1);
			}
			else if (rc == 0) {
				printf("EOF\n");
				close(cl);
			}
		}
     }
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

void func_child(int socket) {
	char buf[1024];
	int rc;
	while ( (rc=read(socket,buf,sizeof(buf))) > 0) {
			printf("child received %s\n", buf);
	}
}

void func_parent(int socket) {
	const char hello[] = "hello parent, I am child";
    write(socket, hello, sizeof(hello));
}

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

	int fdSockPair[2];
    static const int parentsocket = 0;
    static const int childsocket = 1;
    pid_t pid;

    /* 1. call socketpair ... */
    socketpair(PF_LOCAL, SOCK_STREAM, 0, fdSockPair);

    /* 2. call fork ... */
     
     pid_t child;

     child = fork();
     
     if(!child) {
         close(fdSockPair[parentsocket]); /* Close the parent file descriptor */
         func_child(fdSockPair[childsocket]);
     } else {
		 close(fdSockPair[childsocket]); /* Close the child file descriptor */
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
                write(fdSockPair[parentsocket],buf,rc);
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

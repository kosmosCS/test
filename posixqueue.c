#include <mqueue.h>                                                            
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

int main(int argc, char **argv){
	struct mq_attr attr;
	unsigned int r;
	char buffer[1000];
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 1000;
	attr.mq_curmsgs = 0;
	mqd_t mq = mq_open("/test.mq", O_CREAT | O_RDONLY, 0777, &attr);

	memset(&buffer, 0, sizeof(buffer));

	r = mq_receive(mq, buffer, 1000, 0);
	int f = open("/home/box/message.txt", O_RDWR);

	write(f, buffer, r);
	close(f);
	return 0;
}

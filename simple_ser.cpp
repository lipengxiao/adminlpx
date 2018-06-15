#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<arpa/inet.h>
using namespace std;


const char*ip = "127.0.0.1";
const int port = 6000;

int create_socketfd()
{
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1)
	{
		cerr<<"socket fail;errno: "<<errno<<endl;
		return -1;
	}
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);

	bind(fd,(struct sockaddr*)&saddr,sizeof(saddr));

	listen(fd,5);
	return fd;
}

void ser_run(int fd)
{
	struct sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int res = 0;
	while(1)
	{
		if((res = accept(fd,(struct sockaddr*)&caddr,&len)) < 0)
		{
			cerr<<"accept fail;errno: "<<errno<<endl;
			exit(0);
		}
		cout<<"client "<<res<<" connected!"<<endl;
		char buff[128] = {0};
		int num = recv(res,buff,127,0);
		if(num < 0)
		{
			cerr<<"client "<<res<<" err quit;errno: "<<errno<<endl;
			close(res);
			continue;
		}
		cout<<buff<<endl;
		char sendbuff[] = "ok!";
		send(res,sendbuff,strlen(sendbuff),0);
		close(res);
	}
}

int main()
{
	int fd = create_socketfd();
	if(fd == -1)
	{
		exit(0);
	}
	ser_run(fd);
	close(fd);
	return 0;
}

#include<stdio.h>
#include<arpha/inet.h>
int main(){
	int sock_fd;
	char fname[50],buffer[1024];
	struct sockaddr_in addr;
	sock_addr=socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr.sin_port=htons(7000);
	connect(sock_addr,(struct sockaddr *)&addr, sizeof(addr));
	printf("Enter filename:");
	scanf("%s",fname);
	send(sock_fd,fname,size(fname),0);
	while(recv(sock_fd,buffer,size(buffer))){
		print("%s",buffer);
	}
	close(sock_fd);
}
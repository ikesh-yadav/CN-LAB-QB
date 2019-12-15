#include<stdio.h>
#include<Winsock2.h>//linux #include<arpha/inet.h>
#include<fcntl.h>
int main(){
	int fd,sock_fd,new_sock_fd;
	char fname[50],buffer[1024];
	struct sockaddr_in addr;
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");//SHould be INET_ANY
	addr.sin_port=htons(7891);
	bind(sock_fd,(struct sockaddr *) &addr,sizeof(addr));
	printf("Server Online");
	fflush(stdin);
	listen(sock_fd,5);
	while(1){
		new_sock_fd=accept(sock_fd,NULL,NULL);
		printf("ACCEPTING CONNECTION");
		recv(new_sock_fd,fname,sizeof(fname),0);
		printf("Client requesting:%s\n",fname);
		fd=open(fname,O_RDONLY);
		if(fd<0){
			send(new_sock_fd,"file not found\n",15,0);
		}else{
			printf("STARTING TRANSFER\n");
			while(read(fd,buffer,sizeof(buffer))){	
				send(new_sock_fd,buffer,sizeof(buffer),0);
			}
		}
		close(fd);
		close(new_sock_fd);
		printf("SOCKET CLOSED\n");
	}
}
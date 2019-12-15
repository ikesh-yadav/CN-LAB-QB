#include<stdio.h>
#include<arpha/inet.h>
#include<fcntl.h>
int main(){
	int fd,sock_fd,new_sock_fd;
	char fname[50],buffer[1024];
	sruct sockaddr_in addr;
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=INET_ANY;
	addr.sin_port=htons(7000);
	bind(sock_fd,(struct sockaddr *) &addr,sizeof(addr));
	printf("Server Online");
	fflush();
	listen(sock_fd,5);
	while(true){
		new_sock_fd=accept(sock_fd,null,null);
		recv(new_sock_fd,fname,size(fname),0);
		fd=open(fname,O_RDONLY);
		if(fd<0){
			send(new_sock_fd,"file not found\n",15,0);
		}else{
			while(read(fd,buffer,size(buffer))){	
				send(new_sock_fd,buffer,size(buffer));
			}
		}
		close(fd);
		close(new_sock_fd);
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char*);
char webpage[] = "HTTP/1.1 200 OK\r\n"
				 "Server:Linux Web Server\r\n"
				 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
				 "<!DOCTYPE html>\r\n"
				 "<html><head><title> My Web Page </title>\r\n"
				 "<style>body {background-color: #FFFF00 } </style></head>\r\n"
				 "<body><center><h1>Hello World!!</h1><br>\r\n"
				 "<img src=\"game.jpg\"></center></body></html>\r\n";


int main(int argc, char* argv[]) {
	int serv_sock;
	int clnt_sock;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr, clnt_adr;
	int str_len, i;
	socklen_t clnt_adr_sz;
	
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock==-1) error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if (listen(serv_sock, 5)==-1) 
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);
	
	for (i=0; i<5; i++) {
		clnt_sock=accept(serv_sock, (struct sockaddr*)&serv_adr, &clnt_adr_sz);
		if (clnt_sock==-1) error_handling("accept() error");

		while((str_len=read(clnt_sock, webpage, BUF_SIZE)) != 0)
			write(clnt_sock, webpage, str_len);

		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
	
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

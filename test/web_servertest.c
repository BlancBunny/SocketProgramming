#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>

#define BUF_SIZE 1024

char webpage[] = "HTTP/1.1 200 OK\r\n"
				"Server:Linux Web Server\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n\r\n"
				"<!DOCTYPE html>\r\n"
				"<html><head><title> My web Page </title>\r\n"
				"<style>body {background-color: #FFFF00 }</style></head>\r\n"
				"<body><center><h1>Hello world!</h1><br>\r\n"
				"<img src=\"chrome-extension://game.jpg\"></center></body></html>\r\n";

void error_handling(char*);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	char buf[BUF_SIZE];
	int fd, fd_img;
	int read_cnt;
	int option = 1;
	socklen_t clnt_adr_sz;
	char buf_img[700000];
	FILE* fp;

	if (argc != 2) {
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, 4);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 20)==-1)
		error_handling("listen() error");
	
	while(1) {
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		puts("New client connected...");
	
		read(clnt_sock, buf, 2047);
		printf("%s\n", buf);

		if (!strncmp(buf, "GET /game.jpg", 13)) {
			fd_img = open("game.jpg", O_RDONLY);
			read(fd_img, buf_img, sizeof(buf_img));
			// read(clnt_sock, buf_img, 700000);
			write(clnt_sock, buf_img, sizeof(buf_img));

			close(fd_img);
		}
	
		else write(clnt_sock, webpage, sizeof(webpage)-1);
	
	}
		close(clnt_sock); close(serv_sock);
		return 0;
}

void error_handling(char* message) 
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

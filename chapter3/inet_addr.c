#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	char* addr1="254.253.252.251";
	char* addr2="1,2,3,256";

	unsigned long conv_addr=inet_addr(addr1);
	if(conv_addr==INADDR_NONE)
		printf("error occured!\n");
	else printf("network ordered interget addr : %#lx\n", conv_addr);

	conv_addr = inet_addr(addr2);
	if(conv_addr==INADDR_NONE)
		printf("error occured!\n");
	else printf("network ordered integer addr: %#lx\n", conv_addr);

	return 0;
}

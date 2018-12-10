// server.c
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int sock;
int client;

void handler(int sig);

void handler(int sig) {
	close(sock);
	close(client);
	exit(0);
}

int main(int argc, char* argv[]) {
	// ipv4, tcp, default attributes
	sock = socket(AF_INET, SOCK_STREAM, 0);
	signal(SIGINT, handler);
	struct sockaddr_in my_address;
	my_address.sin_family = AF_INET;
	// host byte ordering to network byte ordering
	my_address.sin_port = htons(9999);	
	printf("Port is: %d\n", my_address.sin_port);
	my_address.sin_addr.s_addr = INADDR_ANY;

	// Error Checking
	if (sock == -1) {
		printf("Encountered an error creating socket. Exiting. \n");
		exit(1);
	}
	
	// Bind our socket to the address
	if (bind(sock, (struct sockaddr*) &my_address,
		 sizeof (struct sockaddr_in)) == -1) {
		printf("Error binding the socket. \n");
		close(sock);
		exit(1);
	}

	// Start listening
	if (listen(sock, 10) == -1) {
		printf("Listen errored, exiting. \n");
		close(sock);
		exit(1);
	}
	
	// Lets accept a connection
	client = accept(sock, NULL, NULL);
	char * input = calloc(100, sizeof(char));
	char data[256] = {};
	size_t readin = 100;
	while (1) {
		// read data from the client.
		read(client, data, 256);
		printf("Server recvd: %s\n", data);
		getline(&input, &readin, stdin);
		write(client, input, strlen(input) + 1);
	}
	close(sock);
	close(client);
	return 0;
}

// client.c
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int sock;

void handler(int sig);

void handler(int sig) {
	close(sock);
	exit(0);
}

int main(int argc, char* argv[]) {
	// ipv4, tcp, default attributes
	sock = socket(AF_INET, SOCK_STREAM, 0);
	signal(SIGINT, handler);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	// host byte ordering to network byte ordering
	server.sin_port = htons(9999);	

	struct hostent * host;
	host = gethostbyname(argv[1]);

	if (!host) {
		printf("Host not found, exiting\n");
		exit(1);
	}

	// Error Checking
	if (sock == -1) {
		printf("Encountered an error creating socket. Exiting.\n");
		exit(1);
	}

	// Copy host information to our address.
	memcpy(&server.sin_addr, host->h_addr, host->h_length);



	
	// Bind our socket to the address
	if (connect(sock, (struct sockaddr*) &server,
		 sizeof (struct sockaddr_in)) == -1) {
		printf("Error connecting the socket. \n");
		close(sock);
		exit(1);
	}

	// Lets accept a connection
	char * input = calloc(100, sizeof(char));
	char data[256] = {};
	size_t readin = 100;
	while (1) {
		// read from keyboard, send data to the server
		getline(&input, &readin, stdin);
		write(sock, input, strlen(input) + 1);
		read(sock, data, 256);
		printf("client recvd: %s\n", data);
	}
	close(sock);
	return 0;
}

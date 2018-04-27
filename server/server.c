#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "date.h"
#include "update.h"

#define PORT 8888

int main()
{
	struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sock_desc;
    int client_sock;
  	int *sock;
    socklen_t addr_size;
    char buffer[500];
    pid_t childpid;

	// 2 - Create the socket
	sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_desc < 0)
	{
		printf("\nError creating socket.\n");
		exit(1);
	}
	printf("\nServer socket has been created.\n");

	// 3 - Initialise the socket
	//memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// 4 - Bind the socket
	if(bind(sock_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("\nError binding server socket");
		exit(1);
	}
	printf("\nSocket Binding to port %d successful\n", 8888);

	// 5 - Listen for client connections
	if(listen(sock_desc, 10) == 0)
	{
		printf("\nWaiting for client connections...\n");
	}
	else
	{
		printf("\n[-]Error in binding.\n");
	}

	//infinite loop to continuously listen for connections
	while(1)
	{
		// 6 - Accept a clients connection
		client_sock = accept(sock_desc, (struct sockaddr*)&client_addr, &addr_size);
		if(client_sock < 0)
		{
			exit(1);
		}
		// print client details using inet_ntoa() and inet_ntohs() - converts from network byte order to string
		printf("\nConnection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		pthread_t thread;
		sock = malloc(1);
		*sock = client_sock;

		if(pthread_create(&thread, NULL, update, (void*) sock) < 0)
		{
			perror ("\nThread not created");
			return 1;

		}
	}

	close(client_sock);

	return 0;
}



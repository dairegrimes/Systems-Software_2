#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888

int main(){

	//initialise client/server variables
	int client_sock;
	struct sockaddr_in server_addr;
	char buffer[500];
	char username [10];
	char password [10];
	char response [10];
	char reply [10];
	int choice = 0;

	//create the client socket
	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(client_sock < 0)
	{
		printf("\nError creating client socket\n");
		exit(1);
	}
	printf("\nClient socket successfully created\n");

	//initialise server address variables
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// connect to the server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("\nError connecting to server");
		exit(1);
	}
	printf("\nSuccessfully Connected to Server\n");
	send (client_sock, "INIT_LOGIN", strlen ("INIT_LOGIN"), 0);

	printf("\nUsername: ");
	scanf("%s", username);
	send (client_sock, username, strlen (username), 0);

	printf("\nPassword: ");
	scanf("%s", password);
	send (client_sock, password, strlen (password), 0);

	recv (client_sock , reply, 10, 0);
	if(strcmp (reply, "200") == 0)
	{
		printf("\nSuccessfully logged in");
	}

	else
	{
		printf("\nFailed to login\n");
		return 1;
	}


	//infinite while loop to continuously communicate with server
	while(1)
	{
		printf("\n1 : Transfer");
		printf("\n2 : Exit");
		printf("\n: ");
		scanf("%d", &choice);

		switch (choice)
		{
			case 1:
			{
				printf("\nStarting transfer");
				send (client_sock, "TRANSFER", strlen ("TRANSFER"), 0);
			}break;

			case 2:
			{
				exit(0);

			}break;
			default:
			{
				printf ("\nInvalid Entry");
			}
		}


		//send data to server
		printf("Client: ");
		scanf("%s", &buffer[0]);
		send(client_sock, buffer, strlen(buffer), 0);

		if(strcmp(buffer, "exit") == 0){
			close(client_sock);
			printf("Disconnected from server\n");
			exit(1);
		}

		//read data from server
		if(recv(client_sock, buffer, 1024, 0) < 0){
			perror("\nError reading server data");
		}else{
			printf("\nServer: %s\n", buffer);
		}
	}

	return 0;
}
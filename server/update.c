#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "date.h"
#include "update.h"

#define CRED_PATH "/home/daire/Documents/Assignment2/server/logs/credentials.txt"
#define LOG_PATH "/home/daire/Documents/Assignment2/server/logs/logs.txt"
#define PATH_SIZE 300
#define MESSAGE_SIZE 100
#define SIZE 20



void *update(void *socket_desc)
{
	pthread_mutex_t lock;
	char client_message [MESSAGE_SIZE];
	int sock = *(int*)socket_desc;
	int is_password_correct = 0;
	char username [SIZE];
	char password [SIZE];
	char response [SIZE];
	int readSize;
	int bytes;


	char stored_username [SIZE];
	char stored_password [SIZE];


	while(readSize = recv (sock, client_message, MESSAGE_SIZE, 0) > 0)
	{
		if(strcmp (client_message, "INIT_LOGIN") == 0)
		{
			printf("\nLogin Initialised");

			recv(sock, username, 50, 0);
			recv(sock, password, 50, 0);

			pthread_mutex_lock (&lock);

			FILE *credential_file = fopen(CRED_PATH, "r");
			if(credential_file == NULL)
			{
				printf("\nCredentials File was not opened");
				exit(1);
			}

			char line [100];

			while(fgets (line, 100, credential_file))
			{
				sscanf (line, "username: %s password: %s", stored_username, stored_password);

				if( (strcmp (username, stored_username) == 0) &&
					(strcmp (password, stored_password) == 0))
				{
					is_password_correct = 1;
					printf("\nCORRECT");
				}
			}



			fclose(credential_file);
			pthread_mutex_unlock (&lock);

			if(is_password_correct == 1)
			{
				send(sock, "200", sizeof ("200"), 0);
			}

			else
			{
				send (sock, "401", sizeof ("401"), 0);
				printf("\nUsername or password incorrect");
				free (socket_desc);
				pthread_exit (NULL);
			}

			printf("\nLogin Completed");
		}
	}

}
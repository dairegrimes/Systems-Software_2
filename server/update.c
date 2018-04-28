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

#define CRED_PATH "logs/credentials.txt"
#define LOG_PATH "logs/logs.txt"
#define WEBSITE_DIR "intranet/"
#define PATH_SIZE 300
#define MESSAGE_SIZE 500
#define SIZE 20



void *update(void *socket_desc)
{
	char *log_seperation = "\n------------------------------------\n";
	pthread_mutex_t lock;
	char client_message [MESSAGE_SIZE];
	char server_path [PATH_SIZE];
	int sock = *(int*)socket_desc;
	int is_password_correct = 0;
	char username [SIZE];
	char password [SIZE];
	char file_name [SIZE];
	char file_path [SIZE];
	char buffer [PATH_SIZE];
	char log_information[500];
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
				}
			}



			fclose(credential_file);
			pthread_mutex_unlock (&lock);

			if(is_password_correct == 1)
			{
				send(sock, "200", sizeof ("200"), 0);
				printf("\nLogin Completed");
			}

			else
			{
				send (sock, "401", sizeof ("401"), 0);
				printf("\nUsername or password incorrect\n");
				free (socket_desc);
				pthread_exit (NULL);
			}

			if( strcmp (client_message, "TRANSFER") == 0)
			{
				printf("\nTransfer Started");
			}


			if((bytes = recv (sock, file_name, MESSAGE_SIZE, 0)) == 0)
			{
				printf("\nNo file name received");
			}

			if((bytes = recv (sock, file_path, MESSAGE_SIZE, 0)) == 0)
			{
				printf("\nNo file path received");
			}
			

			strcpy (server_path, WEBSITE_DIR);
			strcat (server_path, file_path);
			strcat (server_path, file_name);

			pthread_mutex_lock (&lock);


			FILE *file = fopen (server_path, "w");
			memset (buffer, 0, sizeof (buffer));
			bytes = 0;
			int i = 0;

			while(bytes = recv (sock, buffer, PATH_SIZE, 0) > 0 )
			{
				printf("\nData received %d = %d\n", i, bytes);
				int write_sz = fwrite(buffer, sizeof (char), bytes, file);
				memset (buffer, 0, sizeof (buffer));
				i ++;
				if( write_sz == 0 || write_sz != 512)
				{
					break;
				}
			}

			fclose (file);
			printf ("Transfer Complete");

			send (sock, "Complete", sizeof ("Complete"), 0);


			char date_buffer [50];
			char * date = returnDate (date_buffer);


			strcat (log_information, log_seperation);
			strcat (log_information, "Username: ");
			strcat (log_information, username);
			strcat (log_information, "\nFile changed: ");
			strcat (log_information, file_path);
			strcat (log_information, file_name);
			strcat (log_information, "\nTime and Date: ");
			strcat (log_information, date);
			strcat (log_information, log_seperation);

			FILE *log_file = fopen (LOG_PATH, "ab+");

			if(log_file == NULL)
			{
				printf ("\nCould not open file\n");
				exit (0);
			}

			else
			{
				fwrite (log_information, 1, sizeof (log_information), log_file);
			}

			fclose (log_file);
			memset (client_message, 0, sizeof (log_information));

			pthread_mutex_unlock (&lock);
		}

		memset (client_message, 0, sizeof (client_message));
	}

	if(readSize == 0)
	{
		printf ("\nClient disconnected\n");
		fflush (stdout);
	}

	else if (readSize == -1)
	{
		printf ("\nRecv Failed\n");
	}


	free (socket_desc);
	pthread_exit (NULL);
}
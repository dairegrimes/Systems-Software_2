#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define WEBSITE_DIR "file/"

int main()
{
	//initialise client/server variables
	int client_sock;
	struct sockaddr_in server_addr;
	char buffer[500];
	char username [10];
	char password [10];
	char response [10];
	char message [500];
	char file_directory [500];
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
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
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
	system ("clear");
	if(strcmp (reply, "200") == 0)
	{
		printf("\nSuccessfully logged in");
	}

	else
	{
		printf("\nFailed to login\n");
		return 1;
	}



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
				send (client_sock, "TRANSFER", strlen ("TRANSFER"), 0);
				system ("clear");
				printf("\nStarting transfer\n");

				printf ("\nEnter a file name: ");
				scanf ("%s", message);
				strcpy (file_directory, WEBSITE_DIR);
				strcat (file_directory, message);

				if(access (file_directory, F_OK) < 0)
				{
					printf ("\nThere is no file\n");
					return 1;
				}

				send (client_sock, message, strlen (message), 0);





				printf("\n1 : Root Directory");
				printf("\n2 : Sales");
				printf("\n3 : Promotions");
				printf("\n4 : Offers");
				printf("\n5 : Marketing");
				printf("\n: ");
				scanf("%d", &choice);

				switch (choice)
				{
					case 1:
					{
						send (client_sock, "root/", strlen ("root/"), 0);
					}break;

					case 2:
					{
						send (client_sock, "sales/", strlen ("sales/"), 0);
					}break;

					case 3:
					{
						send (client_sock, "promotions/", strlen ("promotions/"), 0);
					}break;

					case 4:
					{
						send (client_sock, "offers/", strlen ("offers/"), 0);
					}break;

					case 5:
					{
						send (client_sock, "marketing/", strlen ("marketing/"), 0);
					}break;

					default:
					{
						printf ("\nInvalid Entry");
					}
				}

				char file_buffer [500];
				memset (file_buffer, 0, sizeof (file_buffer));

				int block_size = 0;
				int i = 0;
				printf ("\nSending %s to server... \n", file_directory);
				FILE *file_open = fopen (file_directory, "r");

				while (block_size = fread (file_buffer, sizeof (char), 500, file_open) > 0)
				{
					printf ("\nData sent %d = %d\n", i, block_size);
					if(send (client_sock, file_buffer, block_size, 0) < 0)
					{
						return 1;
					}
					memset (file_buffer, 0, sizeof (file_buffer));
					i ++;
				}

				printf ("\nFile sent");

				recv (client_sock, reply, 10, 0);

				if(strcmp (reply, "Complete") == 0)
				{
					printf ("\nTransfer Completed");
				}

				else
				{
					printf ("\n Transfer not Completed\n");
					close(client_sock);
					return 1;
				}

				fclose (file_open);
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
	}
}
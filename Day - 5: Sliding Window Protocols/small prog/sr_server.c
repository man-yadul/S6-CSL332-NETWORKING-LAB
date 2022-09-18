#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define SA struct sockaddr
#define PORT 8080

void main()
{
	srand(time(0));

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		printf("Socket creation failed.\n");
		exit(0);
	}
	else
		printf("Socket created successfully.\n");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("Bind failed.\n");
		exit(0);
	}
	else 
		printf("Bind successful.\n");

	if (listen(sockfd, 5) != 0)
	{
		printf("Listen failed.\n");
		exit(0);
	}
	else
		printf("Listen successful\n");

	len = sizeof(cliaddr);

	connfd = accept(sockfd, (SA *)&cliaddr, &len);

	if (connfd < 0)
	{
		printf("Connection failed.\n");
		exit(0);
	}
	else
		printf("Connection successful.\n");

	int windowsize = 4;
	int start = 0;
	int end = 3;
	int ch;
	int err_frame;

	while (1)
	{
		ch = rand() % 2;
		write(connfd, &ch, sizeof(ch));

		switch (ch)
		{
			case 0:	

					for (int i = start; i <= end; i++)
					{
						printf("Frame %d transmitted successfully.\n", i);
						sleep(1);
						printf("Frame %d acknowledged successfully.\n", i);
						sleep(1);
					}

					break;

			case 1:	err_frame = (rand() % (start - end + 1)) + start;
					write(connfd, &err_frame, sizeof(err_frame));

					for (int i = start; i <= end; i++)
					{
						if (i == err_frame)
						{
							printf("Frame %d acknowledgement not received.\n", i);
							sleep(1);
						}
						else
						{
							printf("Frame %d transmitted successfully.\n", i);
							sleep(1);
							printf("Frame %d acknowledged.\n", i);
							sleep(1);
						}
					}

					printf("Resending frame %d\n", err_frame);
					printf("Frame acknowledged.\n");
					sleep(1);

					break;
		}

		start = end + 1;		
		end = end + windowsize - 1;
	}
}

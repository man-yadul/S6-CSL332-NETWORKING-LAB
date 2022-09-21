#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SA struct sockaddr
#define PORT 8080

void main()
{
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Connection failed.\n");
		exit(0);
	}
	else
		printf("Connection successful.\n");

	int ch;
	int start = 0;
	int windowsize = 4;
	int end = 3;
	int err_frame;

	while (1)
	{
		read(sockfd, &ch, sizeof(ch));

		switch (ch)
		{
			case 0: for (int i = start; i <= end; i++)
					{
						printf("Frame %d received successfully.\n", i);
						sleep(1);
						printf("Frame %d acknowledged successfully.\n", i);
						sleep(1);
					}
					break;

			case 1:	read(sockfd, &err_frame, sizeof(err_frame));

					for (int i = start; i <= end; i++)
					{
						if (i == err_frame)
						{
							printf("Frame %d acknowledgement destroyed.\n", i);
							sleep(1);
						}
						else
						{
							printf("Frame %d received successfully.\n", i);
							sleep(1);
							printf("Frame %d acknowledged.\n", i);
							sleep(1);
						}
					}

					printf("Resent frame acknowledgemed.\n");
					sleep(1);
					break;
		}

		start = end + 1;		
		end = end + windowsize - 1;
	}
}

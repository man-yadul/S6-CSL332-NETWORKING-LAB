#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
struct timeval timeout;
void func(int sockfd, int nf, int ws)
{
	char buff[MAX];
	int ack, i = 0, n, k;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
		perror("setsockopt(SO_RCVTIMEO) failed");
	printf("Frame %d sent\n", i);
	bzero(buff, sizeof(buff));
	snprintf(buff, sizeof(buff), "%d", i);
	k = send(sockfd, buff, sizeof(buff), 0);
	while (1)
	{
		bzero(buff, sizeof(buff));
		n = recv(sockfd, buff, MAX, 0);
		ack = atoi(buff);
		if (n > 0)
		{
			if (ack == nf)
			{
				printf("Acknowlegement received: %d\nExit\n", ack);
				bzero(buff, sizeof(buff));
				strcpy(buff, "Exit");
				k = send(sockfd, buff, sizeof(buff), 0);
				break;
			}
			if (ack == i + 1)
			{
				i++;
				printf("Acknowlegement received: %d\nFrame %d sent\n", i, ack);
				bzero(buff, sizeof(buff));
				snprintf(buff, sizeof(buff), "%d", i);
				k = send(sockfd, buff, sizeof(buff), 0);
			}
		}
		else
		{
			printf("Acknowledgement %d not received\nResending frame %d\n", i + 1, i);
			bzero(buff, sizeof(buff));
			snprintf(buff, sizeof(buff), "%d", i);
			k = send(sockfd, buff, sizeof(buff), 0);
		}
	}
}
void main()
{
	int sockfd, connfd, f, w;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Socket creation failed\n");
		exit(0);
	}
	else
		printf("Socket successfully created\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("Connection with the server failed\n");
		exit(0);
	}
	else
		printf("Connected to the server\n");
	printf("Enter the number of frames: ");
	scanf("%d", &f);
	func(sockfd, f, 1);
	close(sockfd);
}

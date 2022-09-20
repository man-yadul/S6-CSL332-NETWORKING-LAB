#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SA struct sockaddr
#define PORT 8080

void main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("Socket creation failed.\n");
		exit(0);
	}
	else
		printf("Socket created successfully.\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Bind failed.\n");
		exit(0);
	}
	else
		printf("Bind successful.\n");

	if (listen(sockfd, 5) < 0)
	{
		printf("Listen failed.\n");
		exit(0);
	}
	else
		printf("Listen successful.\n");

	len = sizeof(cliaddr);

	connfd = accept(sockfd, (SA *)&cliaddr, &len);
	if (connfd < 0)
	{
		printf("Connection failed.\n");
		exit(0);
	}
	else
		printf("Connection successful.\n");

	char command[100];
	printf("Enter HELO command at client side to start communication with server.\n");
	read(connfd, command, sizeof(command));
	if (strcmp(command, "HELO\n") == 0)
		printf("HELO command received.\nCommunication established.\n");

	char sender_mail_addr[100];
	char recipient_mail_addr[100];
	char body[200];

	while (1)
	{
		read(connfd, command, sizeof(command));
	
		if (strcmp(command, "MAIL FROM\n") == 0)
			read(connfd, sender_mail_addr, sizeof(sender_mail_addr));
		else if (strcmp(command, "RCPT TO\n") == 0)
			read(connfd, recipient_mail_addr, sizeof(recipient_mail_addr));
		else if (strcmp(command, "DATA\n") == 0)
			read(connfd, body, sizeof(body));
		else if (strcmp(command, "QUIT\n") == 0)
			exit(0);
		else
			printf("Invalid command.\n");

		if (strcmp(sender_mail_addr, "") != 0 && strcmp(recipient_mail_addr, "") != 0 && strcmp(body, "") != 0)
		{
			printf("\nMail received.\n");
			printf("Sender mail address: %s", sender_mail_addr);
			printf("Recipient mail address: %s", recipient_mail_addr);
			printf("Body:\n%s\n", body);

			strcpy(sender_mail_addr, "");
			strcpy(recipient_mail_addr, "");
			strcpy(body, "");
		}
	}
}

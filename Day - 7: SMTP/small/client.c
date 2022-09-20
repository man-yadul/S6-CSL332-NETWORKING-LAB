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
	servaddr.sin_addr.s_addr = inet_addr("127.0.01");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Connection failed.\n");
		exit(0);
	}
	else
		printf("Connection successful.\n");

	char command[100];
	printf("Enter HELO command to start communication with server: ");
	fgets(command, sizeof(command), stdin);
	write(sockfd, command, sizeof(command));
	if (strcmp(command, "HELO\n") == 0)
		printf("HELO command sent.\nCommunication established.\n");

	char sender_mail_addr[100];
	char recipient_mail_addr[100];
	char body[200];
	
	while (1)
	{
		printf("\n1. MAIL FROM - Define sender mail address\n");
		printf("2. RCPT TO - Define receiver's mail address\n3. DATA - Define body of the mail\n");
		printf("Enter a command: ");
		fgets(command, sizeof(command), stdin);
		write(sockfd, command, sizeof(command));
		
		if (strcmp(command, "MAIL FROM\n") == 0)
		{
			printf("Enter sender's mail address: ");
			fgets(sender_mail_addr, sizeof(sender_mail_addr), stdin);
			write(sockfd, sender_mail_addr, sizeof(sender_mail_addr));
		}
		else if (strcmp(command, "RCPT TO\n") == 0)
		{
			printf("Enter receiver's mail address: ");
			fgets(recipient_mail_addr, sizeof(recipient_mail_addr), stdin);
			write(sockfd, recipient_mail_addr, sizeof(recipient_mail_addr));
		}
		else if (strcmp(command, "DATA\n") == 0)
		{
			printf("Enter mail body: ");
			fgets(body, sizeof(body), stdin);
			write(sockfd, body, sizeof(body));
		}
		else if (strcmp(command, "QUIT\n") == 0)
			exit(0);
		else
			printf("Invalid command.\n");

		if (strcmp(sender_mail_addr, "") != 0 && strcmp(recipient_mail_addr, "") != 0 && strcmp(body, "") != 0)
		{
			printf("Mail sent.\n");
			
			strcpy(sender_mail_addr, "");
			strcpy(recipient_mail_addr, "");
			strcpy(body, "");
		}
	}
}

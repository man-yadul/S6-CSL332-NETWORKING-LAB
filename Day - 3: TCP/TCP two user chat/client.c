#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created.\n");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection with the server failed.\n");
        exit(0);
    }
    else
        printf("Connected to the server.\n");

    char buf1[100], buf2[100];

    while (1)
    {
        printf("Enter message to send to server: ");
        scanf("%s", buf1);
        write(sockfd, buf1, sizeof(buf1));
        read(sockfd, buf2, sizeof(buf2));
        printf("Message from server: %s\n", buf2);
        // break;
    }

    close(sockfd);
}

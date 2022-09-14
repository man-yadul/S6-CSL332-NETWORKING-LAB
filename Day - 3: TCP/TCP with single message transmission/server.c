#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    char buff[100];
    char str[] = "string";

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("Socket bind failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully binded.\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed.\n");
        exit(0);
    }
    else
        printf("Server listening.\n");

    len = sizeof(cli);

    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("Server accept failed.\n");
        exit(0);
    }
    else
        printf("Client accepted by server.\n");

    read(connfd, buff, sizeof(buff));
    printf("%s\n", buff);
    write(connfd, str, sizeof(str));

    close(sockfd);
}

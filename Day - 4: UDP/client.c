#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr

// Driver code
int main()
{
    int sockfd;
    char buffer[MAXLINE];
    char str[100];
    printf("Enter message to send: ");
    scanf("%s", str);

    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int n, len;

    // sendto(sockfd, (const char *)str, strlen(str), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    sendto(sockfd, str, sizeof(str), MSG_CONFIRM, (SA *)&servaddr, sizeof(servaddr));
    printf("Message sent.\n");

    n = recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (SA *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}

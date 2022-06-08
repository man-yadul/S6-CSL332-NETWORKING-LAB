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

char username[MAX];
char password[MAX];
char text[MAX];

void func(int sockfd)
{
    int n;

    // Read username and password and send to server
    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    write(sockfd, username, sizeof(username));
    write(sockfd, password, sizeof(password));

    // Read authentication response from server
    read(sockfd, text, sizeof(text));

    if (strcmp(text, "n") == 0)
        exit(0);

    // Enter a string and send to server
    printf("Enter string: ");
    scanf("%s", text);
    write(sockfd, text, sizeof(text));

    // Receive reversed string from server and print it
    read(sockfd, text, sizeof(text));
    printf("SERVER - %s\n", text);

    exit(0);
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("Socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created.\n");

    bzero(&servaddr, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection with the server failed.\n");
        exit(0);
    }
    else
        printf("Connected to the server.\n");

    // Function for chat
    func(sockfd);

    // Close the socket
    close(sockfd);
}

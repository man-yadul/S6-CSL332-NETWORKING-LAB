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

char username[MAX];
char password[MAX];
char text[MAX];

// Function designed for chat between client and server.
void func(int connfd)
{
    // Read the username and password from client and print
    read(connfd, username, sizeof(username));
    read(connfd, password, sizeof(password));
    printf("CLIENT - Username: %s\n", username);
    printf("CLIENT - Password: %s\n", password);

    printf("Authenticate? - Enter 'y' or 'n': ");
    scanf("%s", text);

    // Send authentication message
    write(connfd, text, sizeof(text));
    if (strcmp(text, "n") == 0)
        exit(0);

    // Read next message from client, reverse it, and resend it back to client
    read(connfd, text, sizeof(text));

    char temp;
    int i = 0;
    int j = strlen(text) - 1;

    while (i < j)
    {
        temp = text[i];
        text[i] = text[j];
        text[j] = temp;
        i++;
        j--;
    }

    write(connfd, text, sizeof(text));
    
    exit(0);
}

// Driver function
void main()
{
    int sockfd, connfd, len;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("Socket bind failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully binded.\n");

    // Now server is ready to listen and verify
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed.\n");
        exit(0);
    }
    else
        printf("Server listening.\n");

    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("Server accept failed.\n");
        exit(0);
    }
    else
        printf("Client accepted by server.\n");

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);
}

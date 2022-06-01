#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void main()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        perror("Error creating process.\n");
    }

    if (pid == 0)
    {
        printf("CHILD: This is the child process.\n");
        printf("CHILD: Parent's PID: %d\n", getppid());
        printf("CHILD: Child's PID: %d\n", getpid());
        execlp("pwd", "pwd", NULL);
    }

    if (pid > 0)
    {
        wait(NULL);
        printf("Child process complete.\n ");
        printf("PARENT: This is the parent process.\n");
        printf("PARENT: Parent's PID: %d\n", getpid());
        printf("PARENT: Child's PID: %d\n", pid);
    }

    exit(0);
}

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>

void main()
{
    char buf, source[10], destination[10];
    int f1, f2;

    printf("Enter name of source file: ");
    scanf("%s", source);
    printf("Enter name of destination file: ");
    scanf("%s", destination);
    
    // Open source file
    f1 = open(source, O_RDONLY);

    if (f1 == -1)
    {
        printf("Error opening source file.\n");
        close(f1);
        return;
    }

    // Open destination file
    f2 = open(destination, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (f2 == -1)
    {
        printf("Error opening destination file.\n");
        close(f2);
        return;
    }

    // Write contents of source to destination
    while (read(f1, &buf, 1))
        write(f2, &buf, 1);

    printf("Copy successful.\n");

    close(f1);
    close(f2);
}

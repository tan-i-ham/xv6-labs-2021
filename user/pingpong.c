#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // the array p is used to store the file descriptors for the pipe
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);

    int child_pid = fork();
    if (child_pid > 0)
    {
        // printf("in parent process \n");
        close(fd1[0]); // close the read end of the pipe (parent process will write to the pipe)
        close(fd2[1]); // close the write end of the pipe (child process will read from the pipe)

        char buf[4];
        write(fd1[1], "A", 1);
        read(fd2[0], buf, 4);
        printf("%d: received pong\n", getpid());
    }
    else if (child_pid == 0)
    {
        // printf("in child process \n");
        close(fd1[1]); // close the write end of the pipe (parent process will read from the pipe)
        close(fd2[0]); // close the read end of the pipe (child process will write to the pipe)

        char buf[4];
        read(fd1[0], buf, 4);
        printf("%d: received ping\n", getpid());
        write(fd2[1], buf, 4);

        exit(0);
    }
    else
    {
        // printf("fork failed\n");
        exit(1);
    }

    exit(0);
}
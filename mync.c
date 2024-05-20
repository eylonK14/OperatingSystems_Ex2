#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define PROGRAM 2

#define PIPE_READ 0
#define PIPE_WRITE 1

#if 0
static void fd_copy(int ifd, int ofd)
{
    char buffer[1024];
    ssize_t rbytes;
    while ((rbytes = read(ifd, buffer, sizeof(buffer))) > 0)
    {
        ssize_t wbytes;
        if ((wbytes = write(ofd,buffer, (size_t)rbytes)) != rbytes)
            err_syserr("short write %zd bytes instead of %zd expected: ", wbytes, rbytes);
    }
}

static void be_childish(char *cmd[], int *to_child, int *to_parent)
{
    if (dup2(to_child[0], STDIN_FILENO) != STDIN_FILENO)
        err_syserr("failed to duplicate pipe to standard input: ");
    if (dup2(to_parent[1], STDOUT_FILENO) != STDOUT_FILENO)
        err_syserr("failed to duplicate pipe to standard output: ");
    close(to_child[1]);
    close(to_child[1]);
    close(to_parent[0]);
    close(to_parent[0]);
    // char *cmd[] = { "md5sum", 0 };
    execvp(cmd[0], cmd);
    // err_syserr("failed to execute command %s: ", cmd[0]);
    /*NOTREACHED*/
}

static void be_parental(char *cmd[], const char *fname, int *to_child, int *to_parent)
{
    close(to_child[0]);
    close(to_parent[1]);
    int fd = open(fname, 0);
    if (fd < 0) 
        err_syserr("failed to open file '%s' for reading: ", fname);
    fd_copy(fd, to_child[1]);
    close(fd);
    close(to_child[1]);

    char buffer[128];
    ssize_t rbytes = read(to_parent[0], buffer, sizeof(buffer));
    close(to_parent[0]);
    if (rbytes <= 0)
        err_syserr("read error (or unexpected EOF) from hash process: ");
    buffer[strspn(buffer, "0123456789abcdefABCDEF")] = '\0';
    printf("%s: MD5 %s\n", fname, buffer);
}

void execute_command(char *cmd[], int input_fd, int output_fd)
{
    if (fork() == 0)
    {
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(cmd[0], cmd);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}
#endif

int createChild(const char *szCommand, char *const aArguments[])
{
    int aStdinPipe[2];
    int aStdoutPipe[2];
    int nChild;
    char nChar;
    int nResult;

    if (pipe(aStdinPipe) < 0)
    {
        perror("allocating pipe for child input redirect");
        return -1;
    }
    if (pipe(aStdoutPipe) < 0)
    {
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        perror("allocating pipe for child output redirect");
        return -1;
    }

    nChild = fork();
    if (0 == nChild)
    {
        // child continues here

        // redirect stdin
        if (dup2(aStdinPipe[PIPE_READ], STDIN_FILENO) == -1)
        {
            exit(errno);
        }

        // redirect stdout
        if (dup2(aStdoutPipe[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            exit(errno);
        }

        // redirect stderr
        // if (dup2(aStdoutPipe[PIPE_WRITE], STDERR_FILENO) == -1)
        // {
        //     exit(errno);
        // }

        // all these are for use by parent only
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);

        // run child process image
        // replace this with any exec* function find easier to use ("man exec")
        nResult = execvp(szCommand, aArguments);

        // if we get here at all, an error occurred, but we are in the child
        // process, so just exit
        exit(nResult);
    }
    else if (nChild > 0)
    {
        // parent continues here

        // close unused file descriptors, these are for child only
        close(aStdinPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);

        // Include error check here
        // if (NULL != szMessage) {
        //   write(aStdinPipe[PIPE_WRITE], szMessage, strlen(szMessage));
        // }

        while (1)
        {
            // Just a char by char read here, you can change it accordingly
            while (read(aStdoutPipe[PIPE_READ], &nChar, 1))
            {
                write(STDOUT_FILENO, &nChar, 1);
                close(STDOUT_FILENO);
                if (nChar == fileno(stdin))
                    break;
            }

            printf("help!\n");

            while (read(STDIN_FILENO, &nChar, 1) == 1)
            {
                write(aStdinPipe[PIPE_WRITE], &nChar, 1);
            }

            // putchar(nChar);
        }

        // done with these in this example program, you would normally keep these
        // open of course as long as you want to talk to the child
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
    }
    else
    {
        // failed to create child
        close(aStdinPipe[PIPE_READ]);
        close(aStdinPipe[PIPE_WRITE]);
        close(aStdoutPipe[PIPE_READ]);
        close(aStdoutPipe[PIPE_WRITE]);
    }
    return nChild;
}

int main(int argc, char *argv[])
{
    // usage: ./mync -e <command> <args>
    if (argc < PROGRAM)
    {
        printf("Usage: %s -e <command> <args>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *command[argc - PROGRAM + 1];
    for (int i = 0; i < argc - PROGRAM; i++)
    {
        command[i] = malloc(sizeof(char) * strlen(argv[i + PROGRAM]));
    }
    for (int i = 0; i < argc - PROGRAM; i++)
    {
        command[i] = argv[i + PROGRAM];
    }
    command[argc - PROGRAM] = NULL;

    // run the command and capture the output and the input
    createChild(command[0], command);

    // char **command = malloc(sizeof(char *) * (argc - PROGRAM));

    // for (int i = PROGRAM; i < argc; i++)
    // {
    //     command[i] = malloc(sizeof(char) * strlen(argv[i] + 1));

    //     if (command[i] == NULL)
    //     {
    //         printf("Error allocating memory!\n");
    //         exit(EXIT_FAILURE);
    //     }

    //     command[i - PROGRAM] = argv[i];
    //     puts(command[i - PROGRAM]);
    // }

    // command[argc - PROGRAM] = NULL;

    // execute_command(command, STDIN_FILENO, STDOUT_FILENO);

    return 0;
}
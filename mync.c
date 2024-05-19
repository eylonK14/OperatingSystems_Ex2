#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

int main(int argc, char *argv[])
{
	
	return 0;
}
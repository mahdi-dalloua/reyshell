#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    char command[1024];

    while (1)
    {
        printf("reyshell> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        char *command_line = command;

        while (*command_line == ' ' || *command_line == '\t')
            command_line++;

        char *end = command_line + strlen(command_line);

        while (end > command_line && (end[-1] == ' ' || end[-1] == '\t'))
            end--;

        *end = '\0';

        if (strcmp(command_line, "exit") == 0)
            break;

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            execlp(command_line, command_line, (char *)NULL);
            perror("reyshell");
            return 1;
        }

        waitpid(pid, NULL, 0);
    }

    return 0;
}
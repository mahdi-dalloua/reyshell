#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    char command[1024];

    while (1)
    {
        char *args[64];
        int argc = 0;

        printf("reyshell> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        char *token = strtok(command, " \t\n");

        while (token != NULL && argc < 63)
        {
            args[argc] = token;
            argc++;
            token = strtok(NULL, " \t\n");
        }

        args[argc] = NULL;

        if (argc == 0)
            continue;

        if (strcmp(args[0], "exit") == 0)
            break;
        if (strcmp(args[0], "cd") == 0)
        {
            char *path = args[1];

        if (path == NULL)
            path = getenv("HOME");

        if (path == NULL)
            fprintf(stderr, "reyshell: cd: HOME not set\n");
        else if (chdir(path) == -1)
            perror("reyshell: cd");

        continue;
        }

        if (strcmp(args[0], "echo") == 0)
        {
            for (int i = 1; args[i] != NULL; i++)
            {
                printf("%s", args[i]);

                if (args[i + 1] != NULL)
                    printf(" ");
            }

            printf("\n");
            continue;
        }
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            execvp(args[0], args);
            perror("reyshell");
            return 1;
        }

        waitpid(pid, NULL, 0);
    }

    return 0;
}
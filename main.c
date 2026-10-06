#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    char command[1024];
    char history[100][1024];
    int history_count = 0;

    while (1)
    {
        char *args[64];
        int argc = 0;

        printf("reyshell> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
    break;

        if (history_count < 100)
        {
            strcpy(history[history_count], command);
            history_count++;
        }


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
                if (args[i][0] == '$')
                {
                    char *value = getenv(args[i] + 1);

                    if (value != NULL)
                        printf("%s", value);
                }
                else
                {
                    printf("%s", args[i]);
                }
                if (args[i + 1] != NULL)
                    printf(" ");
                }

            printf("\n");
            continue;
        }
        if (strcmp(args[0], "clear") == 0)
        {
            printf("\033[2J\033[H");
            continue;
        }
        if (strcmp(args[0], "history") == 0)
        {
            for (int i = 0; i < history_count; i++)
                printf("%d  %s", i + 1, history[i]);

            continue;
        }

        if (strcmp(args[0], "export") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "reyshell: export: missing argument\n");
                continue;
            }

            char *equals = strchr(args[1], '=');

            if (equals != NULL)
            {
                *equals = '\0';

                if (setenv(args[1], equals + 1, 1) == -1)
                    perror("reyshell: export");

                continue;
            }

            if (args[2] != NULL && strcmp(args[2], "=") == 0 && args[3] != NULL)
            {
                if (setenv(args[1], args[3], 1) == -1)
                    perror("reyshell: export");

                continue;
            }

            fprintf(stderr, "reyshell: export: invalid format\n");
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
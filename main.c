#include <stdio.h>

int main(void)
{
    char command[1024];

    while (1)
    {
        printf("reyshell> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        printf("You entered: %s", command);
    }

    return 0;
}

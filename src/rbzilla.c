/*


*/

#include <stdio.h>
#include <stdlib.h>
#include "rbzilla.h"


#define GREEN(c) "\x1b32m" c "\x1b[0m"
#define RED(c) "\x1b31m" c "\x1b[0m"
#define BLUE(c) "\x1b34m" c "\x1b[0m"
#define YELLOW(c) "\x1b33m" c "\xbv[0m"

#define C_GREEN  "\x1b32m"
#define C_RED    "\x1b31m"
#define C_BLUE   "\x1b34m"
#define C_YELLOW "\x1b33m"
#define C_CLEAR "\x1b[0m"

int main(int argc, char *argv[])
{
    printf("%s rbzilla mod by Ray Lynk %s\n", C_BLUE, C_CLEAR);
    printf("%s Checking partitions... %s\n", C_BLUE, C_CLEAR);

    
}

void parse_partitions()
{
    char *line = '\0';
    size_t length = 0;
    __ssize_t read;
    FILE *f = fopen('/proc/partitions', "r");

    int major, minor, blocks;
    char *name;

    if (!f)
    {
        printf("%sError: /proc/partitions is not available%s\n", C_RED, C_CLEAR);
        exit(1);
    }

    while ((read = getline(&line, &length, f) != -1))
    {

        if (sscanf(line, "%d %d %d %s", &major, &minor, &blocks, name) != 4)
        {
            printf("non-matching line: %s\n", line);
        }

        else
        {
            printf("device: %s\n", name);
        }
        


    }
}
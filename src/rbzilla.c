/*


*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
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
    printf("rbzilla mod by Ray Lynk\n");
    printf("Loading disk labels..\n");
    parse_disk_labels();
    printf("Checking partitions...\n");
    parse_partitions();

    
}
void parse_disk_labels()
{
    DIR *folder;
    char fullpath[251];
    char acutalpath[255];
    struct dirent *each;

    folder = opendir("/dev/disk/by-label");

    if (folder == NULL)
    {
        printf("error: unable to read /dev/disk/by-label -- aborting.\n");
        exit(1);
    }

    while ((each=readdir(folder)))
    {
        if (each->d_name[0] == '.')
            continue;

        strcpy(fullpath, "/dev/disk/by-label/");
        strcat(fullpath, each->d_name);
        realpath(fullpath, acutalpath);

        printf("have: %s [full is %s]\n", each->d_name, acutalpath);
    }

    closedir(folder);
}

void parse_partitions()
{
    char *line = '\0';
    size_t length = 0;
    __ssize_t read;
    FILE *f = fopen("/proc/partitions", "r");

    int major, minor, blocks, part;
    char name[20], dev[20];

    if (!f)
    {
        printf("Error: /proc/partitions is not available!\n");
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
            // if we are root device and not a partition, create as disk
            if (sscanf(name, "%3s%d", dev, &part) < 2)
            {
                printf("device: %s [size: %dGb]\n", name, (blocks / 1048576));
            }
            
            else
            {
                printf("partition: %s -> %d [size: %dGb]\n", dev, part, (blocks / 1048576));
            }
            
        }
        


    }
}
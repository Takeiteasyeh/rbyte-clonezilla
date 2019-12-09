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

Disk *dheader;
Partition *pheader;
Disklabel *dlabel;

int main(int argc, char *argv[])
{
    printf("rbzilla mod by Ray Lynk\n");
    printf("Loading disk labels..\n");
    parse_disk_labels();
    printf("Checking partitions...\n");
    parse_partitions();

    
}

Disklabel create_label(char *dev, char *label)
{
    if (dlabel == NULL)
        dlabel = malloc(sizeof(Disklabel));

    // check if we are the first label
    if (strlen(dlabel->device) == 0)
    {
        strncpy(dlabel->device, dev, sizeof(dlabel->device));
        strncpy(dlabel->label, label, sizeof(dlabel->label));
        dlabel->next = NULL;
        dlabel->previous = NULL;
        return;
    }

    else
    {
        Disklabel *cycle;
        Disklabel *last;
        cycle = dlabel;

        while (cycle->next != NULL)
        {
            last = cycle;
            cycle = cycle->next;
        }

        cycle = malloc(sizeof(Disklabel));
        
    }
    

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

        char devname[4];
        sscanf(acutalpath, "/dev/%3s", devname);
        printf("%s -> %s [%s]\n", acutalpath, devname, each->d_name);
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
    short int have_source = 0;

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
            // not matching line, ignore it
            continue;
        }

        else
        {
            // if we are root device and not a partition, create as disk
            if (sscanf(name, "%3s%d", dev, &part) < 2)
            {
                // if we are smaller than 64gib assume we are a flash drive
                // and not an intended target.
                if ((blocks / 1048576) < 64)
                {
                    printf("%s - skipping flash usb\n", dev);
                    continue;
                }

                printf("device: %s [size: %dGb]\n", name, (blocks / 1048576));
            }
            
            else
            {
                printf("partition: %s -> %d [size: %dGb]\n", dev, part, (blocks / 1048576));
            }
            
        }
        


    }
}
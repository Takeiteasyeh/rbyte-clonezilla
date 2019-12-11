/*


*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include "rbzilla.h"

Disk *dheader;
Partition *pheader;
Disklabel *labelarray[10*sizeof(Disklabel)];
char source[4];
char *destination;
size_t sourcesize;
size_t destsize;
short int labelcount=0;

int multiplesource=0; // deprecate

int main(int argc, char *argv[])
{
   // source = malloc(4);
    printf("rbzilla mod by Ray Lynk\n");
    printf("Loading disk labels..\n");
    parse_disk_labels();
    printf("Checking partitions...\n");
    parse_partitions();

    if (labelcount == 0)
        printf("!! notice: no disk labels detected. 'Windows' is expected source.\n");
    
    else
        printf("found %d partition labels.\n", labelcount);

    printf("we have the following labels:\n");

    for (int i = 0; i < 10; i++)
    {
        if (labelarray[i] == NULL)
            break;

        printf("label: %s\n", labelarray[i]->label);
    }
 
}

Disklabel *create_label(char *dev, char *label)
{
    Disklabel *dlabel = malloc(sizeof(Disklabel));

    strncpy(dlabel->device, dev, sizeof(dlabel->device));
    strncpy(dlabel->label, label, sizeof(dlabel->label));

    return dlabel;

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
        printf("!! warning: unable to read /dev/disk/by-label\n");
        printf("!! warning: probably no install source, or, install source not labeled.\n");
        printf("!! warning: if using source, set label to 'Windows'\n");
        return;
    }

    int pos = 0;

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

           // check if source labeled Windows
        if (strcmp(each->d_name, "Windows"))
        {
            //printf("debug: found source start");
            if (source[0] != NULL)
            {
                printf("!!\n!! warning: source is already defined { have you already cloned? }\n!!\n");
            }

            else if (multiplesource != 0)
            {
                printf("!! warning: multiple 'Windows' partitions (have you already cloned?)\n");
            }

             else
             {
                 strcpy(source, devname);
                 printf("Found source label at %s\n", devname);
                 multiplesource = 1;
             }
         }

        Disklabel *label = malloc(sizeof(Disklabel));
            
        label = create_label(devname, each->d_name);
        labelarray[pos] = label;
        pos++;
    }

    // this is the total count of labels for show-back purposes mostly.
    labelcount = pos;

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
                if ((blocks / 1048576) < 50)
                {
                    printf("%s - skipping flash usb\n", dev);
                    continue;
                }

                printf("device: %s [size: %dGb]\n", name, (blocks / 1048576));
            }
            
            else
            {
                //printf("partition: %s -> %d [size: %dGb]\n", dev, part, (blocks / 1048576));
            }
            
        }
        


    }
}
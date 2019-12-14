/*


*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include "rbzilla.h"

#define DEBUG

Disklabel *labelarray[10*sizeof(Disklabel)];
char source[4];
char destination[4];
char sourcetype[100];
char desttype[100];
int sourcesize;
int destsize;
short int labelcount=0;

int main(int argc, char *argv[])
{
   // source = malloc(4);
   start_color(BLUE);
    printf("rbzilla mod by Ray Lynk - rlynk@bacon.place\n");
    start_color(RESET);
    printf("Parsing disk(s) information...\n");
    parse_disk_labels();
    parse_partitions();

    if (labelcount == 0)
        printf("!! notice: no disk labels detected. 'Windows' is expected source.\n");
    
    // check if we have target and source, if not exit. In the future, we may loop
    if (strlen(source) < 3)
    {
        start_color(RED);
        printf("XX-> error: no source disk was detected [labelled 'Windows'].\n");
        start_color(RESET);
        exit(1);
    }

    if (strlen(destination) < 3)
    {
        start_color(RED);
        printf("XX-> error: no destination disk was detected [of %dGiB].\n", sourcesize);
        start_color(RESET);
        exit(1);
    }

#ifdef DEBUG
    else
    {
        printf("found %d partition labels.\n", labelcount);
    }

    printf("we have the following labels:\n");

    for (int i = 0; i < 10; i++)
    {
        if (labelarray[i] == NULL)
            break;

        printf("label: %s\n", labelarray[i]->label);
    }
#endif
 
 // get the labels for the drive for better accuracy before we continue

 // final countdown, ask for permission to run the following command
 start_color(YELLOW);
 printf("$$ source: %s [%dGiB] %s\n", source, sourcesize, sourcetype);
 printf("$$ target: %s [%dGiB] %s\n", destination, destsize, desttype);
 printf("We are ready to clone %s -> %s\nPlease input 'Y' to continue (or N, i guess..:P):",
    source, destination);
    start_color(RESET);
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
        start_color(RED);
        printf("!! warning: unable to read /dev/disk/by-label\n");
        printf("!! warning: probably no install source, or, install source not labeled.\n");
        printf("!! warning: if using source, set label to 'Windows'\n");
        start_color(RESET);
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
#ifdef DEBUG
        printf("%s -> %s [%s]\n", acutalpath, devname, each->d_name);
#endif
           // check if source labeled Windows
        if (strcmp(each->d_name, "Windows") == 0)
        {
            //printf("debug: found source start");
            if (strlen(source) == 3)
            {
                start_color(RED);
                printf("!!\n!! warning: source is already defined { have you already cloned? }\n!!\n");
                start_color(RESET);
            }

             else
             {
                 strcpy(source, devname);

                 // get the size by cat'n'grep
                 // cat /proc/partitions | grep 'sda$'
                FILE *checker;
                char cmd[50];
                char *line;
                size_t length;
              
                sprintf(cmd, "cat /proc/partitions | grep '%s$'", devname);

                checker = popen(cmd, "r");
                size_t read = getline(&line, &length, checker);
                pclose(checker);
                // unable to find it in cat /proc/partitions
                if (read == -1)
                {
                    start_color(RED);
                    printf("XX-> error: unable to find source size in '/proc/partitions' grep\n");
                    start_color(RESET);
                    return;
                }

                int major, minor, blocks;
                char name[4];

                // try and find the size of the disk (in blocks)
                if (sscanf(line, "%d %d %d %s", &major, &minor, &blocks, name) == 4)
                {
                    int size = blocks / 1048576;
                                        // load our drive type information
                    char *dmodel;
                    char *dvendor;
                    char formatted[100];
					char *buffer;

                    sprintf(formatted, SRC_MODEL, devname);
                    checker = fopen(formatted, "r");
					
					if (checker == NULL)
					{
						dmodel = "unknown";
					}

					else
					{
						getline(&buffer, &length, checker);
						sscanf(buffer, "%s\n", &dmodel);
                    	fclose(checker);
					}
					
					buffer = NULL;
                    sprintf(formatted, SRC_VENDOR, devname);
                    checker = fopen(formatted, "r");

					if (checker == NULL)
					{
						dvendor = "unknown";
					}

					else
					{
						getline(&buffer, &length, checker);
						sscanf(buffer, "%s\n", &dvendor);
                    	fclose(checker);
					}
					

                    sprintf(sourcetype, "%s -> %s", dvendor, dmodel);

                    start_color(GREEN);
                    printf("Found source label in %s [%dGiB] %s\n", devname, size, sourcetype);
                    sourcesize = size;

                    start_color(RESET);

                }

                else
                {
                    start_color(RED);
                    printf("XX-> error: unable to find source size in '/proc/partitions' grep\n");
                    start_color(RESET);
                    return;
                }

                
                
                // multiplesource = 1;
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
    char name[20], dev[4];

    if (!f)
    {
        start_color(RED);
        printf("Error: /proc/partitions is not available!\n");
        start_color(RESET);
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
            // stop doing this calc over and over again plzkthx
            int gibsize = blocks / 1048576;

            // if we are root device and not a partition, create as disk
            if (sscanf(name, "%3s%d", dev, &part) < 2)
            {
#ifdef DEBUG
                printf("device: %s [size: %dGb]\n", name, gibsize);
#endif
                // initial configuration and setup of the Disk item for this
              //  Disk *dsk;
              //  dsk = malloc(sizeof(Disk));
                // if you change this comparison you will probably also want to modify
                // the proceeding else if to check if target is also the source drive or not
                // which is undefined behaviour
                if (strcmp(dev, source) == 0)
                    sourcesize = gibsize;

                // and probably the one after this one too
                else if (strlen(destination) == 3)
                {
                    start_color(RED);
                    printf("XX-> another target exists: %s [%dGB] -- why make this difficult, user? :P \n", dev, gibsize);
                    printf("XX-> error: please unplug unnecessary drives and try again.\n");
                    start_color(RESET);
                    exit(1);
                }

                // check for a disk that matches our sources size, that ISNT the source device
                //debug: change this to exclude the 64gib drive after testing.
                // we are splitting this block up as to only run the size calculation in one cycle.
                else if ((gibsize == sourcesize) && (sourcesize > 0))
                {

                    // load our drive type information from various linux space areas
                    FILE *checker;
                    char *dmodel;
                    char *dvendor;
                    char formatted[100];

                    sprintf(formatted, SRC_MODEL, dev);
                    // assign and check for value
                    checker = fopen(formatted, "r");
                    
                    if (checker == NULL)
                    {
                        strcpy(dmodel, "unknown");
                    }

                    else
                    {
                        getline(&dmodel, &length, checker);
                        fclose(checker);
                    }
                    
                    // reset formatted for the vendor file parse
                    sprintf(formatted, SRC_VENDOR, dev);
                    checker = fopen(formatted, "r");

                    if (checker == NULL)
                    {
                        strcpy(dvendor, "unknown");
                    }

                    else
                    {
                        getline(&dvendor, &length, checker);
                        fclose(checker);
                    }

                    sprintf(desttype, "%s -> %s", dvendor, dmodel);

                        strcpy(destination, dev);
                        destsize = sourcesize;
                        start_color(GREEN);
                        printf("found target same-sized device: %s [%dGB] %s\n", name, sourcesize, desttype);
                        start_color(RESET);
                        continue;
                }
            }
            
            else
            {
            ///sys/class/block/sdb/device/model | vendor | 
            //
                //printf("partition: %s -> %d [size: %dGb]\n", dev, part, (blocks / 1048576));
            }
            
        }
        


    }
}

void start_color(int color)
{
    switch (color)
    {
        case 1: printf("\033[0;31m"); break; // red
        case 2: printf("\033[0;34m"); break; // blue
        case 3: printf("\033[0;32m"); break; // green
        case YELLOW: printf("\033[0;33m"); break; // yellow
        case 5: printf("\033[0;35m"); break; // magenta
        case 6: 
        default: 
            printf("\033[0m"); break;   // reset     
    }
}
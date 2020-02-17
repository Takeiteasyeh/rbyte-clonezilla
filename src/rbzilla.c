/*
* ██████╗ ██████╗ ███████╗██╗██╗     ██╗      █████╗     ███╗   ███╗ ██████╗ ██████╗ 
* ██╔══██╗██╔══██╗╚══███╔╝██║██║     ██║     ██╔══██╗    ████╗ ████║██╔═══██╗██╔══██╗
* ██████╔╝██████╔╝  ███╔╝ ██║██║     ██║     ███████║    ██╔████╔██║██║   ██║██║  ██║
* ██╔══██╗██╔══██╗ ███╔╝  ██║██║     ██║     ██╔══██║    ██║╚██╔╝██║██║   ██║██║  ██║
* ██║  ██║██████╔╝███████╗██║███████╗███████╗██║  ██║    ██║ ╚═╝ ██║╚██████╔╝██████╔╝
* ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝╚══════╝╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ 
* file version: 	1.1-allop
* file modified: 	Feb 2020
* author:			Ray Lynk (https://www.hosthelp.ca/rbzilla/)
*
* description:		
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include "rbzilla.h"
#include "diskinfo.h"

#define DEBUG

Disklabel *labelarray[10*sizeof(Disklabel)];
_diskinfo *sourcedisk;
_diskinfo *targetdisk;
_diskinfo *Disks[10];


int diskcount = 0;

char source[DEVICE_STRING_SIZE]; // sda | nvme0 
char destination[DEVICE_STRING_SIZE];
char sourcetype[100];
char desttype[100];
int sourcesize;
int destsize;
short int labelcount=0;

int main(int argc, char *argv[])
{
   // source = malloc(4);
   start_color(YELLOW);
    printf("RBZilla mod %s by Ray Lynk - rlynk@hosthelp.ca ##\n", VERSION);
  
    printf("Doing detecty things....\n");
	start_color(RESET);
	parse_disk_info();
  //  parse_disk_labels();
    parse_partitions();

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

 
 // get the labels for the drive for better accuracy before we continue

 // final countdown, ask for permission to run the following command
 start_color(YELLOW);
 //printf("$$ source: %s [%dGiB] %s\n", source, sourcesize, sourcetype);
 //printf("$$ target: %s [%dGiB] %s\n", destination, destsize, desttype);
 printf("We are ready to clone %s -> %s\nPlease input 'Y' to continue (or N for more options):",
    source, destination);
    start_color(RESET);

	//char input = getchar();
	char cmd[300];
	sprintf(cmd, ZILLA_COMMAND, source, destination);
	
	while (1)
	{
		char input = getchar();

		printf("\n");

		switch (input)
		{
			case 'y':
			case 'Y': printf("Execute: %s\n", cmd);
				system(cmd);
				exit(1); // no reach
			
			case 'n':
			case 'N': printf("Abort; Swapping to fallback.\n");
					system(ZILLA_FALLBACK);
					exit(1);

			default:
				printf("what you just say to me? try 'Y' or 'N': ");
				break;
		}
	}
}

Disklabel *create_label(char *dev, char *label)
{
    Disklabel *dlabel = malloc(sizeof(Disklabel));

    strncpy(dlabel->device, dev, sizeof(dlabel->device));
    strncpy(dlabel->label, label, sizeof(dlabel->label));

    return dlabel;

}
void parse_disk_info()
{
	// start by getting a list of partitions to look through
	FILE *p_file;
	char *line = NULL;
	size_t length;
	size_t read;

	p_file = popen("cat /proc/partitions", "r");

	if (!p_file)
	{
		start_color(RED);
		printf("XX-> error: unable to parse /proc/partitions\n");
		exit(0);
	}

	int major, minor, blocks;
	char device[10];
	char rootdev[10];
	_diskinfo *s_disk;

	while ((read = getline(&line, &length, p_file)) != -1)
	{
		char root[5];
		int part;
		_diskinfo *di;

		if (sscanf(line, "%d %d %d %s\n", &major, &minor, &blocks, device) == 4)
		{
			// are we a root device?
			if ((sscanf(device, "%3s%1d", root, &part) != 2) && (sscanf(device, "%4s%1d", root, &part) != 2))
			{
				if (strncmp(root, "sr", 2) == 0)
				{
#ifdef DEBUG
					printf("%s: detected optical drive.\n", device);
#endif
				//	continue;
				}
				strcpy(rootdev, device);
				s_disk = malloc(sizeof(Disk));

				strcpy(s_disk->device, device);
				s_disk->size_gb = (blocks / 1048576);
#ifdef DEBUG
				printf("requesting disk info for %s\n", device);
#endif
				di = get_disk_info(device);

				if (di == NULL)
				{
					printf("%s: no disk info available.\n", device);
				}

				else
				{
					printf("debug: setting values\n");
					strcpy(s_disk->vendor, di->vendor);
					strcpy(s_disk->model, di->model);
					strcpy(s_disk->serial, di->serial);
					s_disk->is_usb = di->is_usb;

					//printf("got d: %s v: %s m: %s u: %s\n", di->device, di->vendor, di->model, di->is_usb ? "yes" : "no");
					//printf("is usb: %s", di->is_usb ? "yes" : "no");
				}
				
			}

			// not the root device
			else
			{
				//printf("not root device: %s\n", device);
				di = get_disk_info(device);

				if (di == NULL)
				{
					printf("no luck reading info for %s\n", device);
				}

				// check for Windows source label
				//printf("checking '%s' for label\n", di->label);

				if (strcmp(di->label, "Windows") == 0)
				{
					// check if we already have a source size from another entry
					if (sourcesize > 0)
					{
						start_color(YELLOW);
						printf("** notice: multiple source labels detected. Clone already completed?\n");

						if (di->is_usb)
						{
							// we also need to double check if our original source destination was actually the usb device.
							if (!sourcedisk->is_usb)
							{
								// source is set to ata which is probably wrong; correct it.
								printf("** notice: selecting usb as default source device.");
								// swap the two
								targetdisk = sourcedisk;
								sourcedisk = di;
							}

							printf("** notice: %s (usb device) is probably original source.\n", di->device);
							multiple_sources();
						}

						else if (sourcedisk->is_usb)
						{
							printf("** notice: %s (usb device) is probably original source.\n", sourcedisk->device);
							multiple_sources();
						}
						
						start_color(RESET);
						multiple_sources();
						exit(1);
					}

					// set the information to our root diskinfo, and then set it as our object pointer on the main stack
					//s_disk->source = 1;
					sourcesize = s_disk->size_gb;
					start_color(GREEN);
					printf("Found source: %s on %s @ %d GiB [%s - %s] {sn:%s}\n", 
						s_disk->device, s_disk->bus, 
						s_disk->size_gb, s_disk->vendor, s_disk->model, s_disk->serial);
					start_color(RESET);
					sourcedisk = s_disk;
					strcpy(source,s_disk->device);
				}
			}
			
		}

		//printf("read: %s", line);
	}

	pclose(p_file);
}


void parse_partitions()
{
    char *line = '\0';
    size_t length = 0;
    __ssize_t read;

    FILE *f = fopen("/proc/partitions", "r");

    int major, minor, blocks, part;
    char name[4], dev[4];

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

                // if you change this comparison you will probably also want to modify
                // the proceeding else if to check if target is also the source drive or not
                // which is undefined behaviour
                if (strcmp(dev, source) == 0)
                    sourcesize = gibsize;

                // and probably the one after this one too
                else if ((strlen(destination) == 3) && (gibsize == sourcesize))
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

					_diskinfo *p_dinfo;
					p_dinfo = get_disk_info(name);

                        strcpy(destination, dev);
                        destsize = sourcesize;
                        start_color(GREEN);
                        printf("Found target: %s on %s @ %d GiB [%s - %s] {sn:%s}\n", 
						name, p_dinfo->is_usb ? "USB" : "ATA", 
						sourcesize, p_dinfo->vendor, p_dinfo->model, p_dinfo->serial);
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

void multiple_sources()
{

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
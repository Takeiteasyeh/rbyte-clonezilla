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
* description:	Grabs all our disk information on a given disk.	
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diskinfo.h"
#include "rbzilla.h"
extern short int nvme_count;
extern short int usb_count;
extern short int ata_count;


_diskinfo *get_disk_info(const char *device)
{
	//printf("We have device size of %lu\n", strlen(device));
	if (strlen(device) < 3)
		return NULL;
		
	char *buffer = NULL;
	
	//_diskinfo *dinf = calloc(1, sizeof(_diskinfo));
	// _diskinfo *p_diskinfo = &dinf;
	//printf("here2");
	_diskinfo *p_diskinfo = calloc(1, sizeof(*p_diskinfo));
	
//printf("here3");
	if (p_diskinfo == NULL)
	{
		printf("unable to allocate memory\n");
		exit(0);
	}

	strcpy(p_diskinfo->device, device);

	FILE *p_file;
	size_t buffer_size;
	char *cmd;
	cmd = malloc(sizeof(UDEV_COMMAND) + DEVICE_STRING_SIZE); // we use more to account for nvme and other devices
	sprintf(cmd, UDEV_COMMAND, device);
	p_file = popen(cmd, "r");
	size_t bs;

	if (!p_file)
	{
		printf("fatal: unable to run udevadm on %s\n", device);
		exit(1);
	}

	strcpy(p_diskinfo->model, "unknown");
	strcpy(p_diskinfo->vendor, "unknown");
	strcpy(p_diskinfo->serial, "unknown");

	while ((bs = getline(&buffer, &buffer_size, p_file)) != -1)
	{
//#ifdef DEBUG
		//printf("line: %s", buffer);
//#endif
		//0x55555555ae90 "P: /devices/pci0000:00/0000:00:1f.2/ata1/host0/target0:0:0/0:0:0:0/block/sda\n"
		///: ID_BUS=ata
		// check for the given information we are looking for
		sscanf(buffer, "N: %s", p_diskinfo->device);
		sscanf(buffer, "E: ID_VENDOR=%s", p_diskinfo->vendor);
		sscanf(buffer, "E: ID_MODEL=%s", p_diskinfo->model);
		sscanf(buffer, "E: ID_SERIAL_SHORT=%s", p_diskinfo->serial);

		char ftype[10];

		if (sscanf(buffer, "E: ID_FS_TYPE=%s\n", ftype) == 1)
		{
			if (strcmp(ftype, "ntfs") == 0)
			{
				p_diskinfo->is_ntfs = YES;
			}
		}

		sscanf(buffer, "E: ID_FS_LABEL=%s", p_diskinfo->label);
		
		if (sscanf(buffer, "E: ID_BUS=%s\n", p_diskinfo->bus) == 1)
		{
			//strncpy(p_diskinfo->bus, "nvme0", 6);
			if (strcmp(p_diskinfo->bus, "nvme") == 0)
			{
				p_diskinfo->is_nvme = YES;
				p_diskinfo->is_target = YES;
				nvme_count++;
			}

			else if (strcmp(p_diskinfo->bus, "ata") == 0)
			{
				p_diskinfo->is_ata = YES;
				p_diskinfo->is_target = YES;
				ata_count++;
			}
		}

		if (strcmp(p_diskinfo->bus, "usb") == 0)
		{
			p_diskinfo->is_usb = 1;

			// we dont do source stuff here. do it later.
		}

		else
			p_diskinfo->is_usb = 0;
		}
		

	pclose(p_file);

	return p_diskinfo;
}


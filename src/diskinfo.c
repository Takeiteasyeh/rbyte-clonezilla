#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diskinfo.h"

_diskinfo *get_disk_info(const char *device)
{
	if (strlen(device) < 3)
		return NULL;
		
	char *buffer = NULL;
	_diskinfo *p_diskinfo = malloc(sizeof(_diskinfo));
	strcpy(p_diskinfo->device, device);

	FILE *p_file;
	size_t buffer_size;
	char *cmd;
	cmd = malloc(sizeof(UDEV_COMMAND) +5);
	sprintf(cmd, UDEV_COMMAND, device);
	p_file = popen(cmd, "r");
	size_t bs;

	if (!p_file)
	{
		printf("unable to run udevadm on %s\n", device);
		exit(1);
	}

strcpy(p_diskinfo->model, "unknown");
strcpy(p_diskinfo->vendor, "unknown");
strcpy(p_diskinfo->serial, "unknown");

	while ((bs = getline(&buffer, &buffer_size, p_file)) != -1)
	{
		//printf("line: %s", buffer);
		// check for the given information we are looking for
		sscanf(buffer, "N: %s", p_diskinfo->device);
		sscanf(buffer, "E: ID_VENDOR=%s", p_diskinfo->vendor);
		sscanf(buffer, "E: ID_MODEL=%s", p_diskinfo->model);
		sscanf(buffer, "E: ID_SERIAL_SHORT=%s", p_diskinfo->serial);
		sscanf(buffer, "E: ID_FS_LABEL=%s", p_diskinfo->label);
		char bus[5];

		if (sscanf(buffer, "E: ID_BUS=%s", bus) == 1)
		{

		if (strcmp(bus, "usb") == 0)
			p_diskinfo->is_usb = 1;

		else
			p_diskinfo->is_usb = 0;
		}
		
	}

	pclose(p_file);

	return p_diskinfo;

}
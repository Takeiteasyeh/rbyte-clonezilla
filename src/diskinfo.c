#include <stdio.h>
#include <stdlib.h>
#include "diskinfo.h"

_diskinfo *get_disk_info(const char *device)
{
	char *buffer = NULL;
	_diskinfo *p_diskinfo = malloc(sizeof(_diskinfo));

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

	while ((bs = getline(&buffer, &buffer_size, p_file)) != -1)
	{
		printf("line: %s", buffer);
	}

	return p_diskinfo;

}
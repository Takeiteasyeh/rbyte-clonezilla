/*
* ██████╗ ██████╗ ███████╗██╗██╗     ██╗      █████╗     ███╗   ███╗ ██████╗ ██████╗ 
* ██╔══██╗██╔══██╗╚══███╔╝██║██║     ██║     ██╔══██╗    ████╗ ████║██╔═══██╗██╔══██╗
* ██████╔╝██████╔╝  ███╔╝ ██║██║     ██║     ███████║    ██╔████╔██║██║   ██║██║  ██║
* ██╔══██╗██╔══██╗ ███╔╝  ██║██║     ██║     ██╔══██║    ██║╚██╔╝██║██║   ██║██║  ██║
* ██║  ██║██████╔╝███████╗██║███████╗███████╗██║  ██║    ██║ ╚═╝ ██║╚██████╔╝██████╔╝
* ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝╚══════╝╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ 
* file version: 	2.1
* file modified: 	Mar 2020
* author:			Ray Lynk (https://www.hosthelp.ca/rbzilla/)
*
* description: interface to access the smartctl information and structure for holding
* 				said information.		
*/
#include <stdio.h>
#include <malloc.h>
#ifndef H_SMART
#include "smart.h"
#endif
#ifndef H_DISKINFO
#include "diskinfo.h"
#endif
#include "rbzilla.h"


_smart *smart_get(const _diskinfo *device)
{
	// create the initial struct and set it as invalid till we have good fetch.
	_smart *smart;
	
	smart = calloc(1, sizeof(*smart));

	if (smart == NULL)
	{
		printf("fatal: memory exhausted!\n");
	}

	smart->valid = NO;

	FILE *p_file;
	char *buffer;
	size_t buffer_size;
	char *cmd;
	cmd = malloc(sizeof(CMD_SMART_FETCH) + sizeof(device->device)); // we use more to account for nvme and other devices
	sprintf(cmd, CMD_SMART_FETCH, device->device);
	p_file = popen(cmd, "r");
	size_t bs;

	// if we fail to retrieve smartctl due to this error we just return our invalid object.
	if (!p_file)
	{
		printf("warning unable to run smartctl on %s\n", device->device);
		return smart;
	}

	while ((bs = getline(&buffer, &buffer_size, p_file)) != -1)
	{
		if (sscanf(buffer, " %*d Power_On_Hours %*x %*d %*d %*d %*s %*s - %d", &smart->lifetime) == 1)
		{
			//				  9 Power_On_Hours 0x0032   084   084   000    Old_age   Always       -       6794                  
			printf ("got lifetime %d\n\n\n\n", smart->lifetime);
		}

	//	if (smart->lifetime > 0)
	//	{
	//		printf("lifetime is %dhrs\n", smart->lifetime);
	//	}
		/*
		ID# ATTRIBUTE_NAME          FLAG     VALUE WORST THRESH TYPE      UPDATED  WHEN_FAILED RAW_VALUE                                                                                                   
                                                                                                                                                                                                   
 		1 Raw_Read_Error_Rate     0x000b   100   100   050    Pre-fail  Always       -       0                                                                                                                                                                                            
  		2 Throughput_Performance  0x0005   100   100   050    Pre-fail  Offline      -       0                  
  */

		printf("%s\n", buffer);
	}

	return smart; //just here to allow for compile
	// not done
}
_smart *smart_test(const _diskinfo *device);
int get_drive_percent(const _diskinfo *device);
void calculate_percent(const _diskinfo *device);
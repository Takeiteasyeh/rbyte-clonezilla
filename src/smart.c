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
		if (sscanf(buffer, " %*d Power_On_Hours %*x %*d %*d %*d %*s %*s - %d", &smart->poweron_hours) == 1)
		{
			//				  9 Power_On_Hours 0x0032   084   084   000    Old_age   Always       -       6794                  
			printf ("got lifetime %d\n\n\n\n", smart->poweron_hours);
		}

		else if (sscanf(buffer, " %*d Reallocated_Sector_Ct %*x %*d %*d %*d %*s %*s - %d", &smart->realloc_sector_count) == 1)
		{
			printf("got realloc: %d\n", smart->realloc_sector_count);
		}

		else if (sscanf(buffer, " %*d Seek_Error_Rate %*x %*d %*d %*d %*s %*s - %d", &smart->seek_error_rate) == 1)
		{
			printf("got seek error: %d\n", smart->seek_error_rate);
		}

		else if (sscanf(buffer, " %*d Spin_Retry_Count %*x %*d %*d %*d %*s %*s - %d", &smart->spin_retry_count) == 1)
		{
			printf("got retry: %d\n", smart->spin_retry_count);
		}

		else if (sscanf(buffer, " %*d Reallocated_Event_Count %*x %*d %*d %*d %*s %*s - %d", &smart->realloc_event_count) == 1)
		{
			printf("realloc_event_count: %d\n", smart->realloc_event_count);
		}

		else if (sscanf(buffer, " %*d Current_Pending_Sector %*x %*d %*d %*d %*s %*s - %d", &smart->current_pending_sectors_count) == 1)
		{
			printf("current_pending_sectors: %d\n", smart->current_pending_sectors_count);
		}

		else if (sscanf(buffer, " %*d Offline_Uncorrectable %*x %*d %*d %*d %*s %*s - %d", &smart->offline_uncorrectable_sectors_count) == 1)
		{
			printf("offline_uncorrectable: %d\n", smart->offline_uncorrectable_sectors_count);
		}

		else if (sscanf(buffer, " %*d Temperature_Celsius %*x %*d %*d %*d %*s %*s - %d (Min/Max %d/%d)", &smart->ctemp_now, &smart->ctemp_low, &smart->ctemp_high) == 1)
		{
			printf("temp (now/low/high): %d/%d/%d\n", smart->ctemp_now, smart->ctemp_low, smart->ctemp_high);
		}

		//194 Temperature_Celsius     0x0022   100   100   000    Old_age   Always       -       37 (Min/Max 12/58) 
		/*
		ID# ATTRIBUTE_NAME          FLAG     VALUE WORST THRESH TYPE      UPDATED  WHEN_FAILED RAW_VALUE                                                                                                   
                                                                                                                                                                                                   
 		1 Raw_Read_Error_Rate     0x000b   100   100   050    Pre-fail  Always       -       0                                                                                                                                                                                            
  		2 Throughput_Performance  0x0005   100   100   050    Pre-fail  Offline      -       0                  
  */

	//	printf("%s\n", buffer);
	}

	return smart; //just here to allow for compile
	// not done
}
_smart *smart_test(const _diskinfo *device);
int get_drive_percent(const _diskinfo *device);
void calculate_percent(const _diskinfo *device);
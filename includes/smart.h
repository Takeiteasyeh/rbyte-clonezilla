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

#ifndef H_SMART
#define H_SMART
#include "diskinfo.h"

// define our commands to use for the SMART interface
#define CMD_SMART_TEST "sudo smartctl -t %s /dev/%s"
#define CMD_SMART_FETCH "smartctl -Ha /dev/%s"

/* We define our weights and percent limitations using a 
 * character deliminated string. We can extract this out 
 * later using sscanf to determine our total weight limit. 
 * 
 * this is for future use
#define WEIGHT_REALLOC_SECTOR_COUNT "1:70"
#define WEIGHT_SEEK_ERROR_RATE "0.5:20"

typedef struct SmartWeights
{
	int realloc_sector_count;
}


Reallocated sectors count	1	70
Seek error rate	0.5	20
Spin retry count	3	60
Reallocation event count	0.6	30
Current pending sectors count	0.6	48
Off line uncorrectable sectors count	1	70 
*/

//smartctl -Ha /dev/sdx smartctl -t short /dev/sdx smartctl -l selftest /dev/sdx

typedef struct Smart
{
	int valid; // set if this current struct is a valid struct.
	char *error; // error message if struct is invalid.
	/* temperature settings */
	int ctemp_now;
	int ctemp_low;
	int ctemp_high;

	int poweron_hours;
	int realloc_sector_count;
	int seek_error_rate;
	int spin_retry_count;
	int realloc_event_count;
	int current_pending_sectors_count;
	int offline_uncorrectable_sectors_count;
	int health_percent;
	char *logs;
} _smart;


_smart *smart_get(const _diskinfo *device);
_smart *smart_test(const _diskinfo *device);
int get_drive_percent(const _diskinfo *device);
void calculate_percent(const _diskinfo *device);



#endif

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

#define H_SMART

#ifndef H_DISKINFO
#include "diskinfo.h"
#endif

typedef struct Smart
{
	int valid; // set if this current struct is a valid struct.
	char *error; // error message if struct is invalid.

	int lifetime;
	int health_percent;


} _smart;


_smart *smart_get(const _diskinfo *device);
_smart *smart_test(const _diskinfo *device);
int get_drive_percent(const _diskinfo *device);
void calculate_percent(const _diskinfo *device);

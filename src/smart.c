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
#include "smart.h"
#endif

_smart *smart_get(const _diskinfo *device);
_smart *smart_test(const _diskinfo *device);
int get_drive_percent(const _diskinfo *device);
void calculate_percent(const _diskinfo *device);
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
* description: basic include handling DiskInfo struct and holding UDEVADM command string.		
*/

#define UDEV_COMMAND "udevadm info --query=all --name=/dev/%s"

//const char *dfields[] = { "DEVNAME", "ID_VENDOR", "ID_MODEL", "ID_SERIAL_SHORT", "ID_FS_LABEL"};

typedef struct DiskInfo
{
	char device[5]; // /dev/sdx
	char label[12];
	char vendor[100];
	char model[100];
	char serial[100];
	int size_gb;
	
	short unsigned int is_usb;


} _diskinfo;

_diskinfo *get_disk_info(const char *device);
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
#ifndef DEVICE_STRING_SIZE
#define DEVICE_STRING_SIZE 10
#endif

#define UDEV_COMMAND "udevadm info --query=all --name=/dev/%s"

//const char *dfields[] = { "DEVNAME", "ID_VENDOR", "ID_MODEL", "ID_SERIAL_SHORT", "ID_FS_LABEL"};

typedef struct DiskInfo
{
	char device[DEVICE_STRING_SIZE]; // /dev/sdx
	char root[5];
	char label[12];
	char vendor[100];
	char model[100];
	char serial[100];
	char bus[10];
	int size_gb;
	
	short unsigned int is_usb;
	short unsigned int is_nvme;
	short unsigned int is_ata;
	short unsigned int is_optical;
	short unsigned int is_partition;
	short unsigned int is_source;
	short unsigned int is_target;

} _diskinfo;

_diskinfo *get_disk_info(const char *device);
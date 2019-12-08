/*


*/
#define YES 1
#define NO 0

#define MAX_DISK_LABEL_SIZE 12 // with null terminator
#define MAX_PARTITIONS_PER_DISK 128 // rfc max of 128 partitions per any disk
#define NO_FLASH_SOURCES 1 // do not allow drives under 64gib to act as source media
#define MAX_FILE_READ_SIZE 

int main(int argc, char *argv[]);
void parse_partitions();

typedef struct Disk
{
    struct Disk *next, *prev;
    struct Partition partitions[MAX_PARTITIONS_PER_DISK];
    char device[6];
    //char label[MAX_DISK_LABEL_SIZE]; // 11 and null terminator
    unsigned long long int size;
    unsigned long long int free;
    unsigned short int partcount;
    unsigned short int source;
    unsigned short int target;
    unsigned short int flash;

} Disk;

typedef struct Partition
{
    char device[2];
    char label[MAX_DISK_LABEL_SIZE];
    unsigned long long int size;
    unsigned long long int free;
} Partition;
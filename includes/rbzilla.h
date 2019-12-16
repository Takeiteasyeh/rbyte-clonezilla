/*


*/
#define YES 1
#define NO 0

#define ZILLA_FALLBACK "sudo /usr/sbin/ocs-onthefly -x"
#define ZILLA_COMMAND "sudo /usr/sbin/ocs-onthefly -g auto -e1 auto -e2 -r -j2 -sfsck -pa poweroff -f %s -t %s" // source and target
#define MAX_DISK_LABEL_SIZE 12 // with null terminator
#define MAX_PARTITIONS_PER_DISK 128 // rfc max of 128 partitions per any disk
#define NO_FLASH_SOURCES 1 // do not allow drives under 64gib to act as source media
#define MAX_FILE_READ_SIZE 

/* definitions for the color profiles, including reset */
#define RED     1
#define BLUE    2
#define GREEN   3
#define YELLOW  4
#define MAGENTA 5
#define RESET   6

#define SRC_MODEL   "/sys/class/block/%s/device/model"
#define SRC_VENDOR  "/sys/class/block/%s/device/vendor"

int main(int argc, char *argv[]);
void parse_partitions();
void parse_disk_labels();
void parse_disk_info();
void start_color(int color);
void end_color();


typedef struct Disk
{
    char device[4];
	char vendor[100];
	char model[100];
	char serial[100];

    //char label[MAX_DISK_LABEL_SIZE]; // 11 and null terminator
    unsigned long long int size_gb;
    unsigned short int partcount;
    unsigned short int source;
    unsigned short int target;

} Disk;

typedef struct Partition
{
    struct Partition *next, *prev;
    char device[2];
    char label[MAX_DISK_LABEL_SIZE];
    unsigned long long int size;
    unsigned long long int free;
} Partition;

typedef struct Disklabel
{
    char device[6];
    char label[MAX_DISK_LABEL_SIZE];
} Disklabel;

Disklabel *create_label(char *disk, char *label);
Disk create_disk(char *dev);
Partition create_part(Disk *disk, char *part, char *label);
/*
char *ascii = "                                                                           
                 `Ny-                                   `/dy               
                 :MMMm+`                              -sNMMN               
                 sMMMMMNy:                         `+dMMMMMM.              
                 yMNMMdNMMmo.                    :yNMmdMmdMM:              
                 hMsMMMhhmMMNy:     ````      .omMMmhdMMsmMM:              
                 hMhdMMNNhymMMMmmmNNNNNNNNNmmmNMMmhmMMMM+MMM:              
                 yMMhMmyodMMMMMMMMMMMMMMMMMMMMMMMMMdyymMNMMM:              
                 sMMMMMNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM-              
                 +MMNdysoo///:/-:://osyyyyoooooo+////::/:::/.              
                 :N+.`  `y/` /d/       `+osyyyyyhyyyyydmNNNm               
     `...`       .+   ` -MMy mMh`-:    +MMMMNhoyh-````../NMy        ``...  
      ```.---...``.   sy+:s/-//`hNh    sMMm+. +Nmm.     +MMs `...---.```   
    `.....```...-:.   /dd`:mNNy::o.   .mMy`   /mhM+  `:hMMMN/-...```.....  
    ````......-//mm+   ``oNMMMMNh`   /mMMdo:-``:dm::sdNMMMMMh/:-.....````  
   ..----:---:..:NMMy-  .NMMNmmmd.`:hNNNMMMMNmmdmNNMMMMMMMMMd..----:----.  
                 :smMNho/++/-.-:/sdMNhmddMMMMMMMMMMMMMMMMNd+.              
                   `:sdNMNNmmmmNMMMMMNhdMMMMMMMMMMMMMMmh+-`                
                      `-/sdmNMMMMMMMMMMMMMMMMMMMMNmho:.                    
      -osyyyyyhyyso:.      .-:+oyhdNmhdddMmhyso/:.`   .:+syyhyhyyyso:      
      /MMMMMNsyyNMMMy              .+yhdy+`           sMMMNyysNMMMMM+      
    .ydMMMMMNdhyMMMm.  `y+  `so   o+  .:-   oy`  +y.  `mMMMyhmNMMMMMdy-    
    `omMMMMMMMMMMMMy-..hm.  yN-  oN/  -Ns   :dy  .md.`-yMMMMMMMMMMMMmo`    
      .+dMMMMMMMMdsshdmMs-.sN:  /N+  .md`  /d:Ns`-sMmdhssdMMMMMMMMdo.      
        `:sdNMMMm-  `hN+ohmMm+-/Ny  `hm.  :Ns /hoso+Nd`  -NMMMNds:`        
            ./sy-   sN:  /Ns/omMNy+-yN-  .mh` `ym`  :Ns   .ys/-            
                    :-  -Ny  .md-/sNMms+:mm`  sN-  `.-:                    
                  `    `dh` `hm.  :Mo-:sMNds+sM+  `hd   `                  
                 /d-  `ss/. sN-  .my   sN:.:dMmhs+hM:  -d/                 
    ````````     `dNoshdNN: +:  `dd`  +M+  :Ns`./NNmhyoNd`     ````````    
    -hmmdddhhyyyyhdmM+. :Ny     sm.  :No  -Nh   yN- .+Mmdhyyyyhhdddmmh-    
     `/hNMMMMMMMs. `dh   :N/    ..   `-   -+`  /N:   hd. .sMMMMMMMNh/`     
        .dMMMMN+    `-    `                     `    .`    +NMMMMd.        
         mMMMh.                                             .hMMMm         
        .MMm:                                                 :mMM-        
        :No`                                                   `oN:        
         `                                                       `         
                                                                           
"; */
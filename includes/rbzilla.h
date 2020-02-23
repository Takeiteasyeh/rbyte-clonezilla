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
* description:	Include file for the main application.	
*/ 
#define H_RBZILLA

#define YES 1
#define NO 0
#define VERSION "2.0"

#define ZILLA_FALLBACK "sudo /usr/sbin/ocs-onthefly -x"
#define ZILLA_ORIGIN "sudo clonezilla"
#define ZILLA_COMMAND "sudo /usr/sbin/ocs-onthefly -g auto -e1 auto -e2 -r -j2 -sfsck -ps -pa poweroff -f %s -t %s" // source and target
#define ZILLA_SIZEDIFF_COMMAND "sudo /usr/sbin/ocs-onthefly -g auto -e1 auto -e2 -r -j2 -sfsck -ps -icds -k1 -pa poweroff -ps -f %s -t %s"

#define MAX_DISK_LABEL_SIZE 12 // with null terminator
#define MAX_PARTITIONS_PER_DISK 128 // rfc max of 128 partitions per any disk
#define NO_FLASH_SOURCES 1 // do not allow drives under 64gib to act as source media
#define MAX_FILE_READ_SIZE

#ifndef DEVICE_STRING_SIZE
#define DEVICE_STRING_SIZE 10 // max size of the device 'sda / nvme0' including null term.
#endif
/* definitions for the color profiles, including RESET -- use it! */
#define RED     1
#define BLUE    2
#define GREEN   3
#define YELLOW  4
#define MAGENTA 5
#define RESET   6 // DO NOT FORGET, ESTI! YOU MUST ALWAYS CALL THIS AT SOME POING AFTER!!

#define SRC_MODEL   "/sys/class/block/%s/device/model"
#define SRC_VENDOR  "/sys/class/block/%s/device/vendor"

int main(int argc, char *argv[]);
void multiple_sources();
void parse_partitions();
void parse_disk_labels();
void parse_disk_info();
void start_color(int color);
void end_color();
void reset_counts();
void manually_set_disks();
void show_menu();
void show_disks();
void copy_disks(int reverse);

/*
typedef struct Partition
{
    struct Partition *next, *prev;
    char device[2];
    char label[MAX_DISK_LABEL_SIZE];
    unsigned long long int size;
    unsigned long long int free;
} Partition;
*/

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
         mMMMh.     Have you eaten your shreddies today?   .hMMMm         
        .MMm:                                                 :mMM-        
        :No`                                                   `oN:        
         `                                                       `         
                                                                           
"; */
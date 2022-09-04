#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <string.h>
#include <errno.h>


void usage(){
    printf("\nExpected two additional arguments.");
    printf("\nArgument 1: Path to file.");
    printf("\nArgument 2: Word to be copied to file");
}

int main(int argc,char** argv){
openlog("finder-log",LOG_PID|LOG_ERR,LOG_USER);  
setlogmask(LOG_UPTO(LOG_DEBUG));
    if(argc !=3){
        usage();
        return(1);
    }

    size_t count;
    ssize_t nr;
    char* file_path=argv[1];
    char* string=argv[2];

    int fd = open(file_path, O_RDWR| O_CREAT ,0644);
    if (fd == -1){
    syslog (LOG_USER, "Unable to open file, Check the perssion");
    syslog(LOG_ERR, "ERROR: %s", strerror(errno));
    return(1);
    }    

count = strlen(string);
syslog (LOG_DEBUG, "Wrinting %s to %s.",string,file_path);

nr = write(fd, string, count);
if (nr == -1){
        syslog (LOG_USER, "Writing to file not Successfull");
        syslog(LOG_ERR, "Error: %s", strerror(errno));     
        return(1);
}
else if (nr != count){
        syslog (LOG_USER, "partial write");       
        return(1);
}
else
syslog (LOG_USER, "Writing Successfull"); 

close(fd);
closelog();
return(0);
}
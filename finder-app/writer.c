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
    printf("Expected two additional arguments.");
}

int main(int argc,char** argv){

    if(argc !=3){
        usage();
        closelog ();
        return(1);
    }

    size_t count;
    ssize_t nr;
    char* file_path=argv[1];
    char* string=argv[2];

    int fd = open(file_path, O_RDWR | O_CREAT ,0644);
    if (fd == -1){
    printf("File was not opened properly");
    syslog (LOG_USER, "Unable to open file");
    perror ("open");
    syslog(LOG_ERR, "Couldn't open: %s", strerror(errno));
    return(1);
    }

count = strlen(string);
nr = write(fd, string, count);
if (nr == -1){
        printf("Not successfull");
        syslog (LOG_USER, "Not Successfull");
        syslog(LOG_ERR, "Issue while writing: %s", strerror(errno));     
        return(1);
}
else if (nr != count){
        printf("Partial write");
        syslog (LOG_USER, "partial write");       
        return(1);
}
else
close(fd);
return(0);
}
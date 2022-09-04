#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>

void usage(){
    printf("Expected two additional arguments.");
}

int main(int argc,char** argv){
    if(argc !=3){
        usage();
        exit(1);
    }

    size_t count;
    ssize_t nr;
    char* file_path=argv[1];
    char* string=argv[2];

    int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC ,0700);
    if (fd == -1){
    printf("File was not opened properly");
    perror ("open");
        exit(1);
    }

count = sizeof (string);
nr = write(fd, string, count);
if (nr == -1){
        printf("Not successfull");
        exit(-1);
}
else if (nr != count){
        printf("Partial write");
        exit(-1);
}
exit(0);
}
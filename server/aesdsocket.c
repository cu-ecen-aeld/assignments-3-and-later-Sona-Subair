#include<sys/types.h>
#include<sys/socket.h>
#include<sys/syslog.h>
#include<errno.h>
#include<signal.h>
#include<sys/syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include <netdb.h>

// struct sockaddr{
//     sa_family_t sa_family;
//     char sa_data[14];
// }

#define PORT "9000"
#define BACKLOG (10)
#define BUFFER_SIZE (1024)

char* file_path="/var/tmp/aesdsocketdata";

static void signal_handler (int signo)
{
    if(signo == SIGINT || signo == SIGTERM){
    syslog (LOG_USER,"Caught Signal Exiting!\n");
    remove(file_path);
    exit (EXIT_SUCCESS);
    }
    
}

int main(){
openlog("finder-log",LOG_PID|LOG_ERR,LOG_USER);     
setlogmask(LOG_UPTO(LOG_DEBUG));
struct addrinfo hints;
struct addrinfo *server_info;
int sockfd,status,fd,file_fd,nr;
void* buf[BUFFER_SIZE];
ssize_t size_recived=0,size_send=0;
struct sockaddr_storage client_addr;
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;
if (signal (SIGTERM, signal_handler) == SIG_ERR) {
        printf ("Cannot handle SIGTERM!\n");
        exit (EXIT_FAILURE);
}
if (signal (SIGINT, signal_handler) == SIG_ERR) {
        printf ( "Cannot handle SIGTERM!\n");
        exit (EXIT_FAILURE);
}


status=getaddrinfo(NULL,PORT,&hints,&server_info);
socklen_t address_len=sizeof(struct sockaddr);
if(status!=0){
    printf("Error while getting address");
    return(-1);
}
sockfd=socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);
bind(sockfd,server_info->ai_addr,server_info->ai_addrlen);
freeaddrinfo(server_info);
listen(sockfd,BACKLOG);
while(1){
fd=accept(sockfd,&client_addr,sizeof(client_addr));
if(fd==-1){
    printf("Error while accepting");
    return(-1);
}
bool recv_complete=false;
while(!recv_complete){
size_recived+=recv(fd,buf,BUFFER_SIZE,0);
if(*buf[size_recived]=='\0'){
    recv_complete=true;
}
file_fd=open(file_path, O_RDWR|O_CREAT|O_APPEND ,0644);
if(file_fd == -1){
    syslog(LOG_USER, "Unable to open file, Check the permission");
    return(-1);
}  
nr=write(file_fd, &buf, size_recived);
if(nr == -1){
    syslog(LOG_USER, "Writing to file not Successfull");  
    return(-1);
}
}
bool send_complete=false;
while(!send_complete){
nr=read(file_fd, buf, size_recived);
if(nr == -1){
    syslog(LOG_USER, "Writing to file not Successfull");
    return(-1);
}   
size_send+=send(fd,&buf,BUFFER_SIZE,0);
if(*buf[size_send]=='\0'){
    recv_complete=true;
}
}
}
return(0);
}
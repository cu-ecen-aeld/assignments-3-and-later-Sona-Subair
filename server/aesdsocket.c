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
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>

// struct sockaddr{
//     sa_family_t sa_family;
//     char sa_data[14];
// }

#define PORT "9000"
#define BACKLOG (6)
#define BUFFER_SIZE (1024)
#define FILE_PATH "/var/tmp/aesdsocketdata"

static void signal_handler (int signo)
{
    if(signo == SIGINT || signo == SIGTERM){  
    syslog (LOG_USER,"Caught Signal Exiting!\n");
    remove(FILE_PATH);
    exit (EXIT_SUCCESS);
    }
    
}

int main(int argc,char* argv[]){
struct addrinfo hints;
bool deamon=false;
struct addrinfo *server_info;
int sockfd,status,new_sockfd,file_fd,nr,ret;
char* buf=malloc(BUFFER_SIZE*sizeof(char*));
int size_recived=0,size_read=0,s_recv,s_send,current_size=0;
struct sockaddr client_addr;
char clientIP[INET6_ADDRSTRLEN];
void* in_addr;
socklen_t address_len=sizeof(struct sockaddr);
socklen_t addr_size=sizeof(client_addr);

if(argc>2){
    if(argv[1]=="-d"){
        deamon=true;
    }
}

signal (SIGTERM, signal_handler);
signal (SIGINT, signal_handler);

openlog("aesd-socket",LOG_PID|LOG_ERR,LOG_USER);     
setlogmask(LOG_UPTO(LOG_DEBUG));

memset(&hints,0,sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;
hints.ai_protocol=0;

syslog(LOG_USER, "Started");

status=getaddrinfo(NULL,PORT,&hints,&server_info);
if(status!=0){
    syslog(LOG_USER, "Error while getting address");
    return(-1);
}
sockfd=socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);
if(sockfd==-1){
    syslog(LOG_USER, "Not able to create socket");
    perror("socket");
    return(-1);
}
ret=bind(sockfd,server_info->ai_addr,sizeof(struct sockaddr));
if(ret<0){
    syslog(LOG_USER, "Binding not done.");

}
freeaddrinfo(server_info);

syslog(LOG_USER, "Listening");
listen(sockfd,BACKLOG);
while(1){  
    size_recived=0;
    size_read=0;
    syslog(LOG_USER, "Entered");  
    new_sockfd=accept(sockfd,&client_addr,&addr_size);
    syslog(LOG_USER, "Here1");  
    if(new_sockfd==-1){
        syslog(LOG_USER, "Error while accepting");
        perror("accept");
        close(sockfd);
        return(-1);
    }
    syslog(LOG_USER, "Accepted Connection");
    inet_ntop(AF_INET,&client_addr,clientIP,sizeof(clientIP));
    bool recv_complete=false;
    while(!recv_complete){
        syslog(LOG_USER, "Inside recv loop");
        s_recv=recv(new_sockfd,(buf+current_size),BUFFER_SIZE,0);
        if(s_recv==-1){
            syslog(LOG_USER, "Error while recieving");
            return(-1);
        }
        size_recived+=s_recv;
        if(size_recived>0 && buf[size_recived-1]=='\n'){
            recv_complete=true;
            syslog(LOG_USER, "Recieving complete");
        }
        current_size+=BUFFER_SIZE;
    }
    file_fd=open(FILE_PATH, O_RDWR | O_CREAT | O_APPEND ,0644);
    syslog(LOG_USER, "EHere2");  
    if(file_fd == -1){
        syslog(LOG_USER, "Unable to open file to read, Check the permission");
        perror("open");
        return(-1);
    }     
          while(nr=write(file_fd, &buf, size_recived);
        if(nr == -1){
            syslog(LOG_USER, "Writing to file not Successfull");  
            return(-1);
            }
            syslog(LOG_USER, "EHere3");      

    char message[BUFFER_SIZE];
    lseek(file_fd,0,SEEK_SET);
    while(nr=read(file_fd,message,BUFFER_SIZE)!=0){
        if(nr == -1){
            syslog(LOG_USER, "EHere7"); 
            syslog(LOG_USER, "Reading from file not Successfull");   
            return(-1);
        } 
        syslog(LOG_USER, "EHere8");  
        s_send=send(new_sockfd,message,nr,0);
        if(s_send<0){
           syslog(LOG_USER, "Sending failed");  
        }
    }
        syslog(LOG_USER, "EHere10");     
        
}
remove(FILE_PATH);
free(buf);
close(new_sockfd);
closelog();
close(file_fd);
return(0);
}
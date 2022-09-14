#define _POSIX_SOURCE
#include "systemcalls.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

    int ret;
    ret=system(cmd);
    if (WIFSIGNALED (ret) && (WTERMSIG (ret) == SIGINT || WTERMSIG (ret) == SIGQUIT))
                return(false);
    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count,...)
{
    va_list args;
    va_start(args, count);
    pid_t pid;
    int ret=0,status,i;

    char * command[count+1];
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

     pid=fork();
    if(pid==-1){
        perror("fork");
        return false;
    }
    else if(pid==0){    
    ret=execv(command[0],command);
    if(ret == -1){
    perror("execv");
    exit (EXIT_FAILURE);
    }
    }
    if(waitpid (pid, &status, 0)==-1){
        perror("waitpid");
        return false;

    }
    if(WIFEXITED(status)){
    if(WEXITSTATUS(status)!=0)
        return false;
    else
        return true;    
    }      
    va_end(args);
    return(false);
  
}
/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    pid_t pid;
    va_list args;
    int ret=0;
    va_start(args, count);
    char * command[count+1];
    //char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];
    int status;
    int fd = open(outputfile, O_RDWR|O_CREAT, 0644);
    if (fd < 0) 
    { perror("open"); 
    exit(-1);
    }

    pid=fork();
    if(pid==-1){
        perror("fork");
        return false;
    }
    else if(pid==0){ 
   if (dup2(fd, 1) < 0){ 
    perror("dup2"); 
    exit(-1); }
    close(fd);
    ret=execv(command[0],command);
    if(ret == -1){
    perror("execv");
    exit (EXIT_FAILURE);
    }
    exit (EXIT_FAILURE);
    }
    pid= waitpid (pid, &status, 0);
    if(pid==-1){
        perror("waitpid");    
    }

    if(WIFEXITED(status)){
    va_end(args);    
    if(WEXITSTATUS(status)!=0)
        return false;
    else
        return true;    
    }      

    return(false);    
    va_end(args);
}

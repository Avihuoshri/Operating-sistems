
// C program to illustrate 
// pipe system call in C 
// shared by Parent and Child 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include "md5.h"
#include <iostream>
#define  MSGSIZE 20 
using namespace std;
 
 int pid =-1;
 char inbuf[32]; 
 bool checkSize=false;

void sigCatch(int sum_sig)
{
  if(strlen(inbuf)==32)
  {
      checkSize=true;
  }
  else
      checkSize=false;
};

int main() 
{ 

    string resMsg;
    
    int pr2child[2],child2pr[2] ,myPid, nbytes; 
    
    if (pipe(pr2child) < 0) 
        exit(1); 
     if (pipe(child2pr) < 0) 
        exit(1);     
  
      char input_str[20];
     printf("Get msg from user: ");
      scanf("%s",input_str);
    

     if ((myPid = fork()) < 0) 
     { 
        perror("fork"); 
        exit(1); 
    }
    
    if (myPid==0) {
        
        close(pr2child[1]);
        read(pr2child[0],inbuf,20);
       
          resMsg=md5(inbuf);
        close(pr2child[0]); 
        close(child2pr[0]); 
        sleep(2);
         
         write(child2pr[1],resMsg.c_str(),32);
         close(child2pr[1]);
         pid = getppid();
        kill(pid,SIGINT);
         sleep(2);

         while(1){sleep(2);};//so that the child will not be a zombie
      exit(0);
        
    }
    else//parent process
    {
         pid = getpid();
         signal(SIGINT,sigCatch);
        close(pr2child[0]);

        write(pr2child[1],input_str,strlen(input_str)+1);
    
        close(pr2child[1]);
        
        close(child2pr[1]); 
        sleep(2);

         read(child2pr[0],inbuf,33);
          close(child2pr[0]);

                sleep(2);
                if(checkSize==true)
                {
                    printf("encrypted by process %d: %s",getpid(),inbuf);
                 }
            
           
           kill(myPid,SIGTERM);

            wait(NULL); 
            
    }
    return 0; 
  } 
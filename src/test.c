
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/wait.h>   
#include <signal.h>  



int main(int argc , char *argv[]) {
     if(argc != 2){
         puts("arguments not match");
         puts("Example: ./test 10");
         return 0;
     }


    int proc_count = atoi(argv[1]);
    pid_t pid_list[proc_count];
    

        
    for(int i=0;i<proc_count;i++){
        int pid = fork();
        pid_list[i]=pid;

        if(pid == 0) {
            char *const parmList[] = {NULL};
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            execv("client",parmList);
            printf("ERROR pid %d\n",pid);
            

        } else {

            printf("sucess number %d pid %d\n",i+1,pid_list[i]);
        }       
    }

     return 0;
}

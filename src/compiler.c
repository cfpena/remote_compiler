#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <header.h>
#include <sys/shm.h>
#include <sys/sem.h>	   
#include <sys/wait.h>

int shm_program_id;
char * shm_program_addr;
int *prog_num;

struct program * prog_table;
int sem_prog_set_id;


//compile basic programs in c 
void compile(char prog_file[]){
      
      pid_t pid;
      char prog_name[50];
      int status;

      strcpy(prog_name, prog_file); 
      strcpy(prog_name,strtok(prog_name, "."));


      char *const parmList[] = {"/usr/bin/gcc", prog_file, "-o", prog_name, NULL}; //parameters to get executed with execv

      if ((pid = fork()) == -1)   //catching for error or getting pid of fork
          perror("Error en fork");
      else if (pid == 0) {     //exec file in child process
          execv("/usr/bin/gcc", parmList);
      }else{
          wait(&status);
          if(status==0)
              printf("program compiled sucessful!\n");
          else 
              printf("compile program fails!\n"); //when execv fails
      }
}

int main() {
    

    /* PROGRAM LIST STRUCT SEGMENT ------------------------------------------------*/

    shm_program_id = shmget(SH_MEM_PROGRAM_ID, sizeof(struct program)*2500, IPC_EXCL | 0600);
    if (shm_program_id == -1) {
        perror("main: shmget: ");
        
    }

    /* attach the shared memory segment to our process's address space. */
    shm_program_addr = shmat(shm_program_id, NULL, 0);
     if (!shm_program_addr) { /* operation failed. */
        perror("main: shmat: ");
        
    }
    

    prog_num = (int*) shm_program_addr;
    prog_table = (struct program*) ((void*)shm_program_addr+sizeof(int));

    /* ----------------------------------------------------------------------*/

    int counter=0;
    while(1){
        

        /* find registers with status SOURCE */
        for(int i=0; i<*prog_num;i++){
            struct program *prog = &prog_table[i];

            if((*prog).status==SOURCE){  
                
                compile((*prog).prog_path);    
                update_prog_status(sem_prog_set_id,prog,COMPILED); //update status to compiled

            }
        }

        if(counter>=10){
            counter=0;
            print_program_table(prog_table,prog_num);
        }

        counter++;
        sleep(1);
    }
}


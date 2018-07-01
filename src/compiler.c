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
#include <pthread.h> 

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

//handler for thread to compile
void * compiler_handler(void * ptr){
    int counter=0;
    while(1){
        
        sleep(SLEEP_P);
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
            //print_program_table(prog_table,prog_num);
        }

        counter++;
        
    }

}

//handler for thread that execute program
void * runner_handler(void * ptr){
    int counter=0;
    while(1){

        sleep(SLEEP_P*2);
        

        /* find registers with status COMPILED */
        for(int i=0; i<*prog_num;i++){
            struct program *prog = &prog_table[i];

            if((*prog).status==COMPILED){  
                

                int link[2];
                pid_t pid;
                char response[4096];

                pipe(link);
                    

                pid = fork();

                if(pid == 0) {

                    dup2 (link[1], STDOUT_FILENO);
                    close(link[0]);
                    close(link[1]);
                    char *const parmList[] = {NULL};
                    execv(strtok((*prog).prog_path,"."),parmList);
                    printf("ERROR\n");
                    

                } else {

                    close(link[1]);
                    read(link[0], response, sizeof(response));
                    printf("Output: %s\n", response);
                    strcpy((*prog).response,response);
                    update_prog_status(sem_prog_set_id,prog,RUNNED); //update status to runned
                    int status;
                    wait(&status);
                    if(status==0)
                        printf("program executed sucessful!\n");
                    else 
                        printf("program execute fails!\n"); //when execv fails
                    print_program_table(prog_table,prog_num);   



                }        
                




                
                

            }
        }

        if(counter>=10){
            counter=0;
            //print_program_table(prog_table,prog_num);
        }

        counter++;
        
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

    pthread_t compiler_thread,runner_thread;
    
    // start the threads
    pthread_create(&compiler_thread, NULL, *compiler_handler, (void *) 1);
    pthread_create(&runner_thread, NULL, *runner_handler, (void *) 2);
    
    // wait for threads to finish
    pthread_join(compiler_thread,NULL);
    pthread_join(runner_thread,NULL);
    


    
}


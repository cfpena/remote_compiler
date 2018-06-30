#include <header.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/wait.h>   
#include <sys/shm.h>
#include <signal.h>  
#include <sys/sem.h>
#include <sys/ipc.h>

int shm_request_id, shm_program_id;
char* shm_request_addr, *shm_program_addr;
struct shmid_ds shm_request_desc;
struct shmid_ds shm_program_desc;

void signal_handler() 
{
    
    
    //de-allocate the shared memory segment.
    if (shmctl(shm_request_id, IPC_RMID, &shm_request_desc) == -1) {
        perror("main: shmctl: ");
    }
    //de-allocate the shared memory segment.
    if (shmctl(shm_program_id, IPC_RMID, &shm_program_desc) == -1) {
        perror("main: shmctl: ");
    }

    printf("\nBye bye\n");
    exit(0);
}

int main() {
    //compile("helloworld.c");
    signal(SIGINT,signal_handler);  

    /*  REQUEST LIST STRUCT MEMORY SHARE -----------------------------------------------------------------*/

    /* allocate a shared memory segment */
    /* accessible only to the current user. */
    shm_request_id = shmget(SH_MEM_REQUEST_ID, sizeof(struct request)*2500, IPC_CREAT | IPC_EXCL | 0600);
    if (shm_request_id == -1) {
        perror("main: shmget: ");
        exit(1);
    }   

    /* attach the shared memory segment to our process's address space. */
    shm_request_addr = shmat(shm_request_id, NULL, 0);
     if (!shm_request_addr) { /* operation failed. */
        perror("main: shmat: ");
        exit(1);
    }
    

    int *req_num;
    //struct request* req_table;

    /* Initialize index of request array */
    req_num = (int*) shm_request_addr;
    *req_num=0;

    /*  --------------------------------------------------------------------------------------------------*/

    

    /*  PROGRAM LIST STRUCT MEMORY SHARE -----------------------------------------------------------------*/

    /* allocate a shared memory segment */
    /* accessible only to the current user. */
    shm_program_id = shmget(SH_MEM_PROGRAM_ID, sizeof(struct program)*2500, IPC_CREAT | IPC_EXCL | 0600);
    if (shm_program_id == -1) {
        perror("main: shmget: ");
        exit(1);
    }   

    /* attach the shared memory segment to our process's address space. */
    shm_program_addr = shmat(shm_program_id, NULL, 0);
     if (!shm_program_addr) { /* operation failed. */
        perror("main: shmat: ");
        exit(1);
    }
    

    int *prog_num;
    //struct program* prog_table;

    /* Initialize index of request array */
    prog_num = (int*) shm_program_addr;
    *prog_num=0;

    /*  --------------------------------------------------------------------------------------------------*/


    /* SEMAPHORE INITIALIZATION FOR PROGRAM ---------------------------------------------------------------*/
    int sem_set_id;
    union semun sem_val; 
    int rc;

    sem_set_id = semget(SEM_PROGRAM_ID, 1, IPC_CREAT | 0600);
    if (sem_set_id == -1) {
        perror("main: semget");
    	exit(1);
    }

    sem_val.val = 1;
    rc = semctl(sem_set_id, 0, SETVAL, sem_val);
    if (rc == -1) {
        perror("main: semctl");
        exit(1);
    }

    /* ------------------------------------------------------------------------------------------------------*/



    /* SEMAPHORE INITIALIZATION FOR REQUEST ---------------------------------------------------------------*/
    

    sem_set_id = semget(SEM_REQUEST_ID, 1, IPC_CREAT | 0600);
    if (sem_set_id == -1) {
        perror("main: semget");
    	exit(1);
    }

    sem_val.val = 1;
    rc = semctl(sem_set_id, 0, SETVAL, sem_val);
    if (rc == -1) {
        perror("main: semctl");
        exit(1);
    }

    /* ------------------------------------------------------------------------------------------------------*/
    
    
    
    int pid_server = fork();
    

    if (pid_server > 0) { //parent

        
        
        int pid_compiler = fork();

        if (pid_compiler>0){
            printf("Server pid: %d\n",pid_server);
            printf("Compiler pid: %d\n",pid_compiler);
            
            int status;
            waitpid(pid_server, &status, 0);
            waitpid(pid_compiler, &status, 0);
            if (shmctl(shm_request_id, IPC_RMID, &shm_request_desc) == -1) {
                perror("main: shmctl: ");
            }
            if (shmctl(shm_program_id, IPC_RMID, &shm_program_desc) == -1) {
                perror("main: shmctl: ");
            }        
            
            
        }else{
            printf("Executing compiler...\n");
            char *const parmList[] = {NULL}; 
            execv("compiler", parmList);
            printf("Fails to start compiler"); //when execv fails
        }

        

    } else { //child
        
        //execute server
        printf("Execting server...\n");
        char *const parmList[] = {NULL}; 
        execv("server", parmList);
        printf("Fails to start server"); //when execv fails
       
    }    




    return 0;
    
}

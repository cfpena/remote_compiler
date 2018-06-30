#include <stdio.h>
#include <header.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>	
#include <sys/shm.h>
#include <sys/ipc.h>



void sem_lock(int sem_set_id)
{
    /* structure for semaphore operations.   */
    struct sembuf sem_op;

    /* wait on the semaphore, unless it's value is non-negative. */
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

void sem_unlock(int sem_set_id)
{
    /* structure for semaphore operations.   */
    struct sembuf sem_op;

    /* signal the semaphore - increase its value by one. */
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;   /* <-- Comment 3 */
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}


void add_request(int sem_set_id, struct request* request_table,int *request_num, 
                  int user_id,char response[],char prog_path[],char source_path[],int order ){
    
    sem_lock(sem_set_id);

    request_table[*request_num].user_id = user_id;
    strcpy(request_table[*request_num].response, response);
    request_table[*request_num].order = order;

    (*request_num)++;

    sem_unlock(sem_set_id);

}

void print_request(struct request req){
    printf("id: %d  response: %s  order: %d\n",
        req.user_id,
        req.response,
        req.order
    );
}

void print_request_table(struct request* request_table,int *request_num){
    
    for(int i=0; i<*request_num;i++){
        struct request req = request_table[i];
        print_request(req);
    }

}


void add_program(int sem_set_id, struct program* program_table,int *program_num, 
                  char filename[],char prog_path[],char source_path[],enum status_choice status ){
    
    sem_lock(sem_set_id);
    
    strcpy(program_table[*program_num].filename, filename);
    strcpy(program_table[*program_num].prog_path, prog_path);
    strcpy(program_table[*program_num].source_path, source_path);
    program_table[*program_num].status = status;

    (*program_num)++;

    sem_unlock(sem_set_id);

    

}


void print_program(struct program prog){

    printf("filename: %s  path: %s  source: %s  status: %d\n",
        prog.filename,
        prog.prog_path,
        prog.source_path,
        prog.status
    );
}

void print_program_table(struct program* program_table,int *program_num){
    
    for(int i=0; i<*program_num;i++){
        struct program prog = program_table[i];
        print_program(prog);
    }

}

void update_prog_status(int sem_set_id,struct program *prog, enum status_choice status){
    
    sem_lock(sem_set_id);
    
    (*prog).status = status;

    sem_unlock(sem_set_id);
}
#include <time.h>
#include <sys/types.h>

#define SH_MEM_REQUEST_ID 1036
#define SH_MEM_PROGRAM_ID 1037
#define PORT 8080

#define SEM_REQUEST_ID    250
#define SEM_PROGRAM_ID    251

#define SLEEP_P 5

enum status_choice{ SOURCE , COMPILED , RUNNED };


struct request {
    int user_id;
    //time_t datetime;
    
    int order; 
};

struct program {
    char filename[200];
    enum status_choice status;
    char prog_path[200];
    char source_path[200];
    char response[2000];
};

union semun {              /* semaphore value, for semctl().     */
                int val;
                struct semid_ds *buf;
                ushort * array;
};

void compile(char prog_file[]);

void sem_lock(int sem_set_id);
void sem_unlock(int sem_set_id);

void add_request(int sem_set_id, struct request* request_table,int *request_num, 
                  int user_id,int order );
void print_request(struct request req);
void print_request_table(struct request* request_table,int *request_num);

void print_program_table(struct program* program_table,int *program_num);
void print_program(struct program prog);
void add_program(int sem_set_id, struct program* program_table,int *program_num, 
                  char filename[],char prog_path[],char source_path[],enum status_choice status );
void update_prog_status(int sem_set_id,struct program *prog, enum status_choice status);
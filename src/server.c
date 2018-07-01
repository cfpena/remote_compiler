#include <stdio.h>
#include <string.h>    
#include <stdlib.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>  
#include <pthread.h> 
#include <header.h>
#include <sys/shm.h>
#include <sys/sem.h>	





int shm_request_id, shm_program_id;
char* shm_request_addr, * shm_program_addr;
int *req_num, *prog_num;
struct request* req_table;
struct program * prog_table;
int sem_prog_set_id, sem_req_set_id;
 
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    
    /* REQUEST LIST STRUCT SEGMENT ------------------------------------------------*/

    shm_request_id = shmget(SH_MEM_REQUEST_ID, sizeof(struct request)*2500, IPC_EXCL | 0600);
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
    

    req_num = (int*) shm_request_addr;
    req_table = (struct request*) ((void*)shm_request_addr+sizeof(int));

    /* ----------------------------------------------------------------------*/


    /* PROGRAM LIST STRUCT SEGMENT ------------------------------------------------*/

    shm_program_id = shmget(SH_MEM_PROGRAM_ID, sizeof(struct program)*2500, IPC_EXCL | 0600);
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
    

    prog_num = (int*) shm_program_addr;
    prog_table = (struct program*) ((void*)shm_program_addr+sizeof(int));

    /* ----------------------------------------------------------------------*/


    /* PROGRAM SEMAPHORE SEGMENT -------------------------------------------------*/

    sem_prog_set_id = semget(SEM_PROGRAM_ID, 1, IPC_CREAT | 0600);
    if (sem_prog_set_id == -1) {
        perror("main: semget");
    	exit(1);
    }

    /*----------------------------------------------------------------------------*/

    /* PROGRAM SEMAPHORE SEGMENT -------------------------------------------------*/

    sem_req_set_id = semget(SEM_REQUEST_ID, 1, IPC_CREAT | 0600);
    if (sem_req_set_id == -1) {
        perror("main: semget");
    	exit(1);
    }

    /*----------------------------------------------------------------------------*/



    
    

    //detach the shared memory segment from our process's address space.
    // if (shmdt(shm_request_addr) == -1) {
    //     perror("main: shmdt: ");
    // }


    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) //manage socket creation fails
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];
    
    char file_name[2000];
    char file_content[10000];
    FILE *fp;

    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        client_message[read_size]='\0';   
        printf("command: %s\n",client_message);
        
        
        if( strcmp(client_message,"START_SEND_FILE")==0 )
        {
            puts("Starting protocol to retrieve file");

            puts("Waiting for filename");

            write(sock , "OK" , strlen("OK"));

            
            if ( (read_size = recv(sock , file_name , 2000 , 0)) < 0){ //waiting for filename
               puts("protocol wait filename fails");
            }
            file_name[read_size]='\0';
            
            puts("Waiting for file content");            
            
            write(sock , "OK" , strlen("OK"));

            
            if ( (read_size = recv(sock , file_content , 2000 , 0)) < 0){ //waiting for file content
               puts("protocol wait file content fails");
            }
            file_content[read_size]='\0';
            
            fp = fopen(file_name,"w"); //write file content
            //add_request( req_table,req_num, 1,"","",file_name,1);
            
            
            if(fp==NULL){
                puts("write file fails");
            }

            fprintf(fp,"%s",file_content);
            fclose(fp);

            puts("Write entry in program table");

            write(sock , "OK" , strlen("OK"));

            
            //add_program(sem_prog_set_id,prog_table,prog_num,strtok(file_name, "."),"",file_name,SOURCE);
            add_program(sem_prog_set_id,prog_table,prog_num,file_name,file_name,file_name,SOURCE);
            print_program_table(prog_table,prog_num);

            puts("Finishing protocol");

            

        }
        else if(strcmp(client_message,"X_LIST_PROGRAMS")==0){
            //char prog_list[*prog_num][200];
            puts("Listing programs");
            if(*prog_num<1)
            {   
               write(sock , "OK" , strlen("OK"));
            }
            else{
                for(int i=0; i<*prog_num;i++){
                    write(sock , prog_table[i].filename , strlen(prog_table[i].filename));
                    strcpy(client_message,"");
                    recv(sock , client_message , 2000 , 0);
                }
                write(sock , "OK" , strlen("OK"));
                
            }

            

        }
        else if(strcmp(client_message,"START_RUN_PROGR")==0){
            client_message[read_size]='\0';

            int index;
            write(sock , "OK" , strlen("OK"));

            
            read_size = recv(sock , client_message , 2000 , 0);
            client_message[read_size]='\0';

            index = atoi(client_message);

            int flag[3];
            

            while(1){
                usleep(100);
                switch(prog_table[index-1].status){
                    case SOURCE:
                        if(!flag[0]){
                            write(sock , "Waiting for compilation..." , strlen("Waiting for compilation..."));
                            read_size = recv(sock , client_message , 2000 , 0);
                            flag[0]=1;
                        }
                        break;
                    case COMPILED:
                        if(!flag[1]){
                            write(sock , "Already compiled" , strlen("Already compiled"));
                            read_size = recv(sock , client_message , 2000 , 0);
                            write(sock , "Waiting for execute..." , strlen("Waiting for execute..."));
                            read_size = recv(sock , client_message , 2000 , 0);
                            flag[1]=1;
                        }
                        break;
                    case RUNNED:
                        if(!flag[2]){

                            
                            
                            write(sock , "Already executed" , strlen("Already executed"));
                            read_size = recv(sock , client_message , 2000 , 0);

                            
                            
                            write(sock , "RESULT: " , strlen("RESULT: "));
                            read_size = recv(sock , client_message , 2000 , 0);

                            

                            write(sock , prog_table[index-1].response , strlen(prog_table[index-1].response));
                            read_size = recv(sock , client_message , 2000 , 0);

                            
                            
                            flag[2]=1;
                            write(sock , "END" , strlen("END"));

                            
                            
                        }
                        break;

                }
                
                if(prog_table[index-1].status==RUNNED) break;
                
                
            }
            client_message[read_size]='\0';
            for (int i=0;i<sizeof(flag);i++) flag[i]=0;
            

            
            

        } 
        else if(strcmp(client_message,"SHUTDOWN_SERVER")==0){
            puts("Killing server...");
            close(sock);
            exit(0);

        }
        client_message[read_size]='\0';
           
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}



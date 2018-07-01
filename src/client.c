/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>
#include <header.h>
 
#define CLEAR  "\x1B[2J"


int sock;
struct sockaddr_in server;


void send_file(char *path);
void run_program(char *path);
void main_menu();
void send_sock_message(char *message);
void receive_sock_message();
const char* getFileNameFromPath(const char* path);
void list_programs();

int main(int argc , char *argv[])
{
    
    
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( PORT );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    main_menu();
     
    close(sock);
    return 0;
}


void main_menu(){
    int option=0;
    char path[200];

    while(1){
        printf("\nMENU:\n");
        printf("    1. SEND FILE\n");
        printf("    2. LIST PROGRAMS\n");
        printf("    3. RUN PROGRAM\n");
        printf("    4. KILL SERVER\n");

        printf("Enter option : ");
        scanf("%i" , &option);
        fflush(stdin);

        switch(option){
            case 1:
                
                puts(CLEAR);
                printf("Enter absolute path: ");
                scanf("%s" , path);
                fflush(stdin);
                strcpy(path,path);
                
                send_file(path);
                break;
            case 2:
                puts(CLEAR);
                list_programs();
                break;
            case 3:
                puts(CLEAR);
                printf("Enter program number: ");
                scanf("%s" , path);
                fflush(stdin);
                strcpy(path,path);
                run_program(path);
                break;


            case 4:
                puts(CLEAR);
                printf("Killing server... ");
                send_sock_message("SHUTDOWN_SERVER");
                break;
        }
    }

}

void list_programs(){
    
    char server_reply[2000];
    int count=0;
    send_sock_message("X_LIST_PROGRAMS");
    
    puts("List of programs:");
    while(1){
        server_reply[0]='\0';
        receive_sock_message(&server_reply);
        
        if(strcmp(server_reply,"OK")==0){
            printf("%d program(s) in memory",count);
            break;
        }else{
            printf("%d. %s\n",count+1,server_reply);
            send_sock_message("OK");
            count++;
        }
    }

    
}
void run_program(char *path){
    char message[1000] , server_reply[2000];
    strcpy(message,"START_RUN_PROGR"); //start protocol
    send_sock_message(message);
    receive_sock_message(&server_reply);    

    if(strcmp(server_reply,"OK")!=0){
        puts("send file protocol fails");
    }

    strcpy(message,path); //send index of choice
    send_sock_message(message);
    

    while(1){
        receive_sock_message(&server_reply);

        if(strcmp(server_reply,"END")==0){
             break;
        }else{
            printf("%s\n",server_reply);
            send_sock_message("OK");
            }
        
    }

    


}

void send_file(char *path){

    char message[1000] , server_reply[2000];
    FILE *fp; 
    char *fcontent;
    int size;
    

    
    
    strcpy(message,"START_SEND_FILE"); //start protocol
    send_sock_message(message);
    receive_sock_message(&server_reply);    

    if(strcmp(server_reply,"OK")!=0){
        puts("send file protocol fails");
    }

        

    strcpy(message,getFileNameFromPath(path)); //send file name
    send_sock_message(message);
    receive_sock_message(&server_reply);

    if(strcmp(server_reply,"OK")!=0){
        puts("send file name fails");
    }
    

    fp=fopen(path,"r");  //send file content

    if(fp==NULL){
        puts("\n404 File not found or unreadable!!!\n");
        return;
    }

    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 
    fcontent = malloc(size);
    fread(fcontent, 1, size, fp);
    
    strcpy(message,fcontent);

    fclose(fp);
    send_sock_message(message);
    receive_sock_message(&server_reply);
    
}

void send_sock_message(char message[]){
    
    if( send(sock , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
    }
}

void receive_sock_message(char * server_reply){
    int read_size;
    
    if( (read_size = recv(sock , server_reply , 2000 , 0)) < 0)
    {
        puts("recv failed");
    }
    server_reply[read_size]='\0'; //sure to clean buffer excess
    
}


const char* getFileNameFromPath(const char* path)
{
   for(size_t i = strlen(path) - 1;  i >= 0; i--)
   {
      if (path[i] == '/')
      {
         return &path[i+1];
      }
   }

   return path;
}
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
void main_menu();
void send_sock_message(char *message);
void receive_sock_message();
const char* getFileNameFromPath(const char* path);

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
        printf("    2. COMPILE PROGRAM\n");
        printf("    3. RUN PROGRAM\n");
        printf("    4. KILL SERVER\n");

        printf("Enter option : ");
        scanf("%i" , &option);
        fflush(stdin);

        switch(option){
            case 1:
                puts(CLEAR);
                printf("Entre absolute path: ");
                scanf("%s" , path);
                fflush(stdin);
                strcpy(path,path);
                
                send_file(path);
                break;

            case 4:
                puts(CLEAR);
                printf("Killing server... ");
                send_sock_message("SHUTDOWN_SERVER");           
                break;
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
    
    if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    
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
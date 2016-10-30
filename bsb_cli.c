#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>    
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define MAXDATASIZE 100   
/* El número máximo de datos en bytes */

typedef struct{
    char option;
    int code;
    int money;
} transaction;


int main (int argc, char * argv[]){
    
   if (argc != 9){
      printf("Argumentos incompletos, %d de 8 esperados\n", argc-1);
   }

   else{

    int  port = atoi(argv[4]);
    char op = * argv [6];
    int userCode = atoi(argv[8]);  
    transaction buff;
    buff.option = * argv[6];
    buff.code = userCode;
    buff.money = 1;

    int fd, numbytes;

   /* ficheros descriptores */

    char buf[MAXDATASIZE];  
   /* en donde es almacenará el texto recibido */

    struct hostent *he;         
   /* estructura que recibirá información sobre el nodo remoto */

    struct sockaddr_in server;  
   /* información sobre la dirección del servidor */


   if ((he=gethostbyname(argv[2]))==NULL){       
      /* llamada a gethostbyname() */
      perror("error en gethostbyname()\n");
      exit(-1);
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
      /* llamada a socket() */
      perror("error en socket()\n");
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(port); 
   /* htons() es necesaria nuevamente ;-o */
   server.sin_addr = *((struct in_addr *)he->h_addr);  
   /*he->h_addr pasa la información de ``*he'' a "h_addr" */
   bzero(&(server.sin_zero),8);

   if(connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr))==-1){ 
      /* llamada a connect() */
      perror("error en connect()\n");
      exit(-1);
   }

   if ((numbytes=write(fd,&buff,sizeof(buff))) == -1){  
      /* llamada a recv() */
      perror(" error Error \n");
      exit(-1);
   }

   //buf[numbytes]='\0';

   //printf("Mensaje del Servidor: %s\n",buff); 
   /* muestra el mensaje de bienvenida del servidor =) */

   close(fd);   /* cerramos fd =) */
    
   }

}
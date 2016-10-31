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
    char message[60];
} transaction;


int main (int argc, char * argv[]){
    
   if (argc != 9){
      printf("Argumentos incompletos, %d de 8 esperados\n", argc-1);
   }

   else{

    int  port = atoi(argv[4]);
    int userCode = atoi(argv[8]);  
    transaction buff;
      buff.option = * argv[6];
      buff.code = userCode;

    char * c_time_string;
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

   int count = 0;
   int temp = -1;
   while(temp = (connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr)))==-1 || count >= 3){ 
      /* llamada a connect() */
      count += 1;
      if (count == 3){
          perror("Agotado tiempo de espera, error en connect()\n");
          exit(-1);
      }
   }

   /*Comunicacion y primer envio infromacion al servidor*/
   /* se envia el buff para informar, codigo usuario, tipo operacion*/
   if ((numbytes=write(fd,&buff,sizeof(buff))) == -1){  
      /* llamada a recv() */
      perror(" error Error \n");
      exit(-1);
   }

  switch(buff.option){
    case 'd':
       printf ("Bienvenido usuario, ingrese la cantidad a depositar: "); //
       break;

    case 'r':
       printf ("Bienvenido usuario, ingrese la cantidad a retirar: "); //
       break;
   }
       //Interaccion del cajero con la central bancaria.
       scanf("%d",&buff.money);//Se lee la cantidad de dinero a depositar/retirar 
       write(fd,&buff.money, sizeof(buff.money)); //se envia la cantidad al servidor
       read(fd,buff.message,100); //lectura del ticket comprobante
       printf("Transaccion realizada con exito\n");
       printf("Ticket: \n");
       printf("       Codigo: %d, Operacion: %c, Fecha: %s",buff.code, buff.option, buff.message);

   close(fd);   /* cerramos fd =) */

    
   }

}
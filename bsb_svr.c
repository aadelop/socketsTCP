#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>    
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 2 /* El número de conexiones permitidas */

typedef struct{
    char option;
    int code;
    int money;
} transaction;

int main (int argc, char * argv[]){
	 int  port;
   int numbytes;
   transaction buff;
	 char * bit_deposito;
	 char * bit_retiro;  
    
    int fd, fd2; /* los ficheros descriptores */

    struct sockaddr_in server; 
   /* para la información de la dirección del servidor */

    struct sockaddr_in client; 
   /* para la información de la dirección del cliente */
    int sin_size;
    
    if (argc != 7){
        printf("Argumentos incompletos, %d de 6 esperados\n", argc-1);
    }
    else {
        if (strcmp(argv[1],"-l")==0){
            printf ("primer if \n");
            printf ("%s\n",argv[1]);
            port = atoi(argv[2]);
            if (strcmp(argv[3],"-i")){
                bit_deposito = argv[4];
                bit_retiro = argv[6];
            }
            else{
                bit_deposito = argv[6];
                bit_retiro = argv[4];
            }
        }       
        else if (strcmp(argv[1],"-i")==0){
            printf("segundo if\n");
            bit_deposito = argv[2]; 
            if (strcmp(argv[3],"-l")){
                port = atoi(argv[4]);
                bit_retiro = argv[6];
            }
            else{
                port = atoi(argv[6]);
                bit_retiro = argv[4];
            } 
        }
        else if (strcmp(argv[1],"-o")==0){
            printf("tercer if\n");
            bit_retiro = argv[2];   
            if (strcmp(argv[3],"-l")){ 
                port = atoi(argv[4]);  
                bit_deposito = argv[6];
            }
            else{
                port = atoi(argv[6]); 
                bit_deposito = argv[4];
            }
        }
    }

    

   /* A continuación la llamada a socket() */
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
      perror("error en socket()\n");
      exit(-1);
   }

   server.sin_family = AF_INET;         

   server.sin_port = htons(port); 
   /* ¿Recuerdas a htons() de la sección "Conversiones"? =) */

   server.sin_addr.s_addr = INADDR_ANY; 
   /* INADDR_ANY coloca nuestra dirección IP automáticamente */

   bzero(&(server.sin_zero),8); 
   /* escribimos ceros en el reto de la estructura */


   /* A continuación la llamada a bind() */
   if(bind(fd,(struct sockaddr*)&server,
           sizeof(struct sockaddr))==-1) {
      perror("error en bind() \n");
      exit(-1);
   }     

   if(listen(fd,BACKLOG) == -1) {  /* llamada a listen() */
      perror("error en listen()\n");
      exit(-1);
   }

   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() */
      if ((fd2 = accept(fd,(struct sockaddr *)&client,
                        &sin_size))==-1) {
         perror("error en accept()\n");
         exit(-1);
      }

   if (numbytes=(read(fd2,&buff,100) == -1)){  
      perror(" error Error \n");
      exit(-1);
    }
      
      printf("Se obtuvo una conexión desde %s\n",
             inet_ntoa(client.sin_addr) ); 
      /* que mostrará la IP del cliente */
      printf("operacion de usuario : %c \n" ,buff.option);

      close(fd2); /* cierra fd2 */
   }


}
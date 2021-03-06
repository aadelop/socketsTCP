#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>    
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define BACKLOG 2 /* El número de conexiones permitidas */

typedef struct{
    char option;
    int code;
    int money;
    char message[60];
} transaction;

int main (int argc, char * argv[]){
	 int  port,numbytes,i;
   int totalMoney = 80000;
   transaction buff;
   char * fname_dep;
   char * fname_ret; 
	 FILE * bit_dep;
	 FILE * bit_ret;

   time_t current_time;
   char * c_time_string;

   int fd, fd2,sin_size;
    /* los ficheros descriptores */

   struct sockaddr_in server; 
   /* para la información de la dirección del servidor */

   struct sockaddr_in client; 

    if (argc != 7){
        printf("Argumentos incompletos, %d de 6 esperados\n", argc-1);
    }
    else {
      i=0;
      while( i< 6 ) {
        if (strcmp(argv[i],"-l")==0){
          port = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i],"-i")==0){
          fname_dep = argv[i+1];
        }
        else if (strcmp(argv[i],"-o")==0){
          fname_ret = argv [i+1];
        }
        i++;
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


   //Cominezo de escucha de operaciones en servidor
   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() */
      if ((fd2 = accept(fd,(struct sockaddr *)&client,
                        &sin_size))==-1) {
         perror("error en accept()\n");
         exit(-1);
      }

    if ((read(fd2,&buff,100) == -1)){  
      perror(" error Error \n");
      exit(-1);
    }
      
      printf("Se obtuvo una conexión desde %s\n",
             inet_ntoa(client.sin_addr) ); 
      /* que mostrará la IP del cliente */
    switch(buff.option){
        case 'd':
          //generar funcion
            read(fd2,&buff.money,100);
            totalMoney += buff.money;
            bit_dep = fopen(fname_dep,"r+");//solo si existe archivo, añadir fopen +w si no existe 
            
            if (bit_dep == NULL )
              bit_dep = fopen(fname_dep,"w+");   
            current_time = time(NULL);
            c_time_string = ctime(&current_time);
            fseek(bit_dep,0,SEEK_END);
            fprintf(bit_dep, "usuario: %d, evento: %c, monto actual: %d, fecha: %s",
                                  buff.code, buff.option,totalMoney, c_time_string);
            fclose(bit_dep);
            strcpy(buff.message,c_time_string);
            write(fd2,buff.message,sizeof(buff.message));

            break;
        case 'r':
            read(fd2,&buff.money,100);
            totalMoney -= buff.money;
            bit_ret = fopen(fname_ret,"r+"); //solo si existe archivo, añadir fopen +w si no existe 
            if (bit_ret == NULL)
              bit_ret = fopen(fname_ret,"w+");
            current_time = time(NULL);
            c_time_string = ctime(&current_time);
            fseek(bit_ret,0,SEEK_END);
            fprintf(bit_ret, "usuario: %d, evento: %c, monto actual: %d, fecha: %s",
                                  buff.code, buff.option,totalMoney, c_time_string);
            fclose(bit_ret);
            strcpy(buff.message,c_time_string);
            write(fd2,buff.message,sizeof(buff.message));
            break;

        default:
            printf("invalid option\n");

    }
      close(fd2); /* cierra fd2 */

  }

}
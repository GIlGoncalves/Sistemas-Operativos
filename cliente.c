#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

int copiou=-10;

void recebeSinais(int sinal){  
    switch (sinal) {
        case SIGUSR1:
           copiou=1;
            break;
        case SIGUSR2: 
            copiou=0;
            break;
          default:
            break;
    }
}
int main(int argc, char const *argv[])
{
 	const char* toServidor="./toServidor";

  char * escreve=NULL;
  escreve = (char *)malloc(1024);
  
  signal(SIGUSR1,recebeSinais);
  signal(SIGUSR2,recebeSinais);

	int fifo;
  int i=0;

  char *pidUtilizador=NULL;
  pidUtilizador=(char *)malloc(1024);
	
  fifo=mkfifo(toServidor,0666);

	int fdEscrita = open(toServidor,O_WRONLY);
           if(strcmp(argv[1],"backup")==0 && argc>=3) {
                write(fdEscrita,"backup",strlen("backup")+1);
                sprintf(pidUtilizador,"%d",getpid());
                write(fdEscrita,pidUtilizador,1024);

                for(i=2;i<argc;i++) {                   
                    write(fdEscrita,argv[i],1024);
                }
                pause();
                for(i=2;i<argc;i++) {    
                    if(copiou==1) {
                        strcpy(escreve,argv[i]);
                        strcat(escreve,": copiado");
                        strcat(escreve,"\n");
                        write(0,escreve,strlen(escreve)+1);
                        }
                        else {
                          if(copiou==0) {
                              strcpy(escreve,"erro ao copiar o ficheiro ");
                              strcat(escreve,argv[i]);
                              strcat(escreve,"\n");
                              write(0,escreve,strlen(escreve)+1);
                          }
                          else {write(0,"não fez nada \n",strlen("não fez nada \n")+1);}
                        }
                }              
            }
            else{
              if(strcmp(argv[1],"restore")==0 && argc==3) {  
                    write(fdEscrita,"restore",strlen("restore")+1);
                    sprintf(pidUtilizador,"%d",getpid());
                    write(fdEscrita,pidUtilizador,1024);
                    write(fdEscrita,argv[2],1024); 
                    pause();
                    if(copiou==1) {
                          strcpy(escreve,argv[2]);
                          strcat(escreve,": recuperado");
                          strcat(escreve,"\n");
                          write(0,escreve,strlen(escreve)+1);
                    }
                    else {
                      if(copiou==0) {
                          strcpy(escreve,"erro ao recuperar o ficheiro ");
                          strcat(escreve,argv[2]);
                          strcat(escreve,"\n");
                          write(0,escreve,strlen(escreve)+1);
                      }
                      else {write(0,"não fez nada \n",strlen("não fez nada \n")+1);}
                    }
              }
              else {
                  if(strcmp(argv[1],"delete")==0 && argc==3) {
                      write(fdEscrita,"delete",strlen("delete")+1);
                      sprintf(pidUtilizador,"%d",getpid());
                      write(fdEscrita,pidUtilizador,1024);
                      write(fdEscrita,argv[2],1024);
                      pause();
                      if(copiou==1) {
                          strcpy(escreve,argv[2]);
                          strcat(escreve,": apagado\n");
                          write(0,escreve,strlen(escreve)+1);                            
                      }
                      else {
                        if(copiou==0) {
                              strcpy(escreve,"erro ao apagar o ficheiro ");
                              strcat(escreve,argv[2]);
                              strcat(escreve,"\n");
                              write(0,escreve,strlen(escreve)+1);               }
                        else {write(0,"não fez nada \n",strlen("não fez nada \n")+1);}
                      }
                  }
                  else {
                    if(strcmp(argv[1],"gc")==0 && argc==2){
                          write(fdEscrita,"gc",strlen("gc")+1);
                          sprintf(pidUtilizador,"%d",getpid());
                          write(fdEscrita,pidUtilizador,1024);
                          pause();
                          if(copiou==1) {
                              strcpy(escreve,"os ficheiro foram eliminados\n");
                              write(0,escreve,strlen(escreve)+1);      
                          }
                          else {
                            if(copiou==0) {
                                strcpy(escreve,"todos os ficheiros estão a ser utilizados\n");
                                write(0,escreve,strlen(escreve)+1);
                            }
                            else {write(0,"não fez nada \n",strlen("não fez nada \n")+1);}
                          }
                    }
                    else {
                        write(0,"erro\n",strlen("erro\n")+1);
                        close(fifo);
                        close(fdEscrita);
                        free(pidUtilizador);
                        _exit(1);}
                  }
              }
            }
    close(fifo);
    close(fdEscrita);
    free(pidUtilizador);
    
	return 0;
}

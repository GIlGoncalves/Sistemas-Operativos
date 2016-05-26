#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "listaLigada.h"
/*Vai retirar o \n do string*/
void my_strcpy(char *s, char *buf) {
	int i=0;
	while(buf[i]!='\n') {
		s[i]=buf[i];
		i++;
	}
	s[i]='\0';
}
void clearbuffer(char *buffer, int length){
    int i=0;
    while(i<length)
        buffer[i++]='\0';
}
/*vai ler a informação escrita no terminar e guarda num buffer e retorna esse buffer*/
int crearPastaData(){
	pid_t filho;
	int valor;
	int filhos;
		filho=fork();
		if(filho==-1) {perror("fork no criar pasta data");_exit(1);}
		if(filho==0) {
				execlp("mkdir","mkdir","data",NULL);
				perror("criar data");
				_exit(1);		
		}
		else {
				filhos=filho;
		}
		waitpid(filhos,&valor,0);
		if(WIFEXITED(valor)) {
				if(WEXITSTATUS(valor)) {
						return 1;
				}
				else return 0;
		}
    else {return 0;}
}
int crearPastaMetadata(){
	pid_t filho;
	int valor;
	int filhos;
		filho=fork();
		if(filho==-1) {perror("fork no criar pasta data");_exit(1);}
		if(filho==0) {
				execlp("mkdir","mkdir","metadata",NULL);
				perror("criar data");
				_exit(1);		
		}
		else {
				filhos=filho;
		}
		waitpid(filhos,&valor,0);
		if(WIFEXITED(valor)) {
				if(WEXITSTATUS(valor)) {
						return 1;
				}
				else return 0;
		}
    else {return 0;}
}
char * lerSha(char * terminal) {
	char * buf=NULL;
	int pid[2];
	pid_t filho;
	int pi;
	buf = (char*) malloc(1024);
	int filhos;
	int valor;
	pi=pipe(pid);

	if(pi==-1) {perror("pipe");_exit(1);}
		filho=fork();
		if(filho==-1) {perror("fork");_exit(1);}
				if(filho==0)
           			{
           				close(pid[0]);
           				dup2(pid[1],1);
           				close(pid[1]);
           				execlp("sha1sum","sha1sum",terminal,NULL);
           				perror("sha1sum");
           				_exit(1);
					}
					else
					{
						filhos=filho;
						close(pid[1]);
						dup2(pid[0],0);
						read(0,buf,1024);
						close(pid[0]);
 }
	waitpid(filhos,&valor,0);
				 		if(WIFEXITED(valor)) {	
				 				if(WEXITSTATUS(valor)!=1) {
				 							return buf;
				 					}
				 		else {return NULL;}
				 	}
					else {return NULL;}
}

int gzip(char * buffer) {
	char * s =NULL;
	char * nomeEstanho =NULL;
	char * nome=NULL;

	int pidfilho[2];
	int valor;
	pid_t filho;
	pid_t filho1;
	 s =(char *) malloc(1024);
	 nomeEstanho = (char *) malloc(1024);
	char *guarda[2];
	const char limitador[2]=" ";
	char *token=NULL;
	int i=0;
	nome=(char *) malloc(1024);
	token=strtok(buffer,limitador);
	 while( token != NULL ) 
   {
      guarda[i]=token;
      token = strtok(NULL,limitador);
      i++;
   }		
			filho=fork();
			if(filho==-1) {perror("primeiro fork");_exit(1);}
			if(filho==0) {
								filho1=fork();
								if(filho1==-1) {perror("segundo fork");_exit(1);}
									if(filho1==0) {
													my_strcpy(s,guarda[1]);
													execlp("gzip","gzip","-k","-f","-r",s,NULL);
														perror("gzip");
														_exit(1);
									}
										else {        /*espera que o filho acaba de comprimir muda o nome e guarda o seu pid*/
													/*ele quando comprime poem o nome.gz daí eu o esta a associar*/
													wait(0);
													pidfilho[0]=filho1;
													my_strcpy(nome,guarda[1]);
													strcat(nome,".gz");
													strcpy(nomeEstanho,guarda[0]);
													strcat(nomeEstanho,".gz");
													execlp("mv","mv",nome,nomeEstanho,NULL);
													perror("mudar o nome");
														_exit(1);
										}
			}
			else { /*espera pelo filho e guarda o seu pide*/
					wait(0);
					pidfilho[1]=filho;
					 free(nome);
					 free(s);
					 free(token);	
			}
/*espera que todos os filhos tenham todas terminado*/
	for(i=0;i<2;i++){
			waitpid(pidfilho[i],&valor,0);
}
/* cria outro fork para que não morra a execução do programa, assim o filho faz o exclp e o pai regressa a main*/
  int filhosR;
  filho=fork();
  if(filho==-1) {perror("3 fork"); _exit(1);}
  
  if(filho==0){
  				strcpy(nomeEstanho,guarda[0]);
				strcat(nomeEstanho,".gz");
	 			execlp("mv","mv",nomeEstanho,"data",NULL);
				perror("mudar de pasta");
				_exit(1);
			    }else{
					filhosR=filho;
				}

waitpid(filhosR,&valor,0);					

if(WIFEXITED(valor)) {			
				 		if(WEXITSTATUS(valor)==1) {				 						
				 			free(nomeEstanho);
				 			return 0;
				 		}else {free(nomeEstanho); return 1;}
					}
					else { free(nomeEstanho); return 0;}
}

int metadata(char * buffer) {
	char *c=NULL;
	char *k=NULL;
	char * s=NULL;
	char * nomeEstanho=NULL;

	char * guarda[2];
	int pidTmp=0;
	int valor;
	const char limitador[2]=" ";
	pid_t filho;
	pid_t filho1;
	char *token=NULL;
	int i=0;
	c=(char *) malloc(1024);
	k=(char *) malloc(1024);
	s =(char *) malloc(1024);
	strcpy(c,"../data/");
	nomeEstanho = (char *) malloc(1024);

	token=strtok(buffer,limitador);
	 while( token != NULL ) 
   {
      guarda[i]=token;
      token = strtok(NULL,limitador);
      i++;
   }
			filho=fork();
			if(filho==-1){perror("no fork do metadata");_exit(1);}
			if(filho==0) {									
							filho1=fork();

							if(filho1==-1){perror("no segundo fork do metadata");_exit(1);}
								                   
							if(filho1==0){
								           	strcpy(nomeEstanho,guarda[0]);
											strcat(nomeEstanho,".gz");
											strcat(c,nomeEstanho);
											my_strcpy(s,guarda[1]);
											strcat(s,".");			
											execlp("ln","ln","-s","-f",c,s,NULL);
											perror("ln");
											_exit(1);
								        }else {
												wait(0);
												pidTmp=filho1;
												my_strcpy(k,guarda[1]);
												strcat(k,".");		
												execlp("mv","mv",k,"metadata",NULL);
												perror("mover");
												_exit(1);
											}
			}
			else {   
					waitpid(pidTmp,&valor,0);
					filho=pidTmp;
			}
			waitpid(filho,&valor,0);
			free(nomeEstanho);
			free(token);
			free(c);
			free(s);
			free(k);
			if(WIFEXITED(valor)) {
							if (WEXITSTATUS(valor)!=1)
							{
								return 1;
							}
							else {					
								return 0;
							}
			}
			else {
						return 0;
			}
}
int mudarNome(char * buffer) {
const char limitador[2]=" ";
char *token=NULL;
char *guarda[2];
char *s=NULL;
s=(char*)malloc(1024);
pid_t filho;
int filhos;
int valor;
int i=0;
strcpy(s,"metadata/");
char * string=NULL;
string=(char*)malloc(1024);
char * mudar=NULL;
mudar=(char*)malloc(1024);
char * mudar1=NULL;
mudar1=(char*)malloc(1024);
strcpy(mudar1,"metadata/");

token=strtok(buffer,limitador);

	 while( token != NULL ) 
   {
      guarda[i]=token;
    
      token = strtok(NULL,limitador);
      i++;
   }
	filho=fork();  
	if(filho==-1){perror("fork no mudar nome");_exit(1);}

	if(filho==0) {
					my_strcpy(string,guarda[1]);
					my_strcpy(mudar,guarda[1]);
					strcat(mudar,".");
					strcat(s,mudar);
					strcat(mudar1,string);		

					execlp("mv","mv",s,mudar1,NULL);
					perror("mudar o nome");
					_exit(1);
	} 
	else {
		filhos=filho;
	}	
	waitpid(filhos,&valor,0);
	free(mudar);
	free(string);
	free(s);
	free(mudar1);

	if(WIFEXITED(valor)) {
		if (WEXITSTATUS(valor))
		{return 0;}
		else return 1;
	}
else {return 0;}
}

char * nomeFicheiroF(char * buffer) {
				int i=0;
				int lidos=0;
				const char limitador[2]="/";
				char *guarda[4];
				char *token=NULL;
				char * buf=NULL;
				char *ler=NULL;
				ler =(char *)malloc(1024);
				buf =(char *)malloc(1024);
				int pid[2];
				int p1;
				pid_t filho;
				pid_t filho1;
				int pid1[2];
				int p2;
				strcpy(buf,"metadata/");
				strcat(buf,buffer);
				p1=pipe(pid);
				if(p1==-1) {perror("pipe nomeFicheiro");_exit(1);}
				filho=fork();

				if(filho==-1) {perror("fork no nomeFicheiro");_exit(1);}
				if(filho==0) {

							p2=pipe(pid1);
							if(p2==-1) {perror("pipe2 nomeFicheiro");_exit(1);}	
							filho1=fork();
							if(filho1==-1) {perror("fork no nomeFicheiro");_exit(1);}

							if(filho1==0) {
											close(pid1[0]);
											dup2(pid1[1],1);
											close(pid1[1]);
											execlp("ls","ls","-l",buf,NULL);
											perror("ls nomeFicheiro");_exit(1);
									}
									else {	
											close(pid1[1]);
											dup2(pid1[0],0);
											dup2(pid[1],1);
											close(pid[1]);
											close(pid1[0]);
											close(pid[0]);
											execlp("awk","awk","{print $11}",NULL);
											perror("awk nomeFicheiro");_exit(1);					
											}
									}
					else {
							close(pid[1]);
							dup2(pid[0],0);
							close(pid[0]);
							lidos=read(0,ler,1024);
							if(lidos>0) {
											token=strtok(ler,limitador);
											while( token != NULL ) 
											{
											    guarda[i]=token;
											    token = strtok(NULL,limitador);
											    i++;
											}	   
											my_strcpy(ler,guarda[2]);
											free(buf);
											return ler;											  
								}
								else return NULL;
					}
}
int apagarMetadata(char * buf) {
	int valor;
	int filhos;
	pid_t filho;
	char * b=NULL;
	b=(char *)malloc(1024);
	strcpy(b,"metadata/");
	strcat(b,buf);

	filho=fork();
	if(filho==-1) {perror("fork no apagarMetadata");_exit(1);}
	if(filho==0) {
					execlp("rm","rm",b,NULL);
					perror("apagar o ficheiro");
					_exit(1);
	}
	else {
			filhos=filho;	
	}
	waitpid(filhos,&valor,0);
	free(b);

	if(WIFEXITED(valor)) {
		if(WEXITSTATUS(valor)!=1) {
							return 1;
		}
		else {return 0;}
		}
		else return 0;
}

int copiarFicheiro(char * buf) {
	char * s=NULL;
	s=(char *) malloc(1024);
	pid_t filho;
	int valor=0;
	int filhos=0;
	strcpy(s,"data/");
	strcat(s,buf);

	  filho=fork();
	  if(filho==-1) {perror("Fork no copiar ficheiro");_exit(1);}
	  if(filho==0) {
			execlp("cp","cp",s,buf,NULL);
	  		perror("cp");
	  		_exit(1);
 }
 		else{
 			filhos=filho;
 		}
 		waitpid(filhos,&valor,0);
		free(s);
		if(WIFEXITED(valor)) {
						if(WEXITSTATUS(valor)!=1) {		
							return 1;
						}
						else { return 0;}
		}
		else return 0;
}

int extrairFicheiro(char * buf) {
	pid_t filho;
	int filhos=0;
	int valor=0;
	filho=fork();

	if (filho==-1)
	{
		perror("fork no extrairFicheiro");
		_exit(1);
	}
	if(filho==0) {

				execlp("gunzip","gunzip",buf,NULL);
				perror("extrair ficheiro");
				_exit(1);
	}
	else {	
			filhos=filho;
		}
	waitpid(filhos,&valor,0);
	if(WIFEXITED(valor)) {
			if(WEXITSTATUS(valor)!=1) {				
							return 1;
						}
						else { return 0;}			
		}
		else return 0;
}

int moverFicheiro(char * buf,char * nome) {
	char * s =NULL;
	s=(char *)malloc(1024);
	pid_t filho ;
	int filhos=0;
	int valor=0;
	int i=0;

	while(buf[i]!='.') {
		s[i]=buf[i];
		i++;
	}
	s[i]='\0';
	filho=fork();
	if (filho==-1)
	{
		perror("fork no moverFicheiro");
		_exit(1);
	}
    if(filho==0) {
    	execlp("mv","mv",s,nome,NULL);
		perror("extrair ficheiro");
		_exit(1);
    }
    else {
    	filhos=filho;
    }
    waitpid(filhos,&valor,0);
    free(s);

	if(WIFEXITED(valor)) {
		if(WEXITSTATUS(valor)!=1) {					
			return 1;
		}
		else { return 0;}
	}
	else return 0;
}

int irMetaData(char * buf) {
	pid_t filho;
	pid_t filho2;
	pid_t filho3;
	int pid[2];
	int pid1[2];
	int pid2[2];
	int p;
	int p1;
	int p2;
	char * c =NULL;
	c=(char *) malloc(1024);
	strcpy(c,"metadata/");

	p=pipe(pid);
	if(p==-1) {perror("1 pipe do irMetaData");_exit(1);}

	filho=fork();
	if(filho==-1) {perror("fork no irMetaData");_exit(1);}
		if(filho==0) {
			 			p1=pipe(pid1);
			 			if(p1==-1) {perror("2 pipe do irMetaData");_exit(1);}
			 			filho2=fork();
			 			if(filho2==-1) {perror("2 fork no irMetaData");_exit(1);}
			 			if(filho2==0) {
			 							p2=pipe(pid2);
			 							if(p2==-1) {perror("3 pipe do irMetaData"); _exit(1);}
			 							filho3=fork();
					 					if(filho3==0) {										
					 									dup2(pid2[1],1);
					 									close(pid2[1]);
					 									close(pid2[0]);
					 									execlp("ls","ls","-l",c,NULL);
					 									perror("ls no irMetaData");
					 									_exit(1);
					 					}
					 					else {									
								 				dup2(pid2[0],0);
								 				dup2(pid1[1],1);
								 				close(pid2[0]);
								 				close(pid1[1]);
								 				close(pid2[1]);
												close(pid1[0]);
								 				execlp("grep","grep",buf,NULL);															
								 				perror("grep no irMetaData");
								 				_exit(1);
					 					}
						}
						else {						
								dup2(pid1[0],0);
								dup2(pid[1],1);
								close(pid1[0]);
								close(pid[1]);
								close(pid1[1]);
								close(pid[0]);
								execlp("wc","wc","-l",NULL);															
								perror("wc no irMetaData");
								_exit(1);
						}
		}
		else {	
			 	dup2(pid[0],0);
			 	close(pid[0]);
			 	close(pid[1]);
			 	clearbuffer(c,1024);
			 	int n;
			 	read(0,c,1024);
			 	n=atoi(c);
			 	free(c);
			 	return n;
		}
}

int removerData(char * buf) {

	pid_t filho;
	int filhos;
	int valor;
	char * c =NULL;
	c=(char *) malloc(1024);
	strcpy(c,"data/");
	strcat(c,buf);

	filho=fork();
	if(filho==-1) {perror("fork no removerData"); _exit(1);}
	if(filho==0) {
					execlp("rm","rm",c,NULL);
					perror("remover");
					_exit(1);
	}
	else {
		filhos=filho;
	}
	waitpid(filhos,&valor,0);
	free(c);	
	if(WIFEXITED(valor)) {
		if(WEXITSTATUS(valor)!=1) {					
			return 1;
		}
		else { return 0;}
	}
	else return 0;

		return 0;
}

ListaLigada lerNoMetadata(ListaLigada l) {
	pid_t filho;
	int p1;
	int pid[2];
	const char limitador[2]="\n";
	char *ler;
	ler=(char*)malloc(1024);
	char *token;

	p1=pipe(pid);
	if(p1==-1) {perror("pipe do lerNoMetadata");_exit(1);}
	filho=fork();
	if(filho==-1) {perror("fork do lerNoMetadata");_exit(1);}
	if(filho==0) {
					close(pid[0]);
					dup2(pid[1],1);
					close(pid[1]);
					execlp("ls","ls","-1","data",NULL);
					perror("ls -1");
					_exit(1);
	}
	else {
			close(pid[1]);
			dup2(pid[0],0);
			close(pid[0]);
			read(0,ler,1024);
			token=strtok(ler,limitador);
			while(token!=NULL){
				l=insereElemento(l,token);
				token=strtok(NULL,limitador);
			}
		return l;
	}
}
int main(int argc, char const *argv[])
{
	ListaLigada l =NULL;
	const char* toServidor="./toServidor";
	int n;
	int pidUtilizador;

	pid_t filho,filho1,filho2,filho3;

	int filhos,filhos1,filhos2,filhos3;
	int values,values1,values2,values3;

	int lido;
	char *buf=NULL;
	char *bufleitura=NULL;
	char * receve =NULL;
	char *k=NULL;
	char * s=NULL;
	
	buf = (char*)malloc(1024);
	bufleitura = (char*)malloc(1024);
	 receve = (char *)malloc(1024);
	k=(char *) malloc(1024);
	 s =(char *) malloc(1024);
	
	int fdLeitura;
	
	n=mkfifo(toServidor, 0666);
	if(n==-1) {perror("fifo"); _exit(1);}

	if(crearPastaMetadata()) {perror("erro a criar a pasta metadata"); _exit(1);}
	
	if(crearPastaData()) {perror("erro a criar a pasta data"); _exit(1);}
	 
	write(0,"SERVER ACTIVO\n",strlen("SERVER ACTIVO\n"));   
	while(1) {
    	
    	write(0,"À espera de conexão...\n",strlen("À espera de conexão...\n")+1);
    	fdLeitura = open(toServidor,O_RDONLY);

		if((lido=read(fdLeitura,buf,1024))>0) {				
    			if(strcmp(buf,"backup")==0) {
    				clearbuffer(buf,1024);
    				if((lido=read(fdLeitura,buf,1024))>0) {
   							pidUtilizador=atoi(buf);
					}
					if((lido=read(fdLeitura,buf,1024))>0){
						do {
           			 		filho=fork();

           			 		if(filho==-1) {perror("fork 1"); _exit(1);}

           			 		if(filho==0){
	           			 		if(lerSha(buf)) {
				           			bufleitura=strdup(lerSha(buf));
				           			strcpy(buf,bufleitura);
				           			strcpy(receve,bufleitura);
				           			strcpy(k,bufleitura);
				           		
				           			if (gzip(bufleitura)) 
				           			{
										if(metadata(buf)) {
											if(mudarNome(k)){
			         							clearbuffer(buf,1024);
												kill(pidUtilizador,SIGUSR1);
											}
											else {kill(pidUtilizador,SIGUSR2);clearbuffer(buf,1024);}
										}
									else {kill(pidUtilizador,SIGUSR2);clearbuffer(buf,1024);}

				         			}
	         						else {
	         							kill(pidUtilizador,SIGUSR2);
	         							clearbuffer(buf,1024);
	         						}
								}
								else {
									kill(pidUtilizador,SIGUSR2);
									clearbuffer(buf,1024);
								}
								return 0;
  							}      
  							else {
  								filhos=filho;
  							}

          					waitpid(filhos,&values,0);
          					clearbuffer(buf,1024);
           				}while((lido=read(fdLeitura,buf,1024))>0); 

					}
				}
				else {
					if(strcmp(buf,"restore")==0) {
						if((lido=read(fdLeitura,buf,1024))>0){
                   			pidUtilizador=atoi(buf);
						}
						clearbuffer(buf,1024);
	                    if((lido=read(fdLeitura,buf,1024))>0){       
	                     	filho1=fork(); 
	                     	if(filho1==-1) {perror("segundo fork da main"); _exit(1);}			
	                     	if(filho1==0) {
		                     		s=nomeFicheiroF(buf);
		                     		if(s!=NULL) {
		                     			my_strcpy(k,s);
										if(apagarMetadata(buf))
										{	
											if(copiarFicheiro(k))
			  								{
						                     	if(extrairFicheiro(k))
						                     	{
						                     		if(moverFicheiro(k,buf)) 
						                     		{
						                     			clearbuffer(buf,1024);
						                     			kill(pidUtilizador,SIGUSR1);
						                     		} 
						                     		else {kill(pidUtilizador,SIGUSR2);}
						                     		}
						                     	else {kill(pidUtilizador,SIGUSR2);}
						                     	}
						                    else {kill(pidUtilizador,SIGUSR2);}					                 
						                    }
	 									else {kill(pidUtilizador,SIGUSR2);}				
		                     		}
		                     		else {
		                     			kill(pidUtilizador,SIGUSR2);
		                     			clearbuffer(buf,1024);
	                     			}
	                     		return 0;
	                     	}
	                    else {
	                     		filhos1=filho1;
	                     		clearbuffer(buf,1024);
	                     }
	                     							waitpid(filhos1,&values1,0);
	                     							clearbuffer(buf,1024);							        
                     	}
					}
					else {
						if(strcmp(buf,"delete")==0) {
							clearbuffer(buf,1024);
				            if((lido=read(fdLeitura,buf,1024))>0){
				                pidUtilizador=atoi(buf);     					
							}
							clearbuffer(buf,1024);
							if((lido=read(fdLeitura,buf,1024))>0) {
                     			filho2=fork();
                     			if(filho2==-1) {perror("filho2 "); _exit(1);}
                     			if(filho2==0) {
                     				n=irMetaData(buf);
                     				if(n==0) {
                     					if(removerData(buf)){
                     						kill(pidUtilizador,SIGUSR1);
                     					}
                     					else {kill(pidUtilizador,SIGUSR2);}
                     				}
                     				else {kill(pidUtilizador,SIGUSR2);}
                     						return 0;
                     			}
                     			else {					
                     				filhos2=filho2;
                     				clearbuffer(buf,1024);
                     			}				
                     							waitpid(filhos2,&values2,0);
                     							clearbuffer(buf,1024);

																}
						}
						else {
							if(strcmp(buf,"gc")==0) {
								clearbuffer(buf,1024);
								if((lido=read(fdLeitura,buf,1024))>0){
							        pidUtilizador=atoi(buf);            					
								}
								filho3=fork();
								if(filho3==-1) {perror("fork do gc");_exit(1);}	 
								if(filho3==0) {
									l=initLista();
									l=lerNoMetadata(l);

									int contador=0;
									if(l->nomeDiguest==NULL) {kill(pidUtilizador,SIGUSR2); liberta(l);return 0;}

									while(l) {
										n=irMetaData(l->nomeDiguest);
										if(n==0) {
											contador++;																
											removerData(l->nomeDiguest);
										}
										l=l->prox;
									}
									if(contador){
										kill(pidUtilizador,SIGUSR1);
									}
									else {kill(pidUtilizador,SIGUSR2);}
									liberta(l);
									return 0;		
								}
								else {
									filhos3=filho3;
									clearbuffer(buf,1024);
								}
												waitpid(filhos3,&values3,0);
																	clearbuffer(buf,1024);
							}
						}
					}
				}
				}		
}
   write(1,"Server will shutdown\n",strlen("Server will shutdown\n")+1);
  close(n);
  free(buf);
  free(bufleitura);
  close(fdLeitura);
  free(receve);
  free(s);
  free(k);
	return 0;
}


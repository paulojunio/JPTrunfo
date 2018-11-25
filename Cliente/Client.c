// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define PORT 8888 
int flag;
/**
  * Metodo de criar um arquivo.
  */
void criarArquivo(char *nomeArquivo,int sock) {
   const char * nomeArq = nomeArquivo;
   mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
   int flag = creat(nomeArq,mode);
   if(flag != -1) {
      printf("Foi criado o arquivo: %s\n", nomeArq);   	
   }
   else{
      printf("Não foi possivel criar o arquivo: %s, pois ", nomeArq);
      
      switch(errno) {
         case EACCES:
            printf("não há permisão disponivel!\n");
            break;
         case ENOSPC:
            printf("não há espaço em disco!\n");
            break;
         case EMFILE:
            printf("já tem muito arquivos criados para este processo!\n");
            break;
         case EROFS:
            printf("o arquivo/diretório só tem permissao de ler!\n");
            break;
         default:
            printf("o arquivo tem algum problema!\n");
            break;
      }
   }
}

/**
  * Metodo de deletar um arquivo.
  */
void deletarArquivo(char *nomeArquivo,int sock) {
	
   const char * nomeArq = nomeArquivo;
   int flag = unlink (nomeArq);
   
   if(flag != -1){
      printf("Foi delatado o arquivo/link: %s\n", nomeArq);
   }
   else{
      printf("Não foi possivel deletar o arquivo/link: %s, pois ", nomeArq);
      
      switch(errno) {
         case EACCES:
            printf("não há permisão disponivel!\n");
            break;
         case EROFS:
            printf("o arquivo/diretório só tem permissao de ler!\n");
            break;
         case ENOENT:
            printf("o arquivo/link não existe neste local!\n");
            break;
         default:
            printf("o arquivo/link tem algum problema!\n");
            break;
      }
   }
}

void criarDiretorio(char *nomeDiretorio,int sock) {
   
   const char * nomeDir = nomeDiretorio;
   int flag = mkdir (nomeDir,0755);
   
   if(flag == 0){
      printf("Foi criada a pasta: %s\n", nomeDir);
   }
   else{
      printf("Não foi possivel criar a pasta: %s, pois ", nomeDir);
      
      switch(errno) {
         case EACCES:
            printf("o diretório não pode ser gravado aqui, pois não há permisão disponivel!\n");
            break;
         case EEXIST:
            printf("o diretório já existe neste local!\n");
            break;
         case ENOSPC:
            printf("o diretório não pode ser gravado aqui, pois não há espaço em disco!\n");
            break;
         default:
            printf("o diretório tem algum problema!\n");
            break;
      }
   }
}

/**
  * Metodo de deletar um diretorio.
  */
void deletarDiretorio(char *nomeDiretorio,int sock) {
   
   const char * nomeDir = nomeDiretorio;
   int flag = rmdir (nomeDir);
   
   if(flag == 0) {
      printf("Foi deletada a pasta: %s\n", nomeDir);
   }
   else{
      printf("Não foi possivel deletar a pasta: %s, pois ", nomeDir);
      
      switch(errno) {
         case EACCES:
            printf("não há permisão disponivel!\n");
            break;
         case ENOENT:
            printf("o diretório não existe neste local!\n");
            break;
         case ENOTEMPTY:
            printf("o diretório está com arquivos dentro dele!\n");
            break;
         default:
            printf("o diretório tem algum problema!\n");
            break;
      }
   }
}	

/**
  * Metodo de listar o diretorio.
  */
void listarDiretorio(int sock) {
   
   
  char *sendMessage = "\nArquivos encontrados:\n";

   DIR *dp;
   struct dirent *ep;

   dp = opendir ("./");
   if (dp != NULL)
   {
      while (ep = readdir (dp))
         sprintf(sendMessage,"%s\n",ep->d_name);
         //puts (ep->d_name);
      (void) closedir (dp);
      send(sock,sendMessage,strlen(sendMessage),0);
   }
   else {
      char *messageSend = "Não é possivel listar este diretorio.\n";
      send(sock,messageSend,strlen(messageSend),0);
   }
  
}

void *receiveMessages(void *sock_fd) {
    int sock = *((int *)sock_fd);
    char mensagemServidor[1024];
    //printf("%d",sock);
    while (1) {
      memset(mensagemServidor,'\0',sizeof(mensagemServidor));
      if(recv( sock , mensagemServidor, 1024,0) < 0) {
        //printf("\nErro ao receber uma mensagem.\n");
      }else{
        if(strcmp(mensagemServidor,"Seja bem vindo ao servido Mini_interpretador!")) {
          printf("Servido: %s\n",mensagemServidor);
        }else{
          printf("AAAAA %s", mensagemServidor);
          flag = 1;
          char *separado;
          separado = strtok (mensagemServidor," ,.-");
          int tipo;
          char *nomeArq;
          for(int i = 0; i < 3; i++) {
            if(i == 1) {
              tipo = (int) (*separado - '0');
            }else if(i == 2) {
              nomeArq = separado;
            }
            separado = strtok (NULL," ,.-");
          }
          if(tipo == 1) {
            listarDiretorio(sock);
          }else if(tipo == 2) {
            criarArquivo(nomeArq,sock);
          }else if(tipo == 3) {
            deletarArquivo(nomeArq,sock);
          }else if(tipo == 4) {
            criarDiretorio(nomeArq,sock);
          }else if(tipo == 5) {
            deletarDiretorio(nomeArq,sock);
          }else{
            char* messageSend = "Tipo não aceito!";
            send(sock, messageSend,strlen(messageSend),0);
          }
        }
      }
    }
}

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char mensagem[1024];
	flag = 0;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Erro ao criar Socket! \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	//Convertendo ipv4 e ipv6 pra binario (?)
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nEndereco invalido ou nao suportado!\n"); 
		return -1; 
	} 
	//tenta conectar ao servidor, nesse caso ele nao ta armazenando a conexao e ele realiza a operacao e dps disconecta..
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\n Falha na Conexão !\n"); 
		return -1; 
	} 
	
	printf("Conectando ao servidor...\n");
  pthread_t recv_thread;
  //printf("%d",sock);
  if( pthread_create(&recv_thread, NULL, receiveMessages, (void*) &sock) < 0)
  {   
      perror("Não foi possivel criar a thread de menssagens");
      return 1;
  }  
	while(1) {
		if(flag == 1) {
      memset(mensagem,'\0',sizeof(mensagem));		
      //printf("Digite alguma coisa para enviar pro servidor: ");
      scanf("%s",&mensagem[0]);
      send(sock , mensagem , strlen(mensagem) , 0 ); 
      
      if(strcmp(mensagem,"Sair") == 0) {
        close(sock);
        printf("\nDisconectando...\n");
        exit(1);
      }
    }
	}
  pthread_join(recv_thread , NULL);
}



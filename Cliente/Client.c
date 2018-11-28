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
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

	
#define TRUE 1 
#define FALSE 0 
#define PORT 8888 
int flag;
/**
  * Metodo de criar um arquivo.
  */
void criarArquivo(char *nomeArquivo,int sock) { //MF
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
void deletarArquivo(char *nomeArquivo,int sock) { //DF
	
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

void criarDiretorio(char *nomeDiretorio,int sock) { //MD
   
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
void deletarDiretorio(char *nomeDiretorio,int sock) { //DD
   
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
void listarDiretorio(int sock) { //LD
  char sendMessage [1024];
  strcpy(sendMessage,"\nArquivos encontrados: \n");
  //send(sock,sendMessage,strlen(sendMessage),0);
  DIR *dp;
    struct dirent **list;

    int count = scandir("./", &list, NULL, alphasort );
     if( count < 0 ){
         perror("Couldn't open the directory");
         exit(1);
     }
    //printf("%u items in directory\n", count);
    for( int i=0; i<count;i++){
            //sprintf(sendMessage  , "%s",list[i]->d_name); Concatena a string.
     }
     send(sock,sendMessage,strlen(sendMessage),0);
     
}
/*
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
}*/

int isPartOf(char *a, char *b){
   if(strstr(a,b) != NULL){    
      return 1;
   } 
   return 0;
}

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	struct sockaddr_in serv_addr; 
	int sock = 0, valread, myIndex = -1;
	char mensagem[1024];
   char message[1024], server_reply[1024];
	int podeDigitar = 0;
   char * id = ":";
	
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

   char *hello = "0.1.joao.3";
   char *hello2 = "0.1.paulo.3";
   while(TRUE){
      if((valread = read(sock, server_reply, 1024)) > 0){
         char * buffer = (char *)malloc(strlen(server_reply)+1);
         strcpy(buffer, server_reply);
         if(isPartOf(buffer, id) == 0){
            printf("\n %s\n", server_reply);
         }else{
            char * token = strtok(buffer, ":");
            for(int i = 0; i < 2; i++){
               if (i == 1)
                  myIndex = (int)(*token-'0');
                  token = strtok(NULL, ":");
            }
            memset(server_reply,0, sizeof(server_reply));    
            break;
         }
         memset(server_reply,0, sizeof(server_reply));
      }
   }

   int command = 0;
    struct timeval tmo;
    fd_set readfds;

    printf("Enter a non-zero number: ");
    fflush(stdout);

    /* wait only 5 seconds for user input */
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    tmo.tv_sec = 0.1;
    tmo.tv_usec = 0;

	while(TRUE) {
      //DESCOMENTAR PARA TESTAR COMANDOS (tem que executar 2 terminais e digitar em cada um deles para que a msg seja enviada...)
   //        fflush(stdout);

   //  /* wait only 5 seconds for user input */
   //    FD_ZERO(&readfds);
   //    FD_SET(0, &readfds);
   //    tmo.tv_sec = 0.1;
   //    tmo.tv_usec = 0;
   //    switch (select(1, &readfds, NULL, NULL, &tmo)) {
   //    case -1:
   //       err(1, "select");
   //       break;
   //    case 0:
   //       continue;
   //    }
   //    if(scanf("%d", &command) == 1){
   //       send(sock, hello, strlen(hello), 0);
   //    }

   //    if((valread = read(sock, server_reply, 1024)) > 0){
   //           send(sock, hello2, strlen(hello2), 0);
   //           printf("%s\n", server_reply); 
   //           server_reply[valread] = '\0';
   //           memset(server_reply,0, sizeof(server_reply));  
   //    }
        

      //DESCOMENTAR PARA TESTAR LOOP 
      // send(sock, hello, strlen(hello), 0);
      // while((valread = read(sock, server_reply, 1024)) > 0 && TRUE){
      //       send(sock, hello2, strlen(hello2), 0);
      //       printf("%s\n", server_reply); 
      //       server_reply[valread] = '\0';
      //       memset(server_reply,0, sizeof(server_reply));     
      // }
      

      
   }
   return 0;
}



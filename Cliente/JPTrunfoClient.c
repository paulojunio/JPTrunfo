// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8888 

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char mensagem[1024];
	char mensagemServidor[1024];
	int flag = 0;
	
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
	
	printf("Conectando ao servidor JPTRUNFO...\n");

	while(1) {
		memset(mensagem,'\0',sizeof(mensagem));
		memset(mensagemServidor,'\0',sizeof(mensagemServidor));
		if(recv( sock , mensagemServidor, 1024,0) < 0) {
			printf("\nErro ao receber uma mensagem.\n");
		}else{
			printf("Servidor: %s\n", mensagemServidor);
			if(strcmp(mensagemServidor,"Você acabou de aceitar a partida, agora espere os outros jogadores!") == 0){
				flag = 1;
			} 

		}
		if(flag != 1) {
			printf("Digite alguma coisa para enviar pro servidor: ");
			scanf("%s",&mensagem[0]);
			send(sock , mensagem , strlen(mensagem) , 0 ); 

			if(strcmp(mensagem,"sair") == 0) {
				close(sock);
				printf("\nDisconectado, obrigado por jogar!!!\n");
				exit(1);
			}
		}
	}
} 


//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <limits.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <time.h>


using namespace std;

#define TRUE 1 
#define FALSE 0 
#define PORT 8888 
	
/*struct Carta
{
    string nomeDaCarta; 
    int valor1;
    int valor2; 
    int valor3; 
    int valor4;  
    int valor5;

};

struct Trunfo 
{
    string nomeDoTrunfo;
    string nomeValor1;
    string nomeValor2;
    string nomeValor3;
    string nomeValor4;
    string nomeValor5;
    
};

struct Cliente
{   
    vector <Carta> cartas;
};


Trunfo trunfo;
vector <Cliente> clientes;

void criarTrunfo(int numeroClientes) {
	ifstream inFile;
    inFile.open("CSGOWeapons.txt");
    if (!inFile) {
        cerr << "Trunfo não encontrado";
        exit(1);   // call system to stop
    }

    inFile >> trunfo.nomeDoTrunfo;
    inFile >> trunfo.nomeValor1;
    inFile >> trunfo.nomeValor2;
    inFile >> trunfo.nomeValor3;
    inFile >> trunfo.nomeValor4;
    inFile >> trunfo.nomeValor5;

	vector <Carta> todasAsCartas;
    for(int i = 0; i < 32; i++) {
        Carta carta;
        inFile >> carta.nomeDaCarta; 
        inFile >> carta.valor1; 
        inFile >> carta.valor2; 
        inFile >> carta.valor3; 
        inFile >> carta.valor4; 
        inFile >> carta.valor5; 
        todasAsCartas.push_back(carta);
    }
	int cont 32;
	int numeroDeCartas = 32/numeroClientes;
    int random;
    for(int i = 0; i < numeroClientes;i++){
		Cliente cliente;
        for(int j = 0; j < numeroDeCartas; j++){
			random = rand() % cont;
            Carta carta = todasAsCartas[random];
            cliente.cartas.push_back(carta);
			cont--;
        }
		clientes.push_back(cliente);
    }

}
int ganhadorTurno(int i) {
	int ganhador = -1;
	int maiorValor = -1;
	for(int i = 0; i < clientes.size();i++){
        if(valor == 1) {
            if(maiorValor < clientes[i].cartas[0].valor1) {
                maiorValor = clientes[i].cartas[0].valor1;
				ganhador = i;
            }
        }else if(valor == 2) {
            if(maiorValor < clientes[i].cartas[0].valor2) {
                maiorValor = clientes[i].cartas[0].valor2;
				ganhador = i;
            }
        }else if(valor == 3) {
            if(maiorValor < clientes[i].cartas[0].valor3) {
                maiorValor = clientes[i].cartas[0].valor3;
				ganhador = i;
            }
        }else if(valor == 4) {
            if(maiorValor < clientes[i].cartas[0].valor4) {
                maiorValor = clientes[i].cartas[0].valor4;
				ganhador = i;
            }
        }else{
            if(maiorValor < clientes[i].cartas[0].valor5) {
                maiorValor = clientes[i].cartas[0].valor5;
				ganhador = i;
            }
        }
    }
	for(int i = 0; i < clientes.size(); i++) {

	}
	return ganhador;
}*/
int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[8] , 
		max_clients = 8 , activity, i , valread , sd; 
	int max_sd;
	int numberPlayers = 0;
	int numberReadys = 0;
	srand(time(NULL));
	int playersConfirm [8];
	for(int i = 0; i < 8; i++) {
		playersConfirm[i] = 0;
	}
	struct sockaddr_in address; 
		
	char buffer[1025]; //data buffer of 1K 
	//set of socket descriptors 
	fd_set readfds; 
		
	//a message 
	char *message = "Seja bem vindo ao servido Mini_interpretador!\0"; 
	
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("Falha ao inicializar socket mestre!"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("Falha ao ativar multiplas conexões no socket mestre!"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
		
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("Falha ao ligar o socket mestre"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
	while(TRUE) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		/**
		 * Aqui a gnt vê que tem uma nova conexão..
		 */
		if (FD_ISSET(master_socket, &readfds)) 
		{  // esse if tenta aceitar essa conexao e armazena ela em new_socket
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
		
		    //TODO: Além de mandar a msg de boas vindas(printExplicarGame), podemos mandar as cartas dele.
			// depois que aceitou, ele tenta mandar uma msg de boas vindas (message foi declarado lá no inicio)
			if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
			{ 
				perror("send"); 
			} 
				
			puts("Welcome message sent successfully"); 
				
			//esse array client_socket é o array de todos os clients que estão conectados
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					numberPlayers++;
					printf("Adding to list of sockets as %d\n" , i); 
					break; 
				} 
			} 
		} 
			
		// agora ele trata eventos de entrada e saida, tentando pegar todos os prontos dos jogadores.
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &readfds)) 
			{ 
				// Nesse caso, ele ta verificando se um client se desconectou
				if ((valread = read( sd , buffer, 1024)) == 0) 
				{ 
					//ele pega as informações do socket do client e printa na tela quando ele disconecta
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , 
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					//zera a posição que o client pertencia pra ser reutilizada
					close( sd ); 
					client_socket[i] = 0; 
					numberPlayers--;
					if(playersConfirm[i] == 1) {
						numberReadys--;
						playersConfirm[i] = 0;
					}
				} 
				//TODO: Lidar com as seleções do usuario
				//Nesse caso é quando o valread != 0, ou seja, teve alguma entrada, então ele tá só printando de volta
				else
				{ 
					//set the string terminating NULL byte on the end
					//of the data read 
					if(strcmp(buffer,"Pronto") == 0 && playersConfirm[i] == 0) {
						playersConfirm[i] = 1;
						numberReadys++;
						//puts("Entro aqui1");
						char *messageP = "Você acabou de aceitar a partida, agora espere os outros jogadores!\0";
						send(sd,messageP,strlen(messageP),0);
					}else{
						if(playersConfirm[i] == 0) {
							//puts("Entro aqui2");
							char *messageP = "Você precisa digitar \"Pronto\" para começar a partida!\0";
							send(sd,messageP,strlen(messageP),0);
						}else{
							//puts("Entro aqui3");
							char *messageP = "Pare de digitar, os jogadores precisam aceitar para começar o jogo!\0";
							send(sd,messageP,strlen(messageP),0);
						}
					}
				}  
			} 
		} 
	} 
		
	return 0; 
} 




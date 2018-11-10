#include <iostream>
#include <cstdio>
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <map>
using namespace std;

struct Carta
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
    vector <Carta> todasAsCartas;

};

struct Cliente
{   
    string id;
    vector <Carta> cartas;
};


Trunfo trunfo;
vector <Cliente> clientes;

void printCarta(Carta carta) {
    printf("\n==============================\n");
    printf("%s\n", carta.nomeDaCarta);
    printf("1 - %s : %d\n", trunfo.nomeValor1, carta.valor1);
    printf("2 - %s : %d\n", trunfo.nomeValor2, carta.valor2);
    printf("3 - %s : %d\n", trunfo.nomeValor3, carta.valor3);
    printf("4 - %s : %d\n", trunfo.nomeValor4, carta.valor4);
    printf("5 - %s : %d\n", trunfo.nomeValor5, carta.valor5);
    printf("\n==============================\n");

}

void printExplicarGame() {
    printf("\n=============== JPTRUNFO ===============\n");
    printf("Seja Bem-vindo ao JPTrunfo, o trunfo que irar jogar agora é : %s.\nO jogo pode ter de 2 a 8 jogadores",trunfo.nomeDoTrunfo);
    printf("O valores que você podera escolher é: \n1- %s.\n2- %s.\n3- %s.\n4- %s.\n5- %s.\n", trunfo.nomeValor1,trunfo.nomeValor2,trunfo.nomeValor3,trunfo.nomeValor4,trunfo.nomeValor5);
    printf("Digite \"Pronto\" para começar o game, espere pelo menos 2 jogares para começar");
}
void colocarCartas() {
    int numeroDeCartas = 32/clientes.size();
    int random = rand() % 32;
    for(int i = 0; i < clientes.size();i++){
        for(int j = 0; j < numeroDeCartas; j++){
            Carta carta = trunfo.todasAsCartas[random];
            clientes[i].cartas.push_back(carta);
        }
    }
}
void printfComecarGame() {
    printf("\nCartas sendo embaralhadas...");
    srand(time(NULL));
    int primeiroJogador = rand()% clientes.size();
    printf("\nPrimeiro jogador a começar é: %d ", primeiroJogador);

    colocarCartas();
    while(1) {
        for(int i = 0; i < clientes.size(); i++){
            //mostrar a primeira crata ao jogadores, usando printCarta.
        }
        //Primeiro jogador mandar o valor escolhido.
        //Calcula o maior valor com a funcao calcularGanhadores
        //Retorna a lista de ganhadores, se for maior que 1 ganhador, faz outra rodada com os ganhadores da ultimo.
        // se nao so pega as cartas, exclui dos perdedores e bota no ganhador.
        //faca isso ate ter uma pessoa com 32 cartas na mao.
        //se o jogador nao tem nenhuma carta ele sai do jogo.
    }
}

vector<int> calcularGanhadores(int valor){
    vector <int> ganhadores;
    int maiorValor = 0;
    for(int i = 0; i < clientes.size();i++){
        if(valor == 1) {
            if(maiorValor < clientes[i].cartas[0].valor1) {
                maiorValor = clientes[i].cartas[0].valor1;
            }
        }else if(valor == 2) {
            if(maiorValor < clientes[i].cartas[0].valor2) {
                maiorValor = clientes[i].cartas[0].valor2;
            }
        }else if(valor == 3) {
            if(maiorValor < clientes[i].cartas[0].valor3) {
                maiorValor = clientes[i].cartas[0].valor3;
            }
        }else if(valor == 4) {
            if(maiorValor < clientes[i].cartas[0].valor4) {
                maiorValor = clientes[i].cartas[0].valor4;
            }
        }else{
            if(maiorValor < clientes[i].cartas[0].valor5) {
                maiorValor = clientes[i].cartas[0].valor5;
            }
        }
    }

    for(int i = 0; i < clientes.size();i++) {
        if(valor == 1) {
            if(maiorValor == clientes[i].cartas[0].valor1) {
                ganhadores.push_back(i);
            }
        }else if(valor == 2) {
            if(maiorValor == clientes[i].cartas[0].valor2) {
                ganhadores.push_back(i);
            }
        }else if(valor == 3) {
            if(maiorValor == clientes[i].cartas[0].valor3) {
                ganhadores.push_back(i);
            }
        }else if(valor == 4) {
            if(maiorValor == clientes[i].cartas[0].valor4) {
                ganhadores.push_back(i);
            }
        }else{
            if(maiorValor == clientes[i].cartas[0].valor5) {
                ganhadores.push_back(i);
            }
        }
    }

    return ganhadores;
}
int main() {
    
    
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
    
    return 0;
}
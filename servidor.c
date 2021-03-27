#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define MAX 1024

int main(void){
    int cliente;
    unsigned int PORTA = 4000;
    //Criando socket TCP - IP 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //Caso aconteça algum erro -1 é retornado!
    if(sockfd < 0){printf("Erro na criaçao de socket!");return 1;}
    //Criando estrutura basica IPV4
    struct sockaddr_in svaddr,client;
    char automsgsv[] = "Seja bem vindo ao chat KN1v1!\n";
    //Guarda o tamanho da estrutura sockaddr_in
    int sizeclientaddr = sizeof(client);
    //Variaveis responsaveis por receber as mensagens e envia-las
    char msgcliente[MAX], msgservidor[MAX];
    //preenchendo a memória com bytes nulos
    memset(&msgservidor,'\0',sizeof(msgservidor)); 
    memset(&msgcliente ,'\0',sizeof(msgcliente));  
    //Definindo dados da estrutura IPv4
    svaddr.sin_family       = AF_INET;
    svaddr.sin_port         = htons(PORTA);
    svaddr.sin_addr.s_addr  = htons(INADDR_ANY);
    //Associando socket a um endereço em caso de erro retorna -1
    if(bind(sockfd,(struct sockaddr *)&svaddr,sizeof(svaddr)) < 0) return 1;
    //Esperando conexao do socket em caso de erro retorna -1
    if(listen(sockfd,1024) < 0) return 1;
    printf("Server startado,Listando a porta %i\n", PORTA);
    //Aceitando a conexao do socket e retornando um novo descritor de arquivo (cliente)
    cliente = accept(sockfd, (struct sockaddr *)&client,&sizeclientaddr);
    //enviando Seja bem vindo ao chat KN1v1! para o cliente
    send(cliente, automsgsv, sizeof(automsgsv), 0);
    printf("Esperando msg do cliente!\n");
    while(1){
        
        //recebe uma mensagem de um socket em caso de erro retorna -1
        if( (recv(cliente,msgcliente,MAX,0)) < 0){
            printf("Erro ao receber mensagem!");
            return 1;
        }
        //Exibe a mensagem recebida
        printf("Cliente: %s", msgcliente);
        //Caso a mensagem seja !sair ele fecha o socket/programa
        if(strstr(msgcliente, "!sair")){
            printf("Chat encerrado!");
            close(sockfd);
            return 0;
        }
        //Pede input do servidor
        printf("Digite:");
        fgets(msgservidor, MAX, stdin);
        msgservidor[strlen(msgservidor)-1];
        //Envia a msg para o cliente
        send(cliente, msgservidor, MAX, 0);
        //preenchendo a memória com bytes nulos
        memset(&msgservidor,'\0',sizeof(msgservidor));    
        memset(&msgcliente,'\0',sizeof(msgservidor));
    }
    //Fechando descritor de arquivo
    close(sockfd);
    return 0;
}

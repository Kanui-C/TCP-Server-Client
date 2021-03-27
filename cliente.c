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

int main(int argc, char *argv[]){
    int PORTA = 4000;
    //Variaveis responsaveis por enviar e receber mensagens
    char msgclient[MAX];
    char msgserver[MAX];
    //Criando estrutura basica IPV4
    struct sockaddr_in saddr;
    //Definindo dados da estrutura IPv4
    saddr.sin_family        = AF_INET;
    saddr.sin_port          = htons(PORTA);
    saddr.sin_addr.s_addr   = inet_addr("192.168.0.103");
    //Criando socket TCP - IP 
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    //Em caso de erro retorna -1
    if(sockfd < 0){printf("Erro na criaçao de socket!");return 1;}
    //Inicializando conexao com o socket
    if( (connect(sockfd,(struct sockaddr *)&saddr, sizeof(saddr))) < 0){
        printf("Houve um erro ao se conectar ao servidor!\n");
        return 1;
    }
    //preenchendo a memoria com bytes nulos
    memset(&msgclient,'\0', sizeof(msgclient));
    //Recebendo o banner do servidor
    recv(sockfd, msgserver, MAX, 0);
    //exibindo o banner recebido
    printf("%s", msgserver);
    //preenchendo a memoria com bytes nulos
    memset(&msgserver,'\0', sizeof(msgserver));
    while (1){
        //Pedindo input do client
        printf("Digite:");
        fgets(msgclient, sizeof(msgclient), stdin);
        msgclient[strlen(msgclient)-1];
        //Caso o cliente digite !sair
        if(strstr(msgclient, "!sair")){
            //ele envia o !sair para o servidor em caso de erro retorna -1
            if( (send(sockfd, msgclient, MAX, 0)) < 0) return 0;
            printf("Chat encerrado!");
            //fecha socket
            close(sockfd);
            return 0;
        }
        //Enviando msg para o servidor
        if( (send(sockfd, msgclient, MAX, 0)) < 0){printf("Erro no envio de mensagem!");return 1;}
        //Recebendo msg do servidor
        if( (recv(sockfd, msgserver, MAX, 0)) < 0){printf("Erro ao receber mensagem!");return 1;}
        //Exibindo msg recebida
        printf("Servidor:%s", msgserver);
        //preenchendo a memoria com bytes nulos
        memset(&msgclient,'\0', sizeof(msgclient));
        memset(&msgserver,'\0', sizeof(msgserver));

    }
    close(sockfd);
    return 0;
}

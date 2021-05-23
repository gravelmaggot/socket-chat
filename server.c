#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// handler erro/saida
void error (const char *msg){

    perror(msg);
    exit(1); //sai do prog

}

int main (int argc, char * argv[]){

    //handler pra port number nulo
    if (argc < 2){

        fprintf(stderr, "Port number inválido.");
        exit(1);

    }

    //enderecos, buffer pra strings, socket
    int sockfd, newsockfd, portnumber, n;
    char buffer[255];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){

        error("Erro ao abrir socket");

    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portnumber = atoi(argv[1]);


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portnumber);


    // check bind
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){

        error("Erro no bind");

    }

    listen(sockfd, 5); //listen para clients, max 5
    clilen = sizeof(client_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clilen);
    

    if(newsockfd < 0){

        error("Erro ao aceitar conexão.");

    }

    while (1){

        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if(n <0){

            error("Erro na leitura.");

        }
    
        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));

        if(n < 0){

            error("Erro ao escrever.");

        }

        int i = strncmp("FIM", buffer, 3);
        if(i == 0){

            break;

        }

    }

    close (newsockfd);
    close(sockfd);
    return 0;

}
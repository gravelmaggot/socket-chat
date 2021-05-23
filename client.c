#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error (const char *msg){

	perror(msg);
	exit(0);

}

int main (int argc, char *argv[]){

	int sockfd, portnumber, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];


	//ate a linha 35, faz basicamente o mesmo que em server.c	
	if (argc < 3){

		fprintf(stderr, "Parâmetros insuficientes.");
		exit(1);

	}

	portnumber = atoi (argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){

		error("Erro ao abrir socket.");

	}

	server = gethostbyname(argv[1]);

	//handler para hostname nulo/inexistente
	if(server == NULL){

		fprintf(stderr, "Host inexistente.");

	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length); //bin copy de hostent para serv_addr
	serv_addr.sin_port = htons(portnumber);

	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){

		error("Erro na conexão.");

	}

	//loop escrita/leitura
	while(1){

		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));

		if(n < 0){

			error("Erro na escrita.");

		}

		bzero(buffer, 255);
		n = read(sockfd, buffer, 255);
		
		if(n < 0){

			error("Erro na leitura.");

		}

		printf("Server: %s", buffer);

		int i = strncmp("FIM", buffer, 3);
        if(i == 0){

            break;

        }
	
	}

	close(sockfd);
	return 0;

}
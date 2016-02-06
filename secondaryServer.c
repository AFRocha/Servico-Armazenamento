#include "inet.h"
#include "message.h"
#include "table.h"
#include "table_skel.h"
#include "network_client-private.h"
#include "message-private.h"
#include "list-private.h"
#include "table-private.h"
#include "network_client.h"
#include "data.h"
#include "entry.h"
#include "list.h"
#define MAX_CLIENT 10
#include <poll.h> 



/*
 *secondaryServer.c
 *Andre Rocha 42189
 */

int main(int argc, char **argv){
   int sockfd, connsockfd, cont = 2, i;
   struct sockaddr_in server, client;
   struct pollfd desc_set[MAX_CLIENT];//Definido para 10 utilizadores em max_client
   struct table_t *table=(struct table_t*)malloc(sizeof(struct table_t));
   char *terminal = malloc(sizeof(char));
   int nbytes, msg_size, count;
   socklen_t size_client;
   // Verifica se foi passado algum argumento
   if (argc != 3){
      printf("Uso: ./secondaryServer <porto_servidor> <size_table>\n");
      printf("Exemplo de uso: ./SecondaryServer 12345 6\n");
      return -1;
   }
   // Cria socket TCP
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
      perror("Erro ao criar socket");
      return -1;
   }	   

   // Preenche estrutura server para bind
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   server.sin_port = htons(atoi(argv[1]));
   // Faz bind
   if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0){
      perror("Erro ao fazer bind");
      close(sockfd);
      return -1;
   };
   // Faz listen
   if(listen(sockfd, 20) < 0){
      perror("Erro ao executar listen");
      close(sockfd);
      return -1;
   };
   //cria table
   table=table_skel_init(atoi(argv[2])); 

   desc_set[0].fd=fileno(stdin);
   desc_set[0].events=POLLIN;
   desc_set[0].revents=0;
   desc_set[1].fd=sockfd; //Listen socket
   desc_set[1].events=POLLIN;
   desc_set[1].revents=0;
   for(i=2; i< MAX_CLIENT; i++){
      desc_set[i].fd=-1;
      desc_set[i].events=0;
      desc_set[i].revents=0;
   }
   printf("**Estrutura de dados criada**\n**Servidor 'a espera de dados**\n");	
   //Espera por dados nos sockets abertos
   while(poll(desc_set, MAX_CLIENT, -1) >= 0){ 
      /*Tentativa de nova ligacao*/
      if(desc_set[1].revents & POLLIN){
         connsockfd = accept( desc_set[1].fd, ( struct sockaddr * )&client, &size_client);
         //adiciona connsockfd a desc_set
         desc_set[cont].fd=connsockfd;
         desc_set[cont].events=POLLIN;
         cont++;
      }
      /*Testar se ha algum evento extra por exemplo o pedido de informaçao
      do conteudo actual do server*/
      if(desc_set[0].revents & POLLIN){
         fgets(terminal, MAX_MSG, stdin);
         terminal=strtok(terminal, "\n");
         if(strcmp(terminal, "print")== 0){
            int conta=0;				
            char **tabela;
            tabela = table_get_keys(table);
            printf("A tabela actual e': \n");
            while(tabela[conta]!=NULL){
               printf("Key na posição [%d]: %s", conta, tabela[conta]);
               conta++;
            }
         }else{
            printf("erro! escreva print para apresentar o conteudo actual do server\n");
         }
      }

      for(i=2; i<cont; i++) {
         if (desc_set[i].revents & POLLIN) {
            // Lê string enviada pelo cliente do socket referente a conexão
            if((nbytes = read(desc_set[i].fd,&msg_size,sizeof(int))) <= 0){
               // perror("Erro ao receber dados do cliente1");
               close(desc_set[i].fd);
               continue;
            }
            msg_size=ntohl(msg_size);
            char *msg_buf=(char *)malloc(msg_size+1);// /0
            if((nbytes = read_all(desc_set[i].fd,msg_buf,msg_size)) < 0){
               perror("Erro ao receber dados do cliente2");
               close(desc_set[i].fd);
               continue;
            }		
            struct message_t *msg_recebida = buffer_to_message(msg_buf, msg_size);
            free(msg_buf);
            int res=invoke(msg_recebida);
            //Envia resposta ao cliente depois de converter para formato de rede
            msg_size = message_to_buffer(msg_recebida,&msg_buf);
            msg_size = htonl(msg_size);					 	
            free_message(msg_recebida);
            // Envia tamanho do buffer ao cliente através do socket referente a conexão
            if((nbytes = write(desc_set[i].fd,(char*)&msg_size,sizeof(int))) != sizeof(int)){
               perror("Erro ao enviar resposta ao cliente");
               close(desc_set[i].fd);
               continue;
            }
            msg_size=ntohl(msg_size);
            if((nbytes = write_all(desc_set[i].fd,msg_buf,msg_size) != msg_size)){
               perror("Erro ao enviar resposta ao cliente");
               close(desc_set[i].fd);
               continue;
            }

            //Verifica-se se a ligaçao foi desligada
            if (desc_set[i].revents & POLLHUP){
               close(desc_set[i].fd);
               desc_set[i].fd = -1;
               desc_set[i].events = 0;

            }
         }
      }
   }
   //elimina table
   table_skel_destroy();

   // Fecha socket
   close(connsockfd);
   close(sockfd);
   return 0;
}


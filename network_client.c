#include <errno.h>
#include <string.h>
#include "inet.h"
#include "message.h"
#include "message-private.h"
#include "network_client-private.h"
#define MAX_MSG 1024

/*
 *network_client.c
 *Andre Rocha 
 */

struct server_t *network_connect(const char *address_port){
   struct server_t *server = (struct server_t*)malloc(sizeof(struct server_t));
   char *porto;
   char *address;
   address = strtok(address_port,":");
   porto = strtok(NULL,":");
   server->port=atoi(porto);
   server->enderecoIp=atoi(address);
   if ((server->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Erro ao criar socket TCP");
      return NULL;
   }
   server->addr.sin_family = AF_INET;
   server->addr.sin_port = htons(atoi(porto));
   if (inet_pton(AF_INET, address, &server->addr.sin_addr) < 1) {
      printf("Erro ao converter IP\n");
      close(server->sockfd);
      return NULL;
   }
   if (connect(server->sockfd,(struct sockaddr *)&server->addr, sizeof(server->addr)) < 0) {
      perror("Sem ligação ao servidor");
      network_close(server);
      return NULL;
   }
   return server;
}

struct message_t *network_send_receive(struct server_t *server, struct message_t *msg){
   char *msg_buf = NULL;
   char *msg_buf_result = NULL;
   int result, nbytes, size, size2;
   size = message_to_buffer(msg, &msg_buf);
   size2 = htonl(size);
   if((nbytes = write(server->sockfd,&size2, sizeof(int))) != sizeof(int)){      
      if((nbytes = write(server->sockfd,&size2, sizeof(int))) != sizeof(int)){
         perror("Erro ao enviar tamanho dos dados ao servidor");
         network_close(server);
         return NULL;
      }
   }
   if((nbytes = write_all(server->sockfd,msg_buf, size)) != size){
      perror("Erro ao enviar dados ao servidor");
      network_close(server);
      return NULL;
   }
   free(msg_buf);
   if((nbytes = read(server->sockfd,&result,sizeof(result))) != sizeof(result)){
      perror("Erro ao receber dados do servidor");
      network_close(server);
      return NULL;
   }
   int msg_size=ntohl(result);
   msg_buf_result=(char *)malloc(msg_size+1);
   if((nbytes = read_all(server->sockfd,msg_buf_result,msg_size)) != msg_size){
      perror("Erro ao receber dados do servidor");
      network_close(server);
      return NULL;
   }
   struct message_t *msg_result = buffer_to_message(msg_buf_result, msg_size);
   free(msg_buf_result);
   return msg_result; 
}

int network_close(struct server_t *server){
   close(server->sockfd);
} 


#include <errno.h>
#include "inet.h"
#include "data.h"
#include "entry.h"
#include "network_client.h"
#include "network_client-private.h"
#include "message.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "client_stub.h"


/*
 *table_client.c
 *Andre Rocha 
 */


#define MAX_MSG 1024



int main(int argc, char **argv){
   if (argc != 3){
      printf("Uso: ./table_client <ip_servidor> <porto_servidor>\n");
      printf("Exemplo de uso: ./table_client 127.0.0.1 12345 \n");
      return -1;
   }
   int i;
   char address_port[100];
   /*Formar a string no formato ip:port*/
   strcpy(address_port, argv[1]);	
   strcat(address_port, ":");
   strcat(address_port, argv[2]);
   strcat(address_port, "\0");

   struct rtable_t *servidor=(struct server_t*)malloc(sizeof(struct server_t));
   servidor=rtable_bind(address_port);

   while (i!=7){
      printf("Bem-vindo, escolha uma opção:\n\n");
      printf("1---Put <data> <key>\n2---Cput <data> <key>\n3---Get <key>\n4---Del <key>\n5---Size\n6---Getkeys\n7---Sair\n");
      scanf("%d", &i);
      if(i==1) {
         char *key = (char*)malloc(sizeof(char*)*MAX_MSG);
         char *data = (void*)malloc(sizeof(void*)*MAX_MSG);
         char *aux = (char*)malloc(sizeof(char));
         fgets(aux, MAX_MSG, stdin);
         printf("Key: ");
         fflush(stdin);
         fgets(key, MAX_MSG, stdin);
         printf("Data: ");
         fflush(stdin);
         fgets(data, MAX_MSG, stdin);
         struct data_t *novaData=data_create2(strlen(data),data);
         rtable_put(servidor, key, novaData);
         //network_send_receive(server,msg);
         if ((rtable_put==-1)){
            printf("Os dados nao foram introduzidos!\n");
         }else{
            printf("Os dados foram introduzidos com sucesso!\n");
         }
         free(aux);
      }

      if(i==2){
         char *key = (char*)malloc(sizeof(char)*MAX_MSG);
         char *data = (char*)malloc(sizeof(char)*MAX_MSG);
         char *aux = (char*)malloc(sizeof(char));
         fgets(aux, MAX_MSG, stdin);
         printf("Key: ");
         fflush(stdin);
         fgets(key, MAX_MSG, stdin);
         printf("Data: ");
         fflush(stdin);
         fgets(data, MAX_MSG, stdin);
         struct data_t *novaData = (struct data_t*)malloc(sizeof(struct data_t));

         novaData->data=data;
         novaData->datasize = strlen(data);
         rtable_conditional_put(servidor,key,novaData);
         if ((rtable_conditional_put==-1)){
            printf("Os dados nao foram introduzidos!\n");
         }else{
            printf("Os dados foram introduzidos com sucesso!\n");
         }
         free(aux);		
      }

      if(i==3){
         char *key = (char*)malloc(sizeof(char)*MAX_MSG);
         char *aux = (char*)malloc(sizeof(char));
         fgets(aux, MAX_MSG, stdin);
         printf("Key: ");
         fflush(stdin);
         fgets(key, MAX_MSG, stdin);

         rtable_get(servidor, key);
         if(rtable_get==-1){
            printf("Não existe na tabela\n");
         }else{
            printf("Valor encontrado: %s\n",rtable_get(servidor,key));
         }
         free(aux);		
      }

      if(i==4){
         char *key = (char*)malloc(sizeof(char)*MAX_MSG);
         char *aux = (char*)malloc(sizeof(char));
         fgets(aux, MAX_MSG, stdin);
         printf("Key: ");
         fflush(stdin);
         fgets(key, MAX_MSG, stdin);
         rtable_del(servidor, key);

         if ((rtable_del==-1)){
            printf("Não foi possível remover o elemento da tabela\n");
         }else{
            printf("Os dados foram removidos com sucesso!\n");
         }
         free(aux);		
      }

      if(i==5){
         rtable_size(servidor);

         if ((rtable_size(servidor)==-1)){
            printf("Erro ao encontrar numero de elems na tabela!\n");
         }else{
            printf("Existem %d elementos na tabela!\n",rtable_size(servidor) );
         }
      }

      if(i==6){
         int i = 0;
         rtable_get_keys(servidor);
         if(rtable_get_keys(servidor)==-1){
            printf("Não foi possível retornar as chaves");
         }else{ 
            while(rtable_get_keys(servidor)[i]!=NULL){
               printf("Key pos [%d]: %s", i, (char*)rtable_get_keys(servidor)[i]);
               i++;
            }
         }
      }

      if(i==7){
         rtable_unbind(servidor);

      }
      if(i<1 || i>7) printf("Opcao invalida\n");
   } return 0;
}

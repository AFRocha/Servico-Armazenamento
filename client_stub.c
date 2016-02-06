#include <stdio.h>
//#include "network_client-private.h"
#include "inet.h"
#include "client_stub.h"
#include "client_stub-private.h"
#include "list.h"
#include "list-private.h"
#include "table.h"
#include <errno.h>
#define MAX_MSG 1024

/*
 *client_stub.c
 *Andre Rocha 
 *ad007
 */

struct rtable_t *rtable_bind(const char *address_port){
   struct rtable_t *rtable = (struct rtable_t*)malloc(sizeof(struct rtable_t));
   rtable->msg = (struct message_t*)malloc(sizeof(struct message_t));
   rtable->server = (struct server_t*)malloc(sizeof(struct server_t));
   rtable->server = network_connect(address_port);
   return rtable;
}

int rtable_unbind(struct rtable_t *rtable){
   network_close(rtable->server);
}

int rtable_put(struct rtable_t *rtable, char *key, struct data_t *data){
   int result;
   struct data_t *NovaData=(struct data_t*)malloc(sizeof(struct data_t));
   NovaData=data_create2(strlen(data),data);
   struct entry_t *entry=(struct entry_t*)malloc(sizeof(struct entry_t));
   entry = entry_create(key, NovaData);
   struct message_t *msg = (struct message_t*)malloc(sizeof(struct message_t));
   msg->opcode = OC_PUT;
   msg->c_type = CT_ENTRY;
   msg->content.entry=entry;
   //network_send_receive(rtable->server, msg);
   if( network_send_receive(rtable->server, msg)->opcode==OC_PUT+1){ //testar se foi colocado efectivamente no servidor
      result=1;
   } else {
      result = -1;
   }
   return result;
}

int rtable_conditional_put(struct rtable_t *rtable, char *key, struct data_t *data){
   int result;
   struct message_t *msg = (struct message_t*)malloc(sizeof(struct message_t));
   struct entry_t *entry=(struct entry_t*)malloc(sizeof(struct entry_t));
   entry = entry_create(key, data);
   msg->opcode = OC_COND_PUT;
   msg->c_type = CT_ENTRY;
   msg->content.entry=entry;
   if(network_send_receive(rtable->server, msg)->opcode==OC_COND_PUT+1){ //testar se foi colocado efectivamente no servidor
      result=1;
   } else {
      result = -1;
   }
   return result;
}

struct data_t *rtable_get(struct rtable_t *rtable, char *key){
   int result;
   struct message_t *msg = (struct message_t*)malloc(sizeof(struct message_t));
   msg->opcode = OC_GET;
   msg->c_type = CT_KEY;
   msg->content.key = (char*)malloc(sizeof(char));
   msg->content.key=key;
   if(network_send_receive(rtable->server, msg)->opcode==OC_GET+1){
      return (network_send_receive(rtable->server, msg)->content.value->data);
   }else{
      result = -1;
   }
   return result;
}
int rtable_del(struct rtable_t *rtable, char *key){
   int result;
   struct message_t *msg = (struct message_t*)malloc(sizeof(struct message_t));
   msg->opcode = OC_DEL;
   msg->c_type = CT_KEY;
   msg->content.key = (char*)malloc(sizeof(char));
   msg->content.key=key;
   if(network_send_receive(rtable->server, msg)->opcode==OC_DEL+1){
      result = 1;
   }else{
      result = -1;
   }
   return result;
}

int rtable_size(struct rtable_t *rtable){
   int result;
   struct message_t *msg = (struct message_t*)malloc(sizeof(struct message_t));
   msg->opcode = OC_SIZE;
   msg->c_type = CT_RESULT;
   msg->content.result = 0;
   if(network_send_receive(rtable->server, msg)->opcode==OC_SIZE+1){ //testar se foi colocado efectivamente no servidor
      result = (network_send_receive(rtable->server, msg)->content.result);
   } else {
      result = -1;
   }
   return result;
}

char **rtable_get_keys(struct rtable_t *rtable){
   int result;
   char **keys;
   struct message_t *msg = (struct message_t*)malloc(sizeof(struct message_t));
   msg->opcode = OC_GET_KEYS;
   msg->c_type = CT_RESULT;
   if(network_send_receive(rtable->server, msg)->opcode==OC_GET_KEYS+1){
      return (network_send_receive(rtable->server, msg)->content.keys);
   } else {
      result = -1;
      return result;
   }
}

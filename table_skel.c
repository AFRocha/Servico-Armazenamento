#include <errno.h>
#include "table_skel.h"
#include "table.h"
#include "table-private.h"
#include "inet.h"
#define MAX_MSG 1024

/*
 *table_skel.c
 *Andre Rocha 
 */


struct table_t *table;

int table_skel_init(int n_lists){
   table = table_create(n_lists);
   if(table==NULL)
      return -1;
   return table;
}

int table_skel_destroy(){
   table_destroy(table);
}

int invoke(struct message_t *msg){
   int res;
   switch(msg->opcode){
   case OC_PUT:{  
      msg->opcode = OC_PUT+1;
      msg->c_type = CT_RESULT;
      res=table_put(table, msg->content.entry->key, msg->content.entry->value);
      msg->content.result=res;
      return res;
   }break;

   case OC_COND_PUT:{
      //cria mensagem de resposta

      msg->opcode = OC_COND_PUT+1;
      msg->c_type = CT_RESULT;
      res=table_conditional_put(table, msg->content.entry->key, msg->content.entry->value);
      msg->content.result=res;

      return res;		   

   }break;

   case OC_GET :{

      msg->opcode = OC_GET+1;
      msg->c_type = CT_VALUE;
      msg->content.value=table_get(table, msg->content.key);


      //cria mensagem de resposta
      if(msg->content.value == NULL){
         msg->content.value=data_create2(0,NULL);
      }
      return res;


   }break;

   case OC_DEL:{
      //cria mensagem de resposta

      msg->opcode = OC_DEL+1;
      msg->c_type = CT_RESULT;
      res=table_del(table, msg->content.key);
      msg->content.result=res;

      return res;

   }break;

   case OC_SIZE:{
      res=table_size(table);
      //cria mensagem de resposta
      msg->content.result=res;
      msg->opcode = OC_SIZE+1;
      msg->c_type = CT_RESULT;

      return res;

   }break;  

   case OC_GET_KEYS:{
      //cria mensagem de resposta
      msg->content.keys=table_get_keys(table);
      msg->opcode = OC_GET_KEYS+1;
      msg->c_type = CT_KEYS;

      return res;

   }break;

   default:{
      printf("mensagem recebida do cliente com OP Code invalido\n");
      return -1;
   }
   }
}

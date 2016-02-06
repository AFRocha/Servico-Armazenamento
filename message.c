#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include "inet.h"
#include "data.h"
#include "entry.h"
#include "message-private.h"
#include "list-private.h"
#include <errno.h>

/** 
 *  Modulo message
 *  Autor: Andre Rocha
 */

int message_to_buffer(struct message_t *msg, char **msg_buf){
   int offset=0;
   int opcode=htons(msg->opcode);//Converter opcode de codigo maquina para codigo network(rede)
   int c_type=htons(msg->c_type);//Converter c_type de codigo maquina para codigo network(rede)
   if(msg->c_type == CT_ENTRY){
      int tamanhoKey = strlen(msg->content.entry->key); //inteiro tamanhokey que corresponde ao tamanho da key do msg
      int tamanhoData=msg->content.entry->value->datasize; //tamanho do data que corresponde ao tamanho de data do msg
      //BUFFER ... Short Short Int TamanhoKey long long tamanhoData Data
      int buffer=sizeof(short)+sizeof(short)+sizeof(long long)+sizeof(int)+strlen(msg->content.entry->key)+sizeof(int)+ msg->content.entry->value->datasize;
      *msg_buf = (char*) malloc(buffer); //Alocar memoria para o Buffer
      memcpy(*msg_buf+offset,&opcode,sizeof(short));//Copiar (short) o 'opcode' para 'msg_buf mais offset'
      offset+=sizeof(short);//Incrementar o offset(com short)
      memcpy(*msg_buf+offset,&c_type,sizeof(short));//Copiar (short) o 'c_type' para 'msg_buf mais offset'
      offset+=sizeof(short);//INcrementar o offset ( com short)

      long long timeStamp = swap_bytes_64(msg->content.entry->timestamp);//Valor de timestamp(em long long )
      memcpy(*msg_buf+offset,&timeStamp,sizeof(long long));//COpiar (em long long) 'TimeStamp' para 'msg_buf mais offset'
      offset+=sizeof(long long);//INcrementar o offset(em long long)
      int tamanhoKeyNetwork = htonl(tamanhoKey);//Traduzir tamanhoKey do host para network 
      memcpy(*msg_buf+offset,&tamanhoKeyNetwork,sizeof(int));//COpiar (int) o 'tamanhoKey (network)' para 'msg_buf mais offset'
      offset+=sizeof(int);//Incrementar o offset (com inteiro)
      memcpy(*msg_buf+offset,msg->content.entry->key,tamanhoKey);//Copiar (em valores do tipo tamanhoKey) 'key' para 'offset'
      offset+=tamanhoKey;//Incrementar o offset ( com valores do tipo tamanhoKey).

      int tamanhoDataNetwork = htonl(tamanhoData);//TRaduzir tamanhoData do host para network
      memcpy(*msg_buf+offset,&tamanhoDataNetwork,sizeof(int));//COpiar (em inteiros) 'tamanhodatanetwork' para 'msg_buf mais offset'
      offset+=sizeof(int);//Incrementar offset (em inteiros)
      memcpy(*msg_buf+offset,msg->content.entry->value->data,tamanhoData);//COpiar (em valores de tamanhoData) 'msg data' para 'msg_buf mais offset'
      //offset+=tamanhoData;//INcrementar offset (com valores do tipo tamanhoData)
      return buffer;
   }
   if(msg->c_type == CT_KEY){
      int tamanhoKey = strlen(msg->content.key);//inteiro tamanhokey que corresponde ao tamanho da key do msg
      int buffer=sizeof(short)+sizeof(short)+sizeof(int)+tamanhoKey;//buffer short short int tamanhoKey
      *msg_buf = malloc(buffer);//Alocar memoria para o buffer
      memcpy(*msg_buf+offset,&opcode,sizeof(short));//Copiar (valores short) o 'opcode' para 'msg_buf'
      offset+=sizeof(short);//INcrementar offset(em valores short)
      memcpy(*msg_buf+offset,&c_type,sizeof(short));//COpiar (short) o 'c_type' para 'msg_buf'
      offset+=sizeof(short);//Incrementar o offset ( em valores short)
      int tamanhoKeyNetwork = htonl(tamanhoKey);//TRaduzir tamanhoKey host para network
      memcpy(*msg_buf+offset,&tamanhoKeyNetwork,sizeof(int));//COpiar (em inteiros) o 'tamanhokeynetwork' para 'msg_buf'
      offset+=sizeof(int);//Incrementar offset ( em valores inteiros)
      memcpy(*msg_buf+offset,msg->content.key,tamanhoKey);//Copiar (em valores tamanhoKey) o 'msg ->content.key' para 'msg_buf'
      offset+=tamanhoKey;//INcrementar o offset com valores tamanhoKey
      return buffer; 
   }	
   if(msg->c_type == CT_KEYS){
      int buffer=sizeof(short)+sizeof(short)+sizeof(int);//buffer short short int
      int i;
      //ciclo que nos vai percorrer as keys de msg e acrescentar ao buffer o tamnho de cada uma
      for(i=0; msg->content.keys[i] != NULL; i++){
         buffer+=sizeof(int)+strlen(msg->content.keys[i]);
      }
      *msg_buf = malloc(buffer);//Alocar memoria para o buffer
      memcpy(*msg_buf+offset,&opcode,sizeof(short));//Copiar ( em valores short) o 'opcode' para 'msg_buf mais offset'
      offset+=sizeof(short);//INcrementar o offset com valores short
      memcpy(*msg_buf+offset,&c_type,sizeof(short));//COpiar (em short) o 'c_type' para 'msg_buffer mais offset'
      offset+=sizeof(short);//Incrementar offset com valores short
      int nKeysNetwork = htonl(i);//Traduzir o numero de keys de host para network
      memcpy(*msg_buf+offset,&nKeysNetwork,sizeof(int));//COpiar ( em inteiros) o 'numer ode keys network' para 'msgbuffer mais offset' 
      offset+=sizeof(int);//Incrementar offset com valores int
      //Ciclo que percorre as Keys de msg e copia para buffer uma a uma             	
      for(i=0;msg->content.keys[i] !=NULL; i++){
         int KeysNetwork=htonl(strlen(msg->content.keys[i]));//Traduz a key de host para network
         memcpy(*msg_buf+offset,&KeysNetwork,sizeof(int));//Copiar ( em valores int ) o 'KeysNetwork' para 'msgbuf'
         offset+=sizeof(int);//INcrementar offset com valores int
         memcpy(*msg_buf+offset,msg->content.keys[i],strlen(msg->content.keys[i]));//COpiar a key para msg_buf
         offset+=strlen(msg->content.keys[i]);//INcrementar offset com o numero de keys
      }
      return buffer;
   }
   if(msg->c_type == CT_VALUE){
      int tamanhoData=msg->content.value->datasize;//inteiro tamanhoData que corresponde ao tamanho da data do msg
      int buffer=(sizeof(short)+sizeof(short)+sizeof(int)+tamanhoData);//buffer com short short int e tamanhoData
      *msg_buf = malloc(buffer);//reservar memoria para o buffer
      memcpy(*msg_buf+offset,&opcode,sizeof(short));//copiar (em valores short) o 'opcode' para 'msg_buf'
      offset+=sizeof(short);//INcrementar offset com valores short
      memcpy(*msg_buf+offset,&c_type,sizeof(short));//Copiar (em valores short) o 'c_type' para 'msg_buf'
      offset+=sizeof(short);//Incrementar o offset com valores short
      int DataSize=htonl(msg->content.value->datasize);//traduzir datasize de host para network
      memcpy(*msg_buf+offset,&DataSize,sizeof(int));//Copiar ( em inteiros) o 'datasize' para o 'msg_buf'
      offset+=sizeof(int); //INcrementar o offset ( em valores inteiros)
      memcpy(*msg_buf+offset,msg->content.value->data,tamanhoData);//COpiar (em valores de tamanhoData) o 'data de msg' para 'msg_buf'  
      offset+=tamanhoData; //Incrementar offset ( em valores de tamanhoData)
      return buffer; 
   }
   if(msg->c_type == CT_RESULT){
      int buffer=(sizeof(short)+sizeof(short)+sizeof(int));//Buffer com short short e int
      *msg_buf = malloc(buffer);//Reservar memoria para o buffer
      memcpy(*msg_buf+offset,&opcode,sizeof(short));//COpiar ( em valores short) o 'opcode' para o 'msg_buf mais offset'
      offset+=sizeof(short);//INcrementar o offset com valores short
      memcpy(*msg_buf+offset,&c_type,sizeof(short));//Copiar (em valores short) o 'c_type' para o 'msg_buf mais offset'
      offset+=sizeof(short);//Incrementar offset ( com valores sh0rt)
      int ResultNetwork=htonl(msg->content.result);//COnverter result host para network
      memcpy(*msg_buf+offset,&ResultNetwork,sizeof(int));//COpiar ( em valores int ) o 'ResultNetwork' para o'msg_buf mais offset'
      offset+=sizeof(int);//INcrementar o offset com valores int
      return buffer; 
   }
   free(msg);
   msg=NULL;
   return -1;//Deu erro  
}


struct message_t *buffer_to_message(char *msg_buf, int msg_size){
   struct message_t *NewMessage = (struct message_t*)malloc(sizeof(struct message_t));
   short opcode;
   short c_type;
   int offset=0;
   memcpy(&opcode,msg_buf+offset,sizeof(short));//Copiar em short de msg_buf para opcode
   offset+=sizeof(short);//iNcrementar offset com short
   NewMessage->opcode=ntohs(opcode);//Codificar opcode da nova mensagem de formato de rede para formate de maquina
   memcpy(&c_type,msg_buf+offset,sizeof(short));//Copiar em short de msg_buf para c_type
   NewMessage->c_type=ntohs(c_type);//Codificar opcode da nova mensagem de formato de rede para formate de maquina
   offset+=sizeof(short);//iNcrementar offset com short
   if(NewMessage->c_type==CT_ENTRY){
      int FormatoRede;//Variavel que vai guardar o formato rede para depois poder converter
      int tamanhoKey;	


      long long TimeStamp;
      memcpy(&TimeStamp,msg_buf+offset,sizeof(long long));//Copiar de msg_buf+offset para TimeStamp (em long long)
      long long TimeStamp2 = swap_bytes_64(TimeStamp);//TimeSTamp2 que eh igual a conversao de TimeStamp para long long
      offset+=sizeof(long long);//Incrementar offset com long long

      memcpy(&FormatoRede,msg_buf+offset, sizeof(int)); //Copiar em inteiros de 'msg_buf mais offset' para inteiro'FormatoRede'
      tamanhoKey=ntohl(FormatoRede);//Codificar tamanhokey formato de maquina(atraves de FormatoRede)
      offset+=sizeof(int);//Incrementar offset ( em inteiros)
      //char key[tamanhoKey+1];//reservar espaço tamanhoKey+1 em char key devido ao /0
      char *key;
      //memcpy(key,msg_buf+offset,tamanhoKey);//Copiar a string de msg_buff+offset para key
      key = strdup(msg_buf+offset);
      //key[tamanhoKey]='\0';//colocar o /0 em key
      offset+=tamanhoKey;//incrementar o offset (com tamanhoKey)

      memcpy(&FormatoRede,msg_buf+offset, sizeof(int));//copiar de msg_buf+offset para FormatoRede (em inteiros)
      int tamanhoData=ntohl(FormatoRede);//Colocar tamanho data em formato maquina
      offset+=sizeof(int);//incrementar offset (inteiros)

      void *data=malloc(tamanhoData);//reservar memoria data
      memcpy(data,msg_buf+offset,tamanhoData);//copiar de msg_buf+offset para data
      offset+=tamanhoData;//incrementar offset com valores de tamanhoData
      struct data_t *value= data_create2(tamanhoData,data);//criar bloco de data
      NewMessage->content.entry=entry_create(key,value);//content.entry da nova mensagem é uma nova entry
      NewMessage->content.entry->timestamp = TimeStamp2;//content.entry timestamp danova mensagem é TimeStamp2
      return NewMessage;
   }
   if(NewMessage->c_type==CT_KEY){
      int FormatoRede;
      memcpy(&FormatoRede,msg_buf+offset, sizeof(int));//Copiar do msg_buf++offset para Formatorede (em inteiros)
      int tamanhoKey=ntohl(FormatoRede);//Colocar tamanhoKey em formato de maquina (atraves do FormatoRede)
      offset+=sizeof(int);//incrementar o offset com inteiro
      NewMessage->content.key=malloc(tamanhoKey+1);//reservar espaço tamanhoKey+1 em char key devido ao /0
      memcpy(NewMessage->content.key, msg_buf+offset, tamanhoKey);//Copiar de msg_buf+offset para content.key de nova mensagem
      NewMessage->content.key[tamanhoKey]='\0';//INlcuir o \0 na key
      offset+=tamanhoKey;//Incrementar offset com tamanhoKey
      return NewMessage; 
   }
   if(NewMessage->c_type==CT_KEYS){
      int tamanhoKey;
      int i;
      int FormatoRede;
      memcpy(&FormatoRede, msg_buf+offset, sizeof(int));//Copiar de msg_buf+offset para FormatoRede (em int)
      int num_keys=ntohl(FormatoRede);//transformar num_keys em formato maquina
      offset+=sizeof(int);//INcrementar offset (em int)
      NewMessage->content.keys=(char**)malloc((num_keys+1)*sizeof(char*));//Alocar memoria
      //Ciclo que vai percorrer num_keys , copiar do msg_buf para formatorede e transformar tamanhoKey em codigo maquina atraves da variavel formatoRede
      for(i=0;i<num_keys;i++){
         memcpy(&FormatoRede,msg_buf+offset, sizeof(int));
         int tamanhoKey=ntohl(FormatoRede);
         offset+=sizeof(int);

         NewMessage->content.keys[i]=malloc(tamanhoKey+1);//Alocar memoria
         memcpy(NewMessage->content.keys[i], msg_buf+offset, tamanhoKey);//Copiar de msg_buf+offset para cont_keys do ciclo (i) da nova mensagem
         NewMessage->content.keys[i][tamanhoKey]='\0';
         offset+=tamanhoKey;//iNcrementar offset com tamanhoKey
      }
      NewMessage->content.keys[num_keys]=NULL;
      return NewMessage;
   }
   if(NewMessage->c_type==CT_VALUE){
      int FormatoRede;
      memcpy(&FormatoRede, msg_buf+offset, sizeof(int));//Copoiar de msg_buf+offset para variavel formato rede FromatoRede
      int datasize=ntohl(FormatoRede);//Transofrmar conteudo do FormatoRede em codigo maquina e guardar em datasize
      offset+=sizeof(int);//Incrementar offset ( com int)
      void* data = malloc(datasize);//Alocar memoria
      memcpy(data,msg_buf+offset,datasize);//Copiar de msg_buf+offset para data ( valores em datasize)
      offset+=datasize;//Incrementar o ofset com datasize
      NewMessage->content.value=data_create2(datasize,data);//Criar bloco dados no content.value da nova mensagem
      return NewMessage;         
   }
   if(NewMessage->c_type==CT_RESULT){
      int FormatoRede;
      memcpy(&FormatoRede,msg_buf+offset,sizeof(int));//Copiar de msg_buf+offset para FormatoRede(em int)
      NewMessage->content.result=ntohl(FormatoRede);//Transformar content.result em codigo maquinaatraves do conteudo da variavel FormatoRede
      offset+=sizeof(int);//Incrementar offset
      return NewMessage; 
   }
   free(NewMessage);//Desalocar memoria para a nova mensagem
   NewMessage=NULL;
   return NewMessage;
}



long long swap_bytes_64(long long number)
{
   long long new_number;

   new_number =   ((number & 0x00000000000000FF) << 56 |
         (number & 0x000000000000FF00) << 40 |
         (number & 0x0000000000FF0000) << 24 |
         (number & 0x00000000FF000000) << 8  |
         (number & 0x000000FF00000000) >> 8  |
         (number & 0x0000FF0000000000) >> 24 |
         (number & 0x00FF000000000000) >> 40 |
         (number & 0xFF00000000000000) >> 56);

   return new_number;
}


void free_message(struct message_t *message){
   if(message->c_type == CT_ENTRY){
      entry_destroy(message->content.entry);
   }	
   if(message->c_type == CT_KEY){
      free(message->content.key);
   }
   if(message->c_type == CT_KEYS){
      int i;
      for(i=0; message->content.keys[i] !=NULL; i++) {
         free(message->content.keys[i]);
      }
      free(message->content.keys);
   }
   if(message->c_type == CT_VALUE){
      data_destroy(message->content.value);
   }

   free(message);		
}
int read_all(int sockfd, void *buf, int len) {
   int bufsize = len;
   while(len>0) {
      int res = read(sockfd, buf, len);
      if(res<0) {
         if(errno==EINTR) continue;
         perror("read failed:");
         return res;
      }
      buf += res;
      len -= res;
   }
   return bufsize;
}

int write_all(int sockfd, void *buf, int len) {
   int bufsize = len;
   while(len>0) {
      int res = write(sockfd, buf, len);
      if(res<0) {
         if(errno==EINTR) continue;
         perror("write failed:");
         return res;
      }
      buf += res;
      len -= res;
   }
   return bufsize;
}



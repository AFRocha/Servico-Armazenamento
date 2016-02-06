#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** 
 *  test_data
 *  Autor: Andre Rocha
 */


int testCreate() {
   int result;
   struct data_t *data = data_create(1024);

   memcpy(data->data,"1234567890a",strlen("1234567890a")+1);

   result = (strcmp(data->data,"1234567890a") == 0) && (data->datasize == 1024);

   data_destroy(data);

   printf("Modulo data -> teste data_create: %s\n",result?"passou":"nao passou");
   return result;
}

int testCreate2() {
   int result, data_size = strlen("1234567890abc")+1;
   char *data_s = strdup("1234567890abc");
   struct data_t *data = data_create2(data_size,data_s);

   result = (data->data == data_s) && 
         (data->datasize == data_size);

   data_destroy(data);

   printf("Modulo data -> teste data_create2: %s\n",result?"passou":"nao passou");
   return result;
}

int testDup() {
   int result, data_size = strlen("1234567890abc")+1;
   char *data_s = strdup("1234567890abc");
   struct data_t *data = data_create2(data_size,data_s);

   struct data_t *data2 = data_dup(data);

   result = (data->data != data2->data) && 
         (data->datasize == data2->datasize) &&
         (memcmp(data->data, data2->data, data->datasize) == 0);

   data_destroy(data);
   data_destroy(data2);

   printf("Modulo data -> teste data_dup: %s\n",result?"passou":"nao passou");
   return result;
}

int main() {
   int score = 0;

   printf("Iniciando o teste do modulo data \n");

   score += testCreate();

   score += testCreate2();

   score += testDup();

   printf("Resultado do teste do modulo data: %d/3\n",score);

   return score;
}

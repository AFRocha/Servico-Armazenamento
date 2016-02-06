#include "entry.h"
#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** 
 *  test_entry
 *  Autor: Andre Rocha
 */


int testCreate() {
   char *key = strdup("123abc");
   struct data_t *value = data_create2(strlen("1234567890abc")+1,strdup("1234567890abc"));

   struct entry_t *entry = entry_create(key,value);

   int result = (entry->key == key) && 
         (entry->value == value);

   entry_destroy(entry);

   printf("Modulo entry -> teste entry_create: %s\n",result?"passou":"nao passou");
   return result;
}

int testDup() {
   char *key = strdup("123abc");
   struct data_t *value = data_create2(strlen("1234567890abc")+1,strdup("1234567890abc"));

   struct entry_t *entry = entry_create(key,value);

   struct entry_t *entry2 = entry_dup(entry);

   int result = (entry->key != entry2->key) &&
         (strcmp(entry->key,entry2->key) == 0) && 
         (entry->value != entry2->value) &&
         (entry->value->datasize == entry2->value->datasize) &&
         (memcmp(entry->value->data, entry2->value->data, entry->value->datasize) == 0);

   entry_destroy(entry);
   entry_destroy(entry2);

   printf("Modulo entry -> teste entry_dup: %s\n",result?"passou":"não passou");
   return result;
}

int main() {
   int score = 0;

   printf("Iniciando o teste do modulo entry\n");

   score += testCreate();

   score += testDup();

   printf("Resultados do teste do modulo entry: %d/2\n",score);

   return score;
}

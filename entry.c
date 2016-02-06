#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "data.h"

/** 
 *  Modulo ENTRY
 *  Autor: Andre Rocha
 */

struct entry_t *entry_create(char *key, struct data_t *data){

   struct entry_t *entry = (struct entry_t *) malloc (sizeof(struct entry_t));
   entry->key = key;
   entry->timestamp = 0;
   entry->value = data;

   return entry;

}

void entry_destroy(struct entry_t *entry){

   data_destroy(entry->value);
   free(entry->key);
   free(entry);

}

struct entry_t *entry_dup(struct entry_t *entry){

   struct entry_t *entryCopy = (struct entry_t *) malloc (sizeof(struct entry_t));
   entryCopy->key = strdup(entry->key);
   entryCopy->timestamp = entry->timestamp;
   entryCopy->value = data_dup(entry->value);
   return entryCopy;

}

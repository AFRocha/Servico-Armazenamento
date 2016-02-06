#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

/** 
 *  Modulo DATA
 *  Autor: Andre Rocha
 */

struct data_t *data_create(int size){

   struct data_t *dataBlock = (struct data_t *) malloc (sizeof(struct data_t));
   dataBlock->datasize = size;
   dataBlock->data = (void *) malloc ( dataBlock->datasize );

   return dataBlock;
}

struct data_t *data_create2 (int size, void *data){

   struct data_t *dataBlock2 = (struct data_t *) malloc (sizeof(struct data_t));
   dataBlock2->datasize = size;
   dataBlock2->data = data;
   return dataBlock2;
}

void data_destroy(struct data_t *data){

   free(data->data);
   free(data);
}

struct data_t *data_dup(struct data_t *data){

   struct data_t *dataCopy = data_create(data->datasize);
   if( dataCopy != NULL )
      memcpy(dataCopy->data, data->data, dataCopy->datasize);
   return dataCopy;

}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "list-private.h"

/** 
 *  list-private
 *  Autor: Andre Rocha
 */


int listaVazia_add( struct list_t *list, struct No *no ){

   list->cabeca = no;
   list->cauda = no;
   list->size = 1;

   return 0;
}

int novaCabeca_add( struct list_t *list, struct No *no ){

   struct No *aux = list->cabeca;
   no->proximo = aux;
   list->cabeca = no;
   aux->anterior = no;
   list->size += 1;

   return 0;
}

int novaCauda_add( struct list_t *list, struct No *no ){

   struct No *aux = list->cauda;
   no->anterior = aux;
   list->cauda = no;
   aux->proximo = no;
   list->size += 1;

   return 0;
}

int noMeio_add( struct list_t *list, struct No *no, struct No *limite ){

   no->proximo = limite->proximo;
   no->anterior = limite;
   limite->proximo->anterior = no;
   limite->proximo = no;
   list->size += 1;

   return 0;
}

struct No *encontraNo( struct list_t *list, char *key ){

   struct No *aux = list->cabeca;

   if( list->cabeca == NULL )
      return NULL;

   if( strcmp( list->cabeca->entry->key, key ) < 0 )
      return NULL;

   if( strcmp( list->cauda->entry->key, key ) > 0 )
      return NULL;

   while ( aux != NULL && strcmp( aux->entry->key, key ) != 0 )
      aux = aux->proximo; 
   return aux;
}

void destroiNo( struct No *no ){

   entry_destroy( no->entry );
   free( no );

}

int removeUni( struct list_t *list ){

   struct No *aux = list->cabeca;

   list->cabeca = NULL;
   list->cauda = NULL;
   destroiNo ( aux );
   list->size = 0;

   return 0;

}

int removeCabeca( struct list_t *list ){

   struct No *aux = list->cabeca;

   list->cabeca = aux->proximo;
   aux->proximo->anterior = NULL;
   destroiNo( aux );
   list->size -= 1;

   return 0;

}

int removeCauda( struct list_t *list ){

   struct No *aux = list->cauda;

   list->cauda = aux->anterior;
   aux->anterior->proximo = NULL;
   destroiNo( aux );
   list->size -= 1;

   return 0;

}

int removeMeio( struct list_t *list, struct No *no ){

   no->proximo->anterior = no->anterior;
   no->anterior->proximo = no->proximo;
   destroiNo( no );
   list->size -= 1;

   return 0;

}






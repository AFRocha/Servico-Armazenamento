#include <stdio.h>  
#include <stdlib.h>
#include <assert.h>   
#include <string.h>
#include "list.h"
#include "data.h"
#include "entry.h"
#include "list-private.h"

/** 
 *  list
 *  Autor: Andre Rocha
 */



struct list_t *list_create(){

   /* reservar memoria para armazenar uma estrutura lista */
   struct list_t *list = (struct list_t *) malloc ( sizeof ( struct list_t ) );

   /* inicializar atributos: */
   list->cabeca = NULL;
   list->cauda = NULL;
   list->size = 0 ;
   return list;
}


//Aqui vamos destruir a lista
int list_destroy ( struct list_t *list ){
   struct No *aux;
   struct No *percorre = list->cabeca;
   if( percorre != NULL ){   
      while( percorre->proximo != NULL ){
         aux = percorre->proximo;
         destroiNo( percorre );
         percorre = aux;
      }
      destroiNo( percorre );
   }
   free ( list );

   return 0;
}



//Aqui vamos adicionar uma entry na lista. A lista eh ordenada.
//Nova entry eh colocada no local correto.
//Retorna 0(ok), ou -1(erro)

int list_add ( struct list_t *list, struct entry_t *entry ){

   struct No *aux = list->cabeca;
   struct No *novo_no = (struct No *) malloc( sizeof( struct No ) );
   novo_no->entry = entry;
   novo_no->proximo = NULL;
   novo_no->anterior = NULL;

   if( aux==NULL )
      return listaVazia_add( list, novo_no );    
   if( strcmp( list->cabeca->entry->key, entry->key) < 0 )
      return novaCabeca_add( list, novo_no );
   if( strcmp( list->cauda->entry->key, entry->key) > 0 )
      return novaCauda_add( list, novo_no );

   while ( aux->proximo != NULL && strcmp( aux->proximo->entry->key, entry->key ) > 0)
      aux = aux->proximo;
   return noMeio_add( list, novo_no, aux );
   return -1;

}



/* Vamos eliminar da lista um elemento com a chave key. 
 * A funcao retorna 0 (OK) ou -1 (erro)
 */



int list_remove(struct list_t *list, char *key){

   struct No *no = encontraNo( list, key );

   if( no != NULL ){
      if( no->proximo == NULL && no->anterior == NULL )
         return removeUni( list );
      if( no->anterior == NULL )
         return removeCabeca( list );
      if( no->proximo == NULL )
         return removeCauda( list );
      return removeMeio( list, no );
   }
   return -1;
}
/* Obtemos um elemento da lista com a chave key. 
 * Retornamos a referencia do elemento na lista ( uma alteração
 * implica alterar o elemento na lista).
 */


struct entry_t *list_get(struct list_t *list, char *key){

   struct No *no = encontraNo( list, key );

   if( no != NULL )
      return no->entry;
   return NULL;

}



//Contar o numero de nos ao longo da lista 

int list_size(struct list_t *list){

   return list->size;

}


/* Devolve um array de char* com a cópia de todas as keys da
 * lista, com um ultimo elemento a NULL.
 */

char **list_get_keys(struct list_t *list){

   if( list->cabeca == NULL ){
      return NULL;
   }
   int n = list_size( list );
   struct No *percorre = list->cabeca;
   char **arrayKeys = (char **) malloc ( (1 + n) * sizeof( char * ));
   int i;
   for( i = 0; percorre != NULL; i++ ){
      arrayKeys[i] = strdup( percorre->entry->key );   
      percorre = percorre->proximo;
   }

   arrayKeys[n] = NULL;
   return arrayKeys;

}

void list_free_keys(char **keys){

   /* Vamos assumir que a lista de keys existe
    */
   int i;
   for( i = 0; keys[i] != NULL; i++ )
      free( keys[i] );
   free( keys );

}






#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H


/** 
 *  list-private
 *  Autor: Andre Rocha
 */

// Tipos privados do lista

#include "entry.h"
#include "list.h"


struct No {           /* armazena dados de um no */
   struct No *proximo;
   struct No *anterior;
   struct entry_t *entry;
};

struct list_t {      /* armazena dados de uma lista */
   struct No *cabeca;
   struct No *cauda;
   int size;
};

int listaVazia_add( struct list_t *list, struct No *no );
int novaCabeca_add( struct list_t *list, struct No *no );
int novaCauda_add( struct list_t *list, struct No *no );
int noMeio_add( struct list_t *list, struct No *no, struct No *limite );
struct No *encontraNo( struct list_t *list, char *key );
void destroiNo( struct No *no );
int removeUni( struct list_t *list );
int removeCabeca( struct list_t *list );
int removeCauda( struct list_t *list );
int removeMeio( struct list_t *list, struct No *no );

#endif


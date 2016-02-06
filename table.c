#include <stdio.h>  
#include <stdlib.h>
#include <assert.h>   
#include <string.h>
#include "list.h"
#include "data.h"
#include "entry.h"
#include "list-private.h"
#include "table.h"
#include "table-private.h"

/** 
 *  Modulo table
 *  Autor: Andre Rocha
 */

/* Função hash */

int hash( char *chave, int n ){

   int i, chaveTam, soma = 0;

   chaveTam = strlen( chave );
   if( chaveTam > 5 )
      soma = (int) chave[0] + (int) chave[1] + (int) chave[2] + (int) chave[chaveTam - 2] + (int) chave[chaveTam - 1];
   else
      for( i = 0; i < chaveTam; i++ )
         soma += (int) chave[i];

   return (soma % n);
}

/* Função para criar/inicializar uma nova tabela hash, com n  
 * linhas(n = módulo da função hash)
 */
struct table_t *table_create(int n){

   int i;
   struct table_t *tabela = ( struct table_t * ) malloc( sizeof( struct table_t ));

   tabela->modulo = n;
   tabela->lista = ( struct list_t ** ) malloc( tabela->modulo * sizeof( struct list_t * ));

   for( i = 0; i < tabela->modulo; i++ )
      tabela->lista[i] = list_create();

   return tabela;
}

/* Eliminar/desalocar toda a memoria
 */
void table_destroy(struct table_t *table){

   int i;

   for( i = 0; i < table->modulo; i++ )
      list_destroy( table->lista[i] );

   free( table->lista );
   free( table );

}

/* Funcao para adicionar um elemento na tabela.
 * A função vai copiar a key (string) e os dados num 
 * espaco de memoria alocado por malloc().
 * Se a key ja existe, vai substituir essa entrada 
 * pelos novos dados.
 * Devolve 0 (ok) ou -1 (out of memory)
 */
int table_put(struct table_t *table, char *key, struct data_t *data){

   struct No *colisaoNo = encontraNo( table->lista[hash( key, table->modulo )], key );
   struct data_t *dados = data_dup( data );

   if( colisaoNo != NULL ){
      data_destroy( colisaoNo->entry->value );
      colisaoNo->entry->value = dados;
      return 0;
   }

   char *chave = strdup( key );
   struct entry_t *entrada = entry_create( chave, dados );

   return list_add( table->lista[hash( key, table->modulo )], entrada );
}

/* Funcao para adicionar um elemento na tabela caso a chave
 * associada ao elemento nao exista na tabela. Caso a chave
 * exista, retorna erro.
 * A função vai copiar a key (string) e os dados num 
 * espaco de memoria alocado por malloc().
 * Devolve 0 (ok) ou -1 (chave existe ou out of memory)
 */
int table_conditional_put(struct table_t *table, char *key, struct data_t *data){

   struct No *colisaoNo = encontraNo( table->lista[hash( key, table->modulo )], key );

   if( colisaoNo != NULL )
      return -1;

   char *chave = strdup( key );
   struct data_t *dados = data_dup( data );
   struct entry_t *entrada = entry_create( chave, dados );

   return list_add( table->lista[hash( key, table->modulo )], entrada );
}

/* Funcao para obter um elemento da tabela.
 * O argumento key indica a chave da entrada da tabela. 
 * A função aloca memoria para armazenar uma *COPIA* 
 * dos dados da tabela e retorna este endereco. 
 * O programa a usar essa funcao deve
 * desalocar (utilizando free()) esta memoria.
 * Em caso de erro, devolve NULL
 */
struct data_t *table_get(struct table_t *table, char *key){

   struct data_t *dadosCpy;
   struct entry_t *entrada = list_get( table->lista[hash( key, table->modulo )], key );

   if( entrada == NULL )
      return NULL;
   if( entrada->value == NULL )
      return entrada->value;

   dadosCpy = data_dup( entrada->value );

   return dadosCpy;
}

/* Funcao para remover um elemento da tabela. Vai desalocar
 * toda a memoria alocada na respetiva operação table_put().
 * Devolve: 0 (ok), -1 (key not found)
 */
int table_del(struct table_t *table, char *key){

   return list_remove( table->lista[hash( key, table->modulo )], key );
}

/* Devolve o numero de elementos da tabela.
 */
int table_size(struct table_t *table){

   int i, nElementos = 0;

   for( i = 0; i < table->modulo; i++ )
      nElementos += list_size( table->lista[i] );

   return nElementos;
}

/* Devolve um array de char * com a copia de todas as 
 * keys da tabela, e um ultimo elemento a NULL.
 */
char **table_get_keys(struct table_t *table){

   int i, guardaPosArray = 0, nLista, n = table_size( table );
   char **arrayKeys = ( char ** ) malloc (( 1 + n ) * sizeof( char * ));
   char **listaKeys;

   for( i = 0; i < table->modulo; i++ ){
      listaKeys = list_get_keys( table->lista[i] );

      if( listaKeys != NULL ){
         nLista = list_size( table->lista[i] );
         memcpy( arrayKeys + guardaPosArray, listaKeys, nLista * sizeof( char *) );

         guardaPosArray += nLista;
         free( listaKeys );
      }
   }

   arrayKeys[n] = NULL;

   return arrayKeys;
}

/* Desaloca a memoria alocada por table_get_keys()
 */
void table_free_keys(char **keys){

   int i; 

   for( i = 0; keys[i] != NULL; i++ )
      free( keys[i] );
   free( keys );

}

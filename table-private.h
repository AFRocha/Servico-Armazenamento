#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

/** 
 *  Modulo table-private
 *  Autor: Andre Rocha
 */

#include "table.h"

struct table_t {      /* estrutura de uma tabela */
   struct list_t **lista;
   int modulo;
};

int hash( char *chave, int n );

#endif


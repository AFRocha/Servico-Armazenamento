#ifndef _NETWORK_CLIENT_PRIV_H
#define _NETWORK_CLIENT_PRIV_H

#include "network_client.h"
#define RETRY_TIME 2

/*
 *network_client-private.h
 *Andre Rocha 
 */

struct server_t
{
   struct sockaddr_in addr;
   int port;
   int enderecoIp;
   int sockfd;

};

#endif

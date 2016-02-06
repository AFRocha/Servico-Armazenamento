#ifndef _CLIENT_STUB_PRIV_H
#define _CLIENT_STUB_PRIV_H
#include "network_client-private.h"
#include "message-private.h"
#include "client_stub.h"

/*
 *client_stub-private.h
 *Andre Rocha
 */


struct rtable_t{
   struct server_t *server;
   struct message_t *msg;
};

#endif

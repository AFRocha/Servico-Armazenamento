all: data entry list message table table_client table_server secondaryServer

secondaryServer: secondaryServer.o message.o table.o entry.o data.o list.o list-private.o table_skel.o
	gcc secondaryServer.o message.o table.o entry.o data.o list.o list-private.o table_skel.o -o secondaryServer

table_server: table_server.o message.o table.o entry.o data.o list.o list-private.o table_skel.o client_stub.o network_client.o
	gcc table_server.o message.o table.o entry.o data.o list.o list-private.o table_skel.o client_stub.o network_client.o -o table_server

table_client: entry.o data.o network_client.o table_client.o message.o list.o list-private.o client_stub.o table_skel.o table.o 
	gcc entry.o data.o network_client.o table_client.o message.o list.o list-private.o client_stub.o table_skel.o table.o -o table_client

message: message.o test_message.o data.o entry.o list.o list-private.o 
	gcc message.o test_message.o data.o entry.o list.o list-private.o -o message

table: table.o test_table.o list.o entry.o data.o list-private.o 

data: data.o test_data.o
	gcc data.o test_data.o -o data

entry: entry.o data.o test_entry.o
	gcc entry.o test_entry.o data.o -o entry

list: list.o entry.o data.o test_list.o entry.h list-private.h list.h data.h list-private.o

test: data.o entry.o list.o test_list.o list-private.o
	gcc -o test data.o entry.o list.o test_list.o list-private.o
	
secondaryServer.o: secondaryServer.c message.h inet.h network_client-private.h table.h table-private.h list.h message-private.h network_client.h data.h entry.h table_skel.h

table_skel.o: table_skel.c table_skel.h table.h inet.h table-private.h

client_stub.o: client_stub.c inet.h client_stub.h client_stub-private.h list.h list-private.h table.h

table_server.o: table_server.c message.h inet.h network_client-private.h table.h table-private.h list.h message-private.h network_client.h data.h entry.h client_stub.h table_skel.h

network_client.o: network_client.c message.h inet.h network_client-private.h message.o message-private.h

table_client.o: table_client.c data.h entry.h network_client.h inet.h message.h network_client-private.h client_stub.h

table.o: table.c data.h list-private.h table.h table-private.h entry.h

data.o: data.c data.h
	gcc -o data.o -c data.c

entry.o: entry.c entry.h data.h
	gcc -o entry.o -c entry.c

list.o: list.c list.h list-private.h entry.h data.h
	gcc -o list.o -c list.c

test_message.o: test_message.c list.h message-private.h message.h
	gcc -o test_message.o -c test_message.c

message.o: message.c list-private.h data.h entry.h message-private.h inet.h
	gcc -o message.o -c message.c

test_list.o: test_list.c list-private.h data.h entry.h
	gcc -o test_list.o -c test_list.c

list-private.o: list-private.c list-private.h
	gcc -o list-private.o -c list-private.c

clean:	
	echo Limpeza Concluida!
	rm -r *.o
	rm  data
	rm entry 
	rm list
	rm message
	rm table
	rm table_server
	rm table_client
	rm secondaryServer

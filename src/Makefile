all: mkey userver uclient

mkey: function_key.o
	$(CC)  $(LDFLAGS)  function_key.o -o mkey

function_key.o: function_key.c
	$(CC)  $(CFLAGS)   -c  function_key.c

userver: server.o count.o
	$(CC)  $(LDFLAGS)  server.o count.o -o userver

uclient: client.o count.o
	$(CC)  $(LDFLAGS)  client.o count.o -o uclient

server.o: server.c
	$(CC)  $(CFLAGS)   -c  server.c

client.o: client.c
	$(CC)  $(CFLAGS)   -c  client.c

count.o: count.c
	$(CC)  $(CFLAGS)   -c  count.c

clean:
	    rm *.o mkey userver uclient

all: mkey userver uclient mkeyd ks pn532

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

mkeyd: mkeyd.o
	$(CC)  $(LDFLAGS)  mkeyd.o -o mkeyd

mkeyd.o: mkeyd.c
	$(CC)  $(CFLAGS)   -c  mkeyd.c

ks: subscriber.o
	$(CC)  $(LDFLAGS)  subscriber.o -o ks

subscriber.o: subscriber.c
	$(CC)  $(CFLAGS)   -c  subscriber.c

pn532: pn532.o
	$(CXX)  $(LDFLAGS)  pn532.o -o pn532

pn532.o: pn532.cpp
	$(CXX)  $(CFLAGS)   -c  pn532.cpp

clean:
	    rm *.o mkey userver uclient mkeyd ks pn532

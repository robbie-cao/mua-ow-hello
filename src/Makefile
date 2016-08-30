all: target
FORCE: ;
.PHONY: FORCE

target: function_key userver uclient mkeyd ks pn532 pn532-writeurl

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cxx
	@$(CXX) $(CFLAGS) -c $< -o $@

fucntion_key: function_key.o
	$(CC) $(LDFLAGS) function_key.o -o function_key

userver: server.o count.o
	$(CC) $(LDFLAGS) server.o count.o -o userver

uclient: client.o count.o
	$(CC) $(LDFLAGS) client.o count.o -o uclient

mkeyd: mkeyd.o
	$(CC) $(LDFLAGS) mkeyd.o -o mkeyd

ks: subscriber.o
	$(CC) $(LDFLAGS) subscriber.o -o ks

pn532: pn532.o
	$(CXX) $(LDFLAGS) pn532.o -o pn532

pn532-writeurl: pn532-writeurl.o
	$(CXX) $(LDFLAGS) pn532-writeurl.o -o pn532-writeurl

clean:
	rm -f *.o mkey userver uclient mkeyd ks pn532


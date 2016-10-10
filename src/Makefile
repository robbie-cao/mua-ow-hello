all: target
FORCE: ;
.PHONY: FORCE

target: function_key function_key_uart userver uclient mkeyd ks pn532 pn532-writeurl caller pn532-drv blobmsg log-test ulog-test

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cxx
	@$(CXX) $(CFLAGS) -c $< -o $@

fucntion_key: function_key.o
	$(CC) $(LDFLAGS) function_key.o -o function_key

fucntion_key_uart: function_key_uart.o
	$(CC) $(LDFLAGS) function_key_uart.o -o function_key_uart

userver: server.o count.o
	$(CC) $(LDFLAGS) server.o count.o -o userver

uclient: client.o count.o
	$(CC) $(LDFLAGS) client.o count.o -o uclient

mkeyd: mkeyd.o
	$(CC) $(LDFLAGS) mkeyd.o -o mkeyd

ks: subscriber.o
	$(CC) $(LDFLAGS) subscriber.o -o ks

caller: caller.o
	$(CC) $(LDFLAGS) caller.o -o caller

pn532: pn532.o
	$(CXX) $(LDFLAGS) pn532.o -o pn532

pn532-writeurl: pn532-writeurl.o
	$(CXX) $(LDFLAGS) pn532-writeurl.o -o pn532-writeurl

pn532-drv: pn532-drv.o
	$(CXX) $(LDFLAGS) pn532-drv.o -o pn532-drv

#9160: 9160.o
#	$(CXX) $(LDFLAGS) 9160.o -o 9160

blobmsg: blobmsg-example.o
	$(CXX) $(LDFLAGS) blobmsg-example.o -o blobmsg

log-test: log.o log-test.o
	$(CC) $(LDFLAGS) $^ -o $@

ulog-test: ulog-test.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o function_key function_key_uart userver uclient mkeyd ks pn532 pn532-writeurl caller pn532-drv blobmsg log-test ulog-test


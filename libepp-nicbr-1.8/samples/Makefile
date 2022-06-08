CC:=g++ -g 
CONFIG:= -DHAVE_CONFIG_H 
INCLUDE:= -I .. -I ../include/ -I /usr/local/include/ 
LIB:= -L ../lib/ -L /usr/local/lib 
LIBEPP:= -lepp-nicbr-1.5
LIBXERCES:= -lxerces-c 
LIBSSL:= -lssl 
src:= TestClient.cpp 
exec:= -o TestClient 

all: 
	$(CC) $(CONFIG) $(INCLUDE) $(src) $(LIB) $(LIBEPP) \
	$(LIBXERCES) $(LIBSSL) $(exec)

test:

	cd ../unit_test/ && ./SessionTestServer
	./TestClient

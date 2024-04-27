CFLAGS=-I /opt/homebrew/include
LIBS=-L /opt/homebrew/lib -lssl -lcrypto -lcurl

all: curl raw udp_server udp_client

curl: curl.c
	$(CC) $(CFLAGS) curl.c -o curl $(LIBS)

raw: raw.c
	$(CC) $(CFLAGS) raw.c -o raw $(LIBS)

udp_server: udp_server.c
	$(CC) $(CFLAGS) udp_server.c -o udp_server

udp_client: udp_client.c
	$(CC) $(CFLAGS) udp_client.c -o udp_client

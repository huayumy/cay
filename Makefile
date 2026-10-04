CC=cc
CFLAGS=-O2 -Wall
LDLIBS=-lcurl -lgit2
cay: cay.c aur.c rpc.c json.c git.c makepkg.c
	$(CC) $(CFLAGS) -o cay $^ $(LDLIBS)
clean:
	rm -f cay

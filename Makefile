CC = gcc
CFLAGS = -g

all : bsb_svr bsb_cli

bsb_svr : bsb_svr.c
	$(CC) $(CFLAGS) -o bsb_svr bsb_svr.c

bsb_cli : bsb_cli.c 
	$(CC) $(CFLAGS) -o bsb_cli bsb_cli.c

clear :rm *.o
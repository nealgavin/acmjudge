all:c_connect_mysql clean
OBJS=c_connect_mysql.o
CC=gcc
CFLAGS=-Wall 
INCLUDE=/usr/include/mysql -g -pipe
LIB=/usr/lib/mysql -lmysqlclient -lz -lm
c_connect_mysql:$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o c_connect_mysql -L$(LIB)
c_connect_mysql.o:c_connect_mysql.c
	$(CC) $(CFLAGS) -c c_connect_mysql.c -o c_connect_mysql.o -I$(INCLUDE)

clean:
	rm -rf *.o

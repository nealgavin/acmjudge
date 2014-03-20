all:judge clean
OBJS=judge.o judge_function.o
CC=gcc
CFLAGS=-Wall 
INCLUDE=/usr/include/mysql -g -pipe
LIB=/usr/lib/mysql -lmysqlclient -lz -lm
judge:$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o judge -L$(LIB)
c_connect_mysql.o:c_connect_mysql.c
	$(CC) $(CFLAGS) -c c_connect_mysql.c -o c_connect_mysql.o -I$(INCLUDE)
judge.o:judge.c
	$(CC) $(CFLAGS) -c judge.c -o judge.o -I$(INCLUDE)
judge_function.o:judge_function.c
	$(CC) $(CFLAGS) -c judge_function.c -o judge_function.o -I$(INCLUDE)
clean:
	rm -rf *.o

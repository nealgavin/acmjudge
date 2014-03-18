#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
char *Host = "localhost";
char *Name = "root";
char *Passwd = "root";
char *Database = "acm";
MYSQL *conn;
int Port = 3306;
int main()
{
	conn = mysql_init(NULL);
	if (!conn)
	{
		fprintf(stderr,"mysql_init failed\n");
		return EXIT_FAILURE;
	}
	conn = mysql_real_connect(conn,Host,Name,Passwd,Database,Port,NULL,0);
	if(conn)
		printf("connect success\n");
	else
		printf("connect failed\n");
	mysql_close(conn);
	return EXIT_SUCCESS;
}

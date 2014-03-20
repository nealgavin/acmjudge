#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>

#define DEBUG
//char *Host = "localhost";
//char *Name = "root";
//char *Passwd = "root";
//char *Database = "acm";
//MYSQL *conn;
//int Port = 3306;

int mysql_connect(MYSQL*conn,char*Host,char*Name,char*Passwd,char*Database,int Port)
{
	conn = mysql_init(NULL);
	if(!conn)
	{
	#ifdef DEBUG
		printf("mysql init failed\n");
	#endif
		return EXIT_FAILURE;
	}
	conn = mysql_real_connect(conn,Host,Name,Passwd,Database,Port,NULL,0);
	#ifdef DEBUG
	if(conn)
		{
			puts("connect success!");
		}
	else 
		puts("connect failed!");
	#endif
	if(conn)
		return 1;
	else 
		return 0;
}

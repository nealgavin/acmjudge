#include "allhead.h"
#define DEBUG
char *Host = "localhost";
char *Name = "root";
char *Passwd = "root";
char *Database = "judgeonline";
MYSQL *conn = NULL;
int Port = 3306;
int main(int agrs,char*arg[])
{
	conn = mysql_init(NULL);
	conn = mysql_real_connect(conn,Host,Name,Passwd,Database,Port,NULL,0);
	#ifdef DEBUG
	if(conn != NULL)
		puts("connect success");
	else 
		puts("connect failed");
	#endif
	while(true)
	{
		onlinejudge(conn);
		#ifdef DEBUG
		pause();
		#endif
	}
	return 1;
}

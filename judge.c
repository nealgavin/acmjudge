#include "allhead.h"
#define DEBUG
char *Host = "localhost";
char *Name = "root";
char *Passwd = "root";
char *Database = "judgeonline";
MYSQL *conn = NULL;
int Port = 3306;
void judge_stop(int sig)
{
	if(conn != NULL)
		mysql_close(conn);
	puts("------------------------------");
	puts("--------byebye----------------");
	puts("------------------------------");
	signal(sig,SIG_DFL);
	raise(sig);
}
int main(int agrs,char*arg[])
{
	signal(SIGTERM,judge_stop);
	signal(SIGINT,judge_stop);
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
		puts("another judge!");
		sleep(1);
		#endif
	}
	if(conn != NULL)
		mysql_close(conn);
	return 1;
}

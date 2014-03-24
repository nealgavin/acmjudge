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
    memset(JavasafeSysCall,0,sizeof(JavasafeSysCall));
	JavasafeSysCall[3] = 1;
	JavasafeSysCall[4] = 1;
	JavasafeSysCall[5] = 1;
	JavasafeSysCall[6] = 1;
	JavasafeSysCall[10] = 1;
	JavasafeSysCall[11] = 1;
	JavasafeSysCall[33] = 1;
	JavasafeSysCall[37] = 1;
	JavasafeSysCall[39] = 1;
	JavasafeSysCall[45] = 1;
	JavasafeSysCall[75] = 1;
	JavasafeSysCall[78] = 1;
	JavasafeSysCall[85] = 1;
	JavasafeSysCall[90] = 1;
	JavasafeSysCall[91] = 1;
	JavasafeSysCall[93] = 1;
	JavasafeSysCall[102] = 1;
	JavasafeSysCall[120] = 1;
	JavasafeSysCall[122] = 1;
	JavasafeSysCall[125] = 1;
	JavasafeSysCall[140] = 1;
	JavasafeSysCall[141] = 1;
	JavasafeSysCall[149] = 1;
	JavasafeSysCall[156] = 1;
	JavasafeSysCall[158] = 1;
	JavasafeSysCall[174] = 1;
	JavasafeSysCall[175] = 1;
	JavasafeSysCall[183] = 1;
	JavasafeSysCall[191] = 1;
	JavasafeSysCall[192] = 1;
	JavasafeSysCall[195] = 1;
	JavasafeSysCall[196] = 1;
	JavasafeSysCall[197] = 1;
	JavasafeSysCall[199] = 1;
	JavasafeSysCall[200] = 1;
	JavasafeSysCall[201] = 1;
	JavasafeSysCall[202] = 1;
	JavasafeSysCall[220] = 1;
	JavasafeSysCall[221] = 1;
	JavasafeSysCall[224] = 1;
	JavasafeSysCall[240] = 1;
	JavasafeSysCall[243] = 1;
	JavasafeSysCall[252] = 1;
	JavasafeSysCall[258] = 1;
	JavasafeSysCall[311] = 1;

	memset(safeSysCall,0,sizeof(safeSysCall));
	safeSysCall[3] = 1;
	safeSysCall[4] = 1;
	safeSysCall[5] = 1;
	safeSysCall[6] = 1;
	safeSysCall[11] = 1;
	safeSysCall[45] = 1;
	safeSysCall[90] = 1;
	safeSysCall[91] = 1;
	safeSysCall[122] = 1;
	safeSysCall[175] = 1;
	safeSysCall[192] = 1;
	safeSysCall[197] = 1;
	safeSysCall[243] = 1;
	safeSysCall[252] = 1;
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

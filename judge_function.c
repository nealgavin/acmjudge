#include "allhead.h"
#define DEBUG
Submits submit;
int compiler(MYSQL *conn,Submits *submit)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	char sql[330],path[220];
	sprintf(sql,"select source from source_code where solution_id='%d'",submit->solution_id);
	if(mysql_query(conn,sql))
	{
		#ifdef DEBUG
		printf("failed query\nsql : %s\n",sql);
		#endif
		return 0;
	}
	res = mysql_store_result(conn);

	if(submit->language == 1)
	{
		sprintf(path,"./src/cc/%d.cpp",submit->solution_id);
		FILE *code;
		if ((code = fopen(path,"wb")) == NULL)
		{
			fprintf(stderr,"can't open file\n");
			return 0;
		}
	}
	else if(submit->language == 3)
	{
		sprintf(path,"./src/cc/%d.cpp",submit->solution_id);
		sprintf(sql,"/usr/java/jdk1.7/bin/javac ./src/java/%d/Main.java -d -d ./src/java/%d",submit->solution_id,submit->solution_id);
	}
	return 1;
}

void onlinejudge(MYSQL *conn)
{
	char sql[330],path[220];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;
	pid_t pid_son;
	int num_process = 0;
	/**************************************
	 * get the data submit form mysql
	 * ***********************************/
	sprintf(sql,"select solution_id,problem_id,user_id,contest_id,num,stime,ntime,language from solution where result='%d' order by solution_id limit 1",UNJUDGE);
	if(mysql_query(conn,sql))
	{
		#ifdef DEBUG
		///接续两个query第二个就跪了
		puts("get data from mysql failed");
		printf("find error: %s\n",sql);
		#endif
		return;
	}
	#ifdef DEBUG
	else puts("mysql query  success");
	#endif
	
	res = mysql_store_result(conn);
	#ifdef DEBUG
	puts("out");
	#endif
	while( (row = mysql_fetch_row(res)) != NULL)
	{	
		#ifdef DEBUG
		puts("in while");
		#endif
		sscanf(row[0],"%d", &submit.solution_id);
		sscanf(row[1],"%d", &submit.problem_id);
		strcpy(submit.user_id,row[2]);
		sscanf(row[3],"%d", &submit.contest_id);
		sscanf(row[4],"%d", &submit.num);
		sscanf(row[5],"%lf", &submit.stime);
		sscanf(row[6],"%lf", &submit.ntime);
		sscanf(row[7],"%d", &submit.language);
		submit.result = 10;
		///result = 10 is in wait
		printf("\tSolution %d is Compiling ...\n",submit.solution_id);

		while(1)
		{
			if(num_process <= 3)
			{	
				pid_son = fork();
				if(pid_son == 0)
				{
					//MYSQL *son_conn = mysql_init(NULL);
					if(compiler(conn,&submit))
					{
						#ifdef DEBUG
						puts("compiler success");
						#endif
					}
					else
					{
						#ifdef DEBUG
						puts("CE");
						#endif
					}
				}
			}
		}
	}
	mysql_free_result(res);
	//compiler(conn);
}

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
		return -1;
	}
	res = mysql_store_result(conn);

	if(submit->language == 1)
	{
		sprintf(path,"./src/cc/%d.cpp",submit->solution_id);
		FILE *code;
		//mode_t f_attrib;
		if(access(path,F_OK) == 0)//file is exit
		{
			puts("file is exit");
		}
		else 
		{
			creat(path,0666);
			#ifdef DEBUG
			printf("file create %s\n",path);
			#endif
		}
		if( (code = fopen(path,"wb")) == NULL)
		{
			fprintf(stderr,"can't open\n");
			return -1;
		}
		while((row = mysql_fetch_row(res)) != NULL )
		{
			fprintf(code,"%s",row[0]);
			//printf("data:%s\n",row[0]);
		}           
		// fflush(code);
		fclose(code);
		sprintf(path,"dos2unix ./src/cc/%d.cpp",submit->solution_id);
		sprintf(sql,"g++  ./src/cc/%d.cpp -o ./exe/%d  2> ./error/%d.txt",submit->solution_id,submit->solution_id,submit->solution_id);
	}
	else if(submit->language == 3)
	{
		sprintf(path,"mkdir ./src/java/%d",submit->solution_id);
		system(path);
		sprintf(path,"./src/java/%d/Main.java",submit->solution_id);
		FILE *code;
		if(access(path,F_OK) == 0)
		{
			puts("java file exit");
		}
		else
		{
			creat(path,0666);
		}
		if( (code = fopen(path,"wb")) == NULL)
		{
			fprintf(stderr,"can't open java file\n");
			return -1;
		}
		while((row = mysql_fetch_row(res)) != NULL)
		{
			fprintf(code,"%s",row[0]); ///只是到缓冲区
		}
		fclose(code);
		sprintf(path,"dos2unix ./src/java/%d/Main.java",submit->solution_id);
		sprintf(sql,"/usr/java/jdk1.7/bin/javac ./src/java/%d/Main.java -d -d ./src/java/%d 2> ./error/%d.txt",submit->solution_id,submit->solution_id,submit->solution_id);
	}
	mysql_free_result(res);
	if(system(path) == 0 && system(sql) == 0)//complie success
		return 1;
	sprintf(sql,"update solution set result=%d,time=%d,memory=%d where solution_id=%d",CE,0,0,submit->solution_id);
	if(mysql_query(conn,sql))
	{
		#ifdef DEBUG
		puts("god update failed");
		#endif
	}
	return -1;
}
void update_submit_status(MYSQL *conn,Submits *submit)
{
	char sql[330];
	sprintf(sql,"update solution set result='%d',time='%d',memory='%d' where solution_id='%d'",submit->result,(int)submit->run_t*1000,(int)(submit->run_m*1024),submit->solution_id);
	mysql_query(conn,sql);
	return;
}
int check_char(char ch)
{
	if(ch == '\n' || ch == '\t' || ch == '\b' || ch == ' ' || ch == 10 || ch == 13)
	//10 回车 13 换行
		return 0;
	else 
		return 1;
}

int compare_data(FILE *std,FILE *user)
{
	char a,b;
	int  result = 0;
	int a_end = 0,b_end = 0,pe = 0;
	while(true)
	{
		if(a_end == 0 && (a=fgetc(std)) == EOF)
		{
			a_end = 1;
		}
		if(b_end == 0 && (b=fgetc(user)) == EOF)
		{
			b_end = 1;
		}
		if(a_end && b_end)
			break;
		if(a != b)
		{
			if(check_char(a))
			{
				if(check_char(b))
					return WA;
				else
				{
					while(true)
					{
						if((b=fgetc(user))==EOF)
							{
								b_end = 1;
								return WA;
							}
						if(check_char(b))
							break;
					}
					if(a != b)
						return WA;
				}
			}
		 
		}
			return WA;
	}
	if(feof(std)&&feof(user))
		return AC;
	else 
		return WA;
}

void run_exe_and_check(MYSQL *conn,Submits *submit)
{
	/*****************************
	 * !!!:stdout用freopen 重定向后，定向不回去
	 * 只能先用子进程来解决下了
	 * ***************************/
	fflush(stdin);
	fflush(stdout);
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		char path[220];
		puts("now runing compare");
		sprintf(path,"./data/%d/data.in",submit->problem_id);
		freopen(path,"r",stdin);
		sprintf(path,"./tmp/%d.out",submit->solution_id);
		freopen(path,"w",stdout);
		if(submit->language == 1)
		{
			sprintf(path,"./exe/%d",submit->solution_id);
		}
		else if(submit->language == 3)
		{
			sprintf(path,"java ./src/java/%d",submit->solution_id);
		}
		system(path);
		fflush(stdin);
		fflush(stdout);
		exit(1);
	}		
		   char path[220];
		   int result = 0;
		   /**************************
			*wait() 等待子进程结束
			*否则有可能子进程文件未关闭或未写入导致后
			*序的处理文件打开失败
			*************************/
		    wait(NULL);
			sprintf(path,"./tmp/%d.out",submit->solution_id);
			FILE *user,*std_data;
				user = fopen(path,"r");
			sprintf(path,"./data/%d/data.out",submit->problem_id);
				std_data = fopen(path,"r");
				if(user == NULL)
				  puts("open failed");
				 else
				  puts("open success!");
			result = compare_data(std_data,user);
			submit->result = result;
			puts("compare result is here");
			update_submit_status(conn,submit);
			fclose(std_data);
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
	while( (row = mysql_fetch_row(res)) != NULL)
	{	
		sscanf(row[0],"%d", &submit.solution_id);
		sscanf(row[1],"%d", &submit.problem_id);
		strcpy(submit.user_id,row[2]);
		sscanf(row[3],"%d", &submit.contest_id);
		sscanf(row[4],"%d", &submit.num);
		sscanf(row[5],"%lf", &submit.stime);
		sscanf(row[6],"%lf", &submit.ntime);
		sscanf(row[7],"%d", &submit.language);
		submit.result = COMPLIERING;
		///result = 10 is in wait
		printf("\tSolution %d is Compiling ...\n",submit.solution_id);
		update_submit_status(conn,&submit);
		while(1)
		{
			if(num_process <= 3)
			{	
				pid_son = fork();
				if(pid_son == 0)
				{
					//MYSQL *son_conn = mysql_init(NULL);
					if(compiler(conn,&submit) == 1)
					{
						#ifdef DEBUG
						puts("compiler success");
						#endif
						submit.result=RUNNING;
						update_submit_status(conn,&submit);
						run_exe_and_check(conn,&submit);
						printf("\t%d judge finished\n",submit.solution_id);
					}
					else
					{
						#ifdef DEBUG
						puts("CE");
						#endif
					}
				}
				if(pid_son == 0)//结束子进程
				exit(0);
				else 
				{
					//主进程数，并跳出循环
					++num_process;
					break;
				}
			}
		}
	}
	mysql_free_result(res);
	//compiler(conn);
}

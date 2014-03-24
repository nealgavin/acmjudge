#include "allhead.h"
#define DEBUG
Submits submit;
pid_t pid;
jmp_buf jmpbuffer;
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
	sprintf(sql,"update solution set result='%d',time='%d',memory='%d' where solution_id='%d'",submit->result,(int)(submit->run_t*1000.0),(int)(submit->run_m),submit->solution_id);
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
	int a_end = 0,b_end = 0,pe = 0;
	while(true)
	{
		if((a=fgetc(std)) == EOF)
		{
			a_end = 1;
		}
		if((b=fgetc(user)) == EOF)
		{
			b_end = 1;
		}
		if(a_end && b_end)
			break;
			printf("std-%d-",a);
		putchar(a);
			putchar('-');
			printf("usr-%d-",b);
		putchar(b);
			putchar('-');
		if(a != b)
		{
			puts("\nno");
		}
		else
			puts("\nyse");
		if(a_end)
		{
			pe = 1;
			while((b=fgetc(user))!=EOF)
			{
				if(check_char(b));
					return WA;
			}
			break;
		}
		if(b_end)
		{
			pe = 1;
			while((a=fgetc(std))!=EOF)
			{
				if(check_char(a))
					return WA;
			}
			break;
		}
		if(a != b)
		{
			pe = 1;
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
			else if (check_char(b))
			{
				while(true)
				{
					if((a=fgetc(std))==EOF)
					{
						a_end = 1;
						return WA;
					}
					if(check_char(a))
						break;
				}
				if(a != b)
				return WA;
			}
		}
	}
	if(pe == 0)
		return AC;
	else 
		return PE;
}
void TIMEOUT(int pid)
{
	ptrace(PTRACE_DETACH,pid,0,0);
	kill(pid,SIGCONT);
	kill(pid,SIGKILL);
	longjmp(jmpbuffer,1);
}
void run_exe_and_check(MYSQL *conn,Submits *submit)
{
	/*****************************
	 * !!!:stdout用freopen 重定向后，定向不回去
	 * 只能先用子进程来解决下了
	 * ***************************/
	char sql[330],path[220];
	MYSQL_ROW row;
	MYSQL_RES *res;

	sprintf(sql,"select time_limit,memory_limit from problem where problem_id='%d'",submit->problem_id);
	mysql_query(conn,sql);
	res = mysql_store_result(conn);
	if((row=mysql_fetch_row(res))!=NULL)
	{
		sscanf(row[0],"%lf",&submit->time);
		sscanf(row[1],"%lf",&submit->memory);
	}
	printf("tim = %f  mem = %f\n",submit->time,submit->memory);
	mysql_free_result(res);
	fflush(stdin);
	fflush(stdout);
	puts("run_exe check");
	pid = fork();
	if(pid == 0)
	{
		puts("in pid here");
		struct rlimit t_limit,m_limit;
		getrlimit(RLIMIT_CPU,&t_limit);
		getrlimit(RLIMIT_DATA,&m_limit);
		t_limit.rlim_cur = submit->time/1000;//s
		t_limit.rlim_max = t_limit.rlim_cur;
		m_limit.rlim_cur = submit->memory*1024;//B
		m_limit.rlim_max = m_limit.rlim_cur;
		
		if(submit->language == 3)
		{
			t_limit.rlim_cur *= 3;
			t_limit.rlim_max = t_limit.rlim_cur;
			m_limit.rlim_cur *= 2;
			m_limit.rlim_max = m_limit.rlim_cur;
		}
		puts("error exits here?");
		setrlimit(RLIMIT_CPU,&t_limit);
		setrlimit(RLIMIT_DATA,&m_limit);

		puts("now runing compare");
		sprintf(path,"./data/%d/data.in",submit->problem_id);
		freopen(path,"r",stdin);
		sprintf(path,"./tmp/%d.out",submit->solution_id);
		freopen(path,"w",stdout);
		
		if(submit->language == 1)
		{
			sprintf(path,"./exe/%d",submit->solution_id);
			ptrace(PTRACE_TRACEME,pid,NULL,NULL);
			execl(path,NULL);
		}
		else if(submit->language == 3)
		{
			sprintf(path,"java ./src/java/%d",submit->solution_id);
			ptrace(PTRACE_TRACEME,pid,NULL,NULL);
			execl("/usr/java/jdk1.7/bin/java","-classpath",path,"Main",NULL);
		}

		fflush(stdin);
		fflush(stdout);
		exit(1);
	}		
		   /**************************
			*wait() 等待子进程结束
			*否则有可能子进程文件未关闭或未写入导致后
			*序的处理文件打开失败
			*************************/
			int result = 0,syscallID,insyscall,status,pstatus,returnValue,tag;
			struct rusage use;
			struct user_regs_struct reg;

		    signal(SIGALRM,TIMEOUT);
			alarm((int)submit->time*2);
			
			status = 0;
			if(setjmp(jmpbuffer))
			{
				status = 999;
				goto PTRACE_OVER;
			}
			wait4(pid,&pstatus,0,&use);
			if(WIFEXITED(pstatus))
				goto PTRACE_OVER;
			/***********************************************
			 * PEEKUSER是往USER区域中写入一个字节偏移为addr
			 * SYSCALL 重新执行
			 * 陷入内核时系统中断把调用号装入EAX寄存器
			 * 系统调用表是32位（4字节）要乘4
			 * ********************************************/
			syscallID = ptrace(PTRACE_PEEKUSER,pid,ORIG_EAX*4,NULL);
			ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
			insyscall = 0;
			tag = 0;
			while(true)
			{
				wait4(pid,&pstatus,0,&use);
				if(WIFEXITED(pstatus)||WIFSIGNALED(pstatus))
					break;
				if(insyscall == 0)
				{
					errno = 0;
					/****读入寄存器信息****/
					ptrace(PTRACE_GETREGS,pid,NULL,&reg);
					insyscall = 1;
					if(submit->language == 1)
					{
						if(syscallID<0 || syscallID>_SYSCALL_NUM || !safeSysCall[syscallID])
						{
							if(syscallID<0 || syscallID>_SYSCALL_NUM)
								{status= RE;puts("RE 1");}
							else 
								status = RF;
							ptrace(PTRACE_DETACH,pid,0,0);
							kill(pid,SIGCONT);
							kill(pid,SIGKILL);
							break;
						}
					}
					else if(submit->language == 3)
					{
						if(syscallID<0 || syscallID>_SYSCALL_NUM || !JavasafeSysCall[syscallID])
						{
							if(syscallID<0 || syscallID>_SYSCALL_NUM)
								{status = RE;puts("RE 2");}
							else
								status = RF;
							ptrace(PTRACE_DETACH,pid,0,0);
							kill(pid,SIGCONT);
							kill(pid,SIGKILL);
							break;
						}
					}
				}
				else
				{
					returnValue=ptrace(PTRACE_PEEKUSER,pid,ORIG_EAX*4,NULL);
					insyscall = 0;
				}
				ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
			}
PTRACE_OVER:
			alarm(0);
			
			if(status == 999)
			{
				status = TLE;
				goto JUDGE_OVER;
			}
			
			if(WIFEXITED(pstatus))
			{
				fflush(stdin);
				fflush(stdout);
				sprintf(path,"./tmp/%d.out",submit->solution_id);
				FILE *user,*std_data;
					user = fopen(path,"r");
				sprintf(path,"./data/%d/data.out",submit->problem_id);
					std_data = fopen(path,"r");
					if(std_data == NULL)
					{
						printf("\tcan't open file %d.out",submit->problem_id);
						submit->result = SYSTEM_ERR;
						update_submit_status(conn,submit);
						exit(1);
					}
				#ifdef DEBUG
					if(user == NULL)
					  puts("user open failed");
				#endif
				result = compare_data(std_data,user);
				status = result;
				puts("compare result is here");
				update_submit_status(conn,submit);
				fclose(std_data);
			}
			else if(WIFSIGNALED(pstatus))
			{
				if(WTERMSIG(pstatus)==SIGXCPU)
				{
					status = TLE;
				}
				else if(WTERMSIG(pstatus)==SIGKILL)
				{
					double passtimeS=(use.ru_stime.tv_sec+use.ru_utime.tv_sec);
					double passtimeU=(use.ru_stime.tv_usec+use.ru_utime.tv_usec)/1000000.0;
					if(passtimeS+passtimeU + 0.0002>submit->time/1000)
					{
						status = TLE;
					}
					else if(pstatus == 9||use.ru_minflt*4>=submit->memory)
					{
						status=MLE;
					}
				}
				else 
					{status = RE;puts("RE 3");}
			}
			else if(WIFSTOPPED(pstatus)&&WSTOPSIG(pstatus)!=5)
				{status = RE;puts("RE 4");}
			printf("mem = %d B pstatus=%d\n",use.ru_minflt*4,pstatus);
JUDGE_OVER:
			submit->result = status;
			if(status == AC || status == PE)
			{

					double passtimeS=(use.ru_stime.tv_sec+use.ru_utime.tv_sec);
					double passtimeU=(use.ru_stime.tv_usec+use.ru_utime.tv_usec)/1000000.0;
					printf("S=%lf U=%lf\n",passtimeS,passtimeU);
					submit->run_t = passtimeS + passtimeU;
					submit->run_m = use.ru_minflt*4/1024;
					printf("~~tm= %d mem = %lf time=%lf\n",use.ru_majflt,submit->run_m,submit->run_t);
				if(status == AC)
				{
					sprintf(sql,"update problem set accepted=accepted+1 where problem_id='%d'",submit->problem_id);
					mysql_query(conn,sql);
					sprintf(sql,"select distinct problem_id from solution where result='1' and user_id='%s' and problem_id='%d'",submit->user_id,submit->problem_id);
					mysql_query(conn,sql);
					res = mysql_store_result(conn);
					if(mysql_fetch_row(res))
					{
						mysql_free_result(res);
					}
					else
					{
						mysql_free_result(res);
						sprintf(sql,"update users set solved=solved+1 where user_id='%s'",submit->user_id);
						mysql_query(conn,sql);
					}
				}
			}
			else
			{
				submit->run_t = 0;
				submit->run_m = 0;
			}
			update_submit_status(conn,submit);
			exit(0);
			return;
}

void onlinejudge(MYSQL *conn)
{
	char sql[330];
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

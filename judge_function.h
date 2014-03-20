#include<mysql.h>

#define true 1
#define false 0
/**********************
 *the meaning of the num of Submit's result
 *********************/
#define UNJUDGE 0
#define AC 1
#define PE 2
#define TLE 3
#define MLE 4
#define WA 5
#define RE 6
#define STATUS_SIG 7
#define STATUS_AC 8
#define RUNNING 9
#define COMPLIERING 10
#define SYSTEM_ERR 11
#define RF 12

typedef struct{
	int solution_id;
	char user_id[30];
	int problem_id,contest_id;
	double time,memory;//时间空间限制
	double stime,ntime;
	double run_t,run_m;
	int language,result,num;
}Submits;
int compiler(MYSQL *conn,Submits *submit);

/**************************
 * just write for head the 
 * program needed
 *************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h> /*信号*/
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <asm/ptrace.h>
#include <errno.h>
#include <sys/resource.h>
#include <setjmp.h>
#include <errno.h>

//#include <io.h>
//#include <direct.h>
#include "judge_function.h"
_Bool safeSysCall[_SYSCALL_NUM + 1];
_Bool JavasafeSysCall[_SYSCALL_NUM+1];

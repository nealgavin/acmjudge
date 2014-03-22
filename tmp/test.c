#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE *file;
	if((file = freopen("fuck.txt","w",stdout)) == NULL)
		exit(1);
		puts("int here");
	file = freopen("CON","w",stdout);
	puts("out fuck");
}


#include <stdio.h>
int f[6000000];
int main()
{
	int a,b;
        f[123456]=999;
	scanf("%d %d",&a,&b);
	printf("%d\n",a+b);
	return 0;
}
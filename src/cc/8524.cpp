#include <stdio.h>
int f[6000000];
int main()
{
	int a,b,i;
        for(i=0;i<6000000;i++)
        f[i] = i;
	scanf("%d %d",&a,&b);
	printf("%d\n",a+b);
	return 0;
}
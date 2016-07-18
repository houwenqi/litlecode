#include <stdio.h>

int main()
{
	int a[11],n,i,j,t;

	for(i=0;i<=10;i++)
	{
		a[i]=0;
	}
	
	scanf("%d",&n);

	for(i=1;i<=n;i++)
	{
		scanf("%d",&t);
		a[t]++;
	}

	for(i=10;i>=0;i--)
	{
		for(j=1;j<=a[i];j++)
		{	
			printf("%d,", i);
		}
	}

	return 0;
}
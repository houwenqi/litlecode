#include <stdio.h>

int a[101],n;

int main()
{
	int i,j,k,flag;
	scanf("%d",&n);

	for(i=1;i<=n;i++)
	{
		scanf("%d",&a[i]);
	}

	for(i=1;i<=n-1;i++)
	{
		for(j=1;j<=n-i;j++)
		{
			if(a[j] > a[j+1])
			{
				k = a[j];
				a[j] = a[j+1];
				a[j+1] = k;
			}
		}
	}

	for(i=1;i<=n;i++)
	{
		printf("%d,",a[i] );
	}
	return 0;
}
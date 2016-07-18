#include <stdio.h>

void sort(int left,int right);

int a[101],n;

int main()
{
	int i,sum;

	scanf("%d",&n);
	sum = n;
	for(i=1;i<=n;i++)
	{
		scanf("%d",&a[i]);
		if(a[i] = a[i+1])
			sum --;
	}

	sort(1,sum);

	printf("sum is %d\n",sum );
	
	for(i=1;i<=sum;i++)
	{
		printf("%d,",a[i] );
	}

	return 0;
}

void sort(int left,int right)
{
	int i,j,k,flag;
	flag = a[left];
	i = left;
	j = right;

	while(i != j)
	{
		while(a[j] >= flag && i<j)
			j--;
		while(a[i] <= flag && i<j)
			i++;
		if(i < j)
		{
			k = a[i];
			a[i] = a[j];
			a[j] = k;
		}
	}

	a[left] = a[i];
	a[i] = flag;

	sort(left,i-1);
	sort(i+1,right);

}
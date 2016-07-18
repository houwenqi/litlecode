#include <stdio.h>

int a[101],n;


void quicksort(int left,int right)
{

	int i,j,k,flag;

	if(left > right)
		return;
	flag = a[left];

	i = left;
	j = right;

	while(i != j)
	{
		while(a[j]>=flag && i<j)
			j--;
		while(a[i]<=flag && i<j)
			i++;

		if( i<j )
		{
			k = a[i];
			a[i] = a[j];
			a[j] = k;
		}
	}

	a[left] = a[i];
	a[i] = flag;

	quicksort(left,i-1);
	quicksort(i+1,right);
}

int main()
{
	int i,j,t;
	scanf("%d",&n);

	for(i=1;i<=n;i++)
	{
		scanf("%d",&a[i]);
	}

	quicksort(1,n);

	for(i=1;i<=n;i++)
	{

		printf("%d,",a[i] );
	}
}
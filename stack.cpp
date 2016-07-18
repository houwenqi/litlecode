#include <stdio.h>
#include <string.h>

int main()
{
	char a[101];
	char s[101];

	int i;
	int len;
	int next;
	int top;
	int mid;

	scanf( "%s", &a );
	len = strlen( a );
	mid = len/2-1;
	top = 0;
	printf("mid is %d\n", mid );
	for( i = 0; i<= mid; i++)
	{
		s[ ++top ] = a[i];
	}

	if( len%2 == 0 )
	{
		next = mid + 1;
	}
	else
	{
		next = mid + 2;
	}
	printf("next is %d\n", next );
	for( i = next; i <= len; i++ )
	{
		if( a[i] != s[top] )
		{
			break;
		}
		top--;
	}

	if( top == 0 )
	{
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
	}

	return 0;
}
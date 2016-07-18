#include <stdio.h>

typedef struct queue
{
	int data[101];
	int head;
	int tail;
}T_queue;


int main()
{
	T_queue q;
	int i;

	q.head = 0;
	q.tail = 0;
	for ( i = 0; i <= 8; i++)
	{
		scanf( "%d",&q.data[q.tail] );
		q.tail++;
	}

	while ( q.head < q.tail )
	{
		printf( "%d,",q.data[q.head] );
		q.head++;


		q.data[q.tail] = q.data[q.head];
		q.tail++;

		q.head++;
	}

	return 0;
}
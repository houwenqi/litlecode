#include <stdio.h>

typedef struct queue
{
	int data[1000];
	int head;
	int tail;
}T_queue;

typedef struct stack
{
	int data[10];
	int top;
}T_stack;

	T_queue q1;
	T_queue q2;
	T_stack s;
	int book[10];
	int i;
	int t;

void init()
{
	//初始化
	q1.head = 1;
	q1.tail = 1;
	q2.head = 1;
	q2.tail = 1;

	s.top = 0;

	for ( i = 1; i <= 9; i++ )
	{
		book[i] = 0;
	}

	//输入手牌
	for ( i = 1; i <= 6; i++ )
	{
		scanf("%d",&q1.data[q1.tail]);
		q1.tail++;
	}

	for ( i = 1; i <= 6; i++ )
	{
		scanf("%d",&q2.data[q2.tail]);
		q2.tail++;
	}
}

void play()
{
	//A出牌
	t = q1.data[q1.head];  

	//判断赢牌
	if( book[t] == 0 )  //没赢
	{
		q1.head++;
		s.top++;
		s.data[s.top] = t;
		book[t] = 1;
	}
	else  //赢牌
	{
		q1.head++;
		q1.data[q1.tail] = t;
		q1.tail++;
		//收赢了的牌
		while ( s.data[s.top] != t )
		{
			book[s.data[s.top]] = 0;
			q1.data[q1.tail] = s.data[s.top];
			q1.tail++;
			s.top--;
		}
	}

	//B出牌
	t = q2.data[q2.head];

	if( book[t] == 0 )
	{
		q2.head++;
		s.top++;
		s.data[s.top] = t;
		book[t] = 1;
	}
	else
	{
		q2.head++;
		q2.data[q2.tail] = t;
		q2.tail++;
		while ( s.data[s.top] != t )
		{
			book[s.data[s.top]] = 0;
			q2.data[q2.tail] = s.data[s.top];
			q2.tail++;
			s.top--;
		}
	}
}

void judge()
{
	while ( q1.head < q1.tail && q2.head < q2.tail )
	{
		//判断输赢
		if ( q2.head == q2.tail )
		{
			printf("A is win\n");
			printf("A 手牌有：");
			for( i = q1.head; i < q1.tail-1; i++ )
			{
				printf( "%d",q1.data[i] );
			}

			if ( s.top > 0 )
			{
				printf("\n桌上牌有：");
				for ( i = 1; i <= s.top; i++ )
				{
					printf("%d\n",s.data[i] );
				}
			}
			else
			{
				printf("\n桌上没有牌了");
			}
		}
		else
		{
			printf("B is win\n");
			printf("B 手牌有：\n");
			for ( i = q2.head; i <= q2.tail-1; i++ )
			{
				printf("%d",q2.data[i] );
			}

			if ( s.top > 0 )
			{
				printf("\n桌上牌有：");
				for ( i = 1; i <= s.top; i++ )
				{
					printf("%d",s.data[i] );
				}
			}
			else
			{
				printf("\n桌上没有牌了");
			}
		}
	}
}

int main()
{
	init();

	//开始游戏
	
	play();

	judge();

	return 0;
}
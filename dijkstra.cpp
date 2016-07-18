#include <stdio.h>

int e[10][10];
int dis[10];
int book[10];
int max = 1001;

int main()
{
	int poit;
	int line;
	int i;
	int j;
	int p1;
	int p2;
	int len;
	int u;
	int v;
	int min;

	scanf("%d %d",&poit,&line);

	//初始化
	for ( i = 1; i <= poit; i++ )
	{
		for ( j = 1; j <= poit; j++ )
		{
			if ( i == j )
			{
				e[i][j] = 0;
			}
			else
			{
				e[i][j] = max;
			}
		}
	}

	//输入边
	for ( i = 1; i <= line; i++ )
	{
		scanf( "%d %d %d",&p1,&p2,&len );
		e[p1][p2] = len;
	}

	//初始化dis数组
	for ( i = 1; i <= poit; i++ )
	{
		dis[i] = e[1][i];
	}

	//初始化book数组
	for ( i = 1; i <= poit; i++ )
	{
		book[i] = 0;
	}
	book[1] = 1;

	//Dijkstra算法核心
	for ( i = 1; i <= poit-1; i++ )
	{
		//离1号最近的点
		min = max;
		for ( j = 1; j <= poit; j++ )
		{
			if( book[j] == 0 && dis[j] < min )
			{
				min = dis[j];
				u = j;
			}
		}
		book[u] = 1;
		for ( v = 1; v <= poit; v++ )
		{
			if( e[u][v] < max )
			{
				if( dis[v] > dis[u] + e[u][v] )
				{
					dis[v] = dis[u] + e[u][v];
				}
			}
		}
	}

	//输出结果
	for ( i =1; i <= poit; i++ )
	{
		printf("%d",dis[i] );
	}

	return 0;
}
#include <stdio.h>

int max = 1001;
int e[10][10];

int main()
{
	int point;
	int line;
	int i;
	int j;
	int p1;
	int p2;
	int len;
	int k;

	//输入顶点数和边条数
	scanf("%d %d",&point,&line);

	//初始化矩阵
	for ( i = 1; i <= point ; i++ )
	{
		for( j = 1; j <= point; j++ )
		{
			if( i == j )
			{
				e[i][j] = 0;
			}
			else
			{
				e[i][j] = max;
			}
		}
	}

	//输入实际边长
	for ( i = 1; i <= line; i++ )
	{
		scanf( "%d %d %d",&p1,&p2,&len );
		e[p1][p2] = len;
	}

	//floyd_warshall算法核心代码
	for ( k = 1; k <= point; k++ )
	{
		for ( i = 1; i <= point; i++ )
		{
			for ( j = 1; j <= point; j++ )
			{
				if( e[i][j] > e[i][k] + e[k][j] )
				{
					e[i][j] = e[i][k] + e[k][j];
				}
			}
		}
	}

	//输出结果
	for ( i = 1; i <= point; i++ )
	{
		for ( j = 1; j <= point; j++ )
		{
			printf("%10d", e[i][j]);
		}
		printf("\n");
	}


	return 0;
}
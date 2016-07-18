#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>  //sleep函数
#include "mysql.h"

//数据结构定义
MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;

using namespace std;

void DoSQL(char* sql)
{
	int retcode = 0;
	retcode = mysql_real_query(&mysql,sql,(unsigned int)strlen(sql));//如果查询成功，函数返回零。如果发生一个错误，函数返回非零。
	if( retcode != 0 )
	{
		printf("mysql_real_query is failed:%s\n",mysql_error(&mysql));
	}
}

int CMYSQL()
{
	//需要执行的语句
	char* query = "select usernickname from vec_pt_groupNickName where gid = 1 and uin = 110";

	//执行查询语句
	DoSQL(query);
	printf("DoSQL is ok\n");
	//处理查询结果
	res = mysql_store_result(&mysql);//如果成功，返回具有多个结果的MYSQL_RES结果集合。如果出现错误，返回NULL
	/*
	while( row = mysql_fetch_row(res) )
	{
		for(int i=0;i<mysql_num_fields(res);i++)
		{
			printf("%s\n",row[i] );
		}
		printf("\n");
	}*/
	string result = mysql_fetch_row(0);
	//strcpy(result,mysql_fetch_row(0));
	printf("result is %s\n",result);
	mysql_free_result(res);
	return 0;
}

int main()
{
	//初始化
	mysql_init(&mysql);

	//连接数据库
	if( !mysql_real_connect(&mysql,"172.16.1.23","Cserver","wkl1409624367","vec_ptt",0,NULL,0))//如果连接成功，返回MYSQL*连接句柄。如果连接失败，返回NULL。
	{
		printf("mysql_real_connect is failed:%s\n",mysql_error(&mysql));
		return -3;
	}
	printf("connect is ok\n");

	int ret = 0;
	ret = CMYSQL();
	if( ret != 0 )
	{
		printf("Do CMYSQL failed\n");
		return -1;
	}


	mysql_close(&mysql);
	return 0;
}
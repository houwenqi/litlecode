#include <stdio.h>


class Set
{
public:
	
	//构造函数
	Set()
	{
		this->number = 0;
		memset(this->items,0,sizeof(items));
	}
	//析构函数
	~Set()
	{}
	//初始化函数
	int init(int items[],int number);
	//添加元素
	bool add_item(int item);
	//删除元素
	bool remove_item(int item);
	//求并集
	Set operator+(Set set2);
	//求交集
	Set operator*(Set set2);
	//显示集合元素
	int display();
	//判断集合元素
	int is_exist(int item);
private:
	//变量声明
	int items[100];
	int number;
};

int Set::is_exist(int item)
{
	for(int i=0;i<this->number;i++)
	{
		if(this->items[i] == item)
		{
			return i;
		}
	}
	return -1;
}

bool Set::add_item(int item)
{
	if(is_exist(item) >= 0 || this->number >= 100)
		return false;
	this->items[this->number] = item;
	this->number++;

	return true;
}

bool Set::remove_item(int item)
{
	int pos = is_exist(item);
	if(pos == -1)
		return false;
	for(int i=pos;i<this->number-1;i++)
	{
		this->items[i] = this->items[i+1];
	}
	this->number--;

	return true;
}

Set Set::operator*(Set set2)
{
	Set result;
	for(int i=0;i<this->number;i++)
	{
		if(set2.is_exist(this->items[i]) >= 0)
		{
			result.items[result.number] = this->items[i];
			result.number ++;
		}
	}

	return result;
}

Set Set::operator+(Set set2)
{
	Set result;
	for(int i=0;i<this->number;i++)
	{
		result.items[result.number] = this->items[i];
		result.number++;
	}

	for(int j=0;j<set2.number;j++)
	{
		if(result.is_exist(set2.items[j]) == -1)
		{
			result.items[result.number] = set2.items[j];
			result.number++;
		}
	}
	return result;
}
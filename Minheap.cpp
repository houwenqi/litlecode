#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

int heap[100];


//下滑操作
void siftdown(int start,int end)
{
	//将start号节点向下调整到end
	int i = start,j=2*i;
	int k = heap[i];
	while(j <= end)
	{
		//有右孩子并且右孩子比左孩子小，将J存右孩子
		if(j<end && heap[j]>heap[j+1])
			++j;
		//比j号节点小，不调整
		if(k <= heap[j])
			break;
		else
		{
			//向下调整
			heap[i] = heap[j];
			i = j;
			j = 2*j;
		}
		for(int i=1;i<=end;i++)
		{
			cout<<heap[i]<<"--";

		}
		cout<<endl;
	}
	heap[i] = k;
}


//生成小根堆
void createheap(int n)
{
	memset(heap,0,sizeof(heap));//初始化heap数组
	cout << "enter values:" ;

	//从下标1开始存
	for(int i=1;i<=n;i++)
	{
		cin >> heap[i];
	}
	int currentpos = n/2;//开始调整位置（最后一个节点双亲位置）
	while(currentpos > 0)
	{
		for(int i=1;i<=n;i++)
		{
			cout<<heap[i]<<"||";

		}
		cout<<endl;
		siftdown(currentpos,n);
		--currentpos;
	}

}

//前序遍历
void preorder(int i,int num)
{
	if(i<=num)
	{
		cout<<heap[i]<<" ";

		preorder(2*i,num);
		preorder(2*i+1,num);
	}
}

//向上调整，节点start调到根节点1为止
void siftup(int start)
{
	int j=start,i=j/2;
	heap[0] = heap[j];
	while(j>0)
	{
		if(heap[i]<=heap[0])
			break;
		else
		{
			//向上调工作
			heap[j] = heap[i];
			j=i;
			i=i/2;
		}
	}
	heap[j] = heap[0];
}

//插入操作
bool insert(int x,int &num)
{
	++num;
	heap[num]=x;
	siftup(num);

	return true;
}

//删除操作
bool removeMin(int& num)
{
	heap[1]=heap[num];

	--num;
	siftdown(1,num);
	return true;
}

int main()
{
	int n=0;
	cout << "Enter the sum";
	cin >> n;
	createheap(n);

	preorder(1,n);
	cout<<endl;

	int val = 0;
	cout<<"Enter value to insert:";
	cin >> val;
	insert(val,n);
	preorder(1,n);
	cout<<endl;

	removeMin(n);
	preorder(1,n);
	cout<<endl;
}
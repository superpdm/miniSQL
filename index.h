#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string.h>
#include <stdio.h>
#include "catalog.h"
#include "Handle.h"
using namespace std;

#ifndef _INDEX_H
#define _INDEX_H

class CBPlusTree;
class TKEY;
class CBPlusTreeNode;
typedef CBPlusTreeNode BPTN;
typedef CBPlusTreeNode* LPBPTN;
typedef TKEY* PKEY;
typedef  int TVAL;

extern Handle hdl;
extern catalog ctlg;
class CBPlusTreeException
{
public:
	enum Type
	{
		NoEnoughMemory=0
	};

	CBPlusTreeException(Type type)
	{
		m_type = type;
	}

private:
	Type m_type;
};



class CBPlusTreeNode
{
public:

	
public:
	CBPlusTree& Tree;
	int BlockNum;
	int Rank;
	//int Parent; //指向父节点
	//int NextLeaf;
	char* Buffer;
	//int Count;
	//vector<TKEY> Keys;
	//vector<int> Nodes;
	//bool IsLeaf; 
	//int nodeType;	//非叶为0，叶为1
	bool IsLeaf;	//是否叶子标识
	bool IsNewNode;	//是否是以前文件中不存在的node
	
	/*
	int myIndex.KeyLen;
	//0 for int,1 for float ,2 for char(n)
	int KeyType;
	int Degree;
	*/
public:
	//创建新节点，参数为false表示创建枝干节点，否则为叶子节点
	CBPlusTreeNode(bool IsNewNode,CBPlusTree& tree,int blocknum,bool newLeaf=false);
	
	~CBPlusTreeNode();

	//对buffer中key和value值的访问
	TKEY GetKeys(int i);
	TVAL GetValues(int i);
	TVAL GetNextLeaf();
	TVAL GetParent();
	TVAL GetNodeType();
	TVAL GetCount();
	bool GetIsLeaf();

	bool SetKeys(int i,TKEY key);
	bool SetValues(int i,TVAL val);
	bool SetNextLeaf(TVAL val);
	bool SetParent(TVAL val);
	bool SetNodeType(TVAL val);
	bool SetCount(TVAL val);
	bool SetIsLeaf(bool val);

	bool GetBuffer();
	bool WriteToBuffer();
	bool LoadData1();
	bool LoadData();
	bool UpdateData();
//private: 
	bool IsRoot()
	{
		if(GetParent()!=-1)return false;
		return true;
	}
	bool Search(TKEY key,int &index);
	LPBPTN Splite(TKEY &key);
	int Add(TKEY &key);	//在枝干节点中插入一个key,返回对应的索引值

	int Add(TKEY &key,TVAL &val); //在叶子节点插入一个key,返回对应的索引值
	
	bool RemoveAt(int index);

public:
	void Print();
	
};

class FindNodeParam
{
	public:
	LPBPTN pNode; //找到节点的指针
	int index; // 节点关键字序号
	bool flag; // 是否找到
	void Print()
	{
		cout<<"blocknum:"<<pNode->BlockNum<<" index:"<<index<<" flag:"<<flag<<endl;
	}
};

class CBPlusTree
{

public:
	index *myIndex;
	int Degree;

public:
	CBPlusTree(index*index1)
	{	
		myIndex=index1;

		Degree=2*myIndex->Rank+1;
	}
	~CBPlusTree()
	{

	}


	bool Add(TKEY key,TVAL val);
	bool Remove(TKEY key);

	void Clear();
	bool ContainKey(TKEY key); //是否包含某个key

	FindNodeParam GetVal(TKEY key,TVAL&val); // 查找指定key的值

	FindNodeParam GetNextKey(FindNodeParam fnp,TVAL&val);
	

	bool SetVal(TKEY key,TVAL & val); // 设定key对应的值 
	int  GetNewBlockNum()
	{
		return myIndex->MaxCount++;
	}
	LPBPTN GetNode(int BlockNum);

//private:
	void InitTree();//初始化树
	bool AdjustAfterAdd(int Node);
	bool AdjustAfterRemove(int Node);
	void RemoveNode(int Node);
	void Search(int Node,TKEY &key,FindNodeParam & fnp);//查找一直到叶子节点
	void SearchBranch(int Node,TKEY &key,FindNodeParam & fnp); //查找枝干

public:
	void Print();
	void PrintLeaf(); //按序打印叶子节点
	void PrintNode(int Node);
};

#endif
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
	//int Parent; //ָ�򸸽ڵ�
	//int NextLeaf;
	char* Buffer;
	//int Count;
	//vector<TKEY> Keys;
	//vector<int> Nodes;
	//bool IsLeaf; 
	//int nodeType;	//��ҶΪ0��ҶΪ1
	bool IsLeaf;	//�Ƿ�Ҷ�ӱ�ʶ
	bool IsNewNode;	//�Ƿ�����ǰ�ļ��в����ڵ�node
	
	/*
	int myIndex.KeyLen;
	//0 for int,1 for float ,2 for char(n)
	int KeyType;
	int Degree;
	*/
public:
	//�����½ڵ㣬����Ϊfalse��ʾ����֦�ɽڵ㣬����ΪҶ�ӽڵ�
	CBPlusTreeNode(bool IsNewNode,CBPlusTree& tree,int blocknum,bool newLeaf=false);
	
	~CBPlusTreeNode();

	//��buffer��key��valueֵ�ķ���
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
	int Add(TKEY &key);	//��֦�ɽڵ��в���һ��key,���ض�Ӧ������ֵ

	int Add(TKEY &key,TVAL &val); //��Ҷ�ӽڵ����һ��key,���ض�Ӧ������ֵ
	
	bool RemoveAt(int index);

public:
	void Print();
	
};

class FindNodeParam
{
	public:
	LPBPTN pNode; //�ҵ��ڵ��ָ��
	int index; // �ڵ�ؼ������
	bool flag; // �Ƿ��ҵ�
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
	bool ContainKey(TKEY key); //�Ƿ����ĳ��key

	FindNodeParam GetVal(TKEY key,TVAL&val); // ����ָ��key��ֵ

	FindNodeParam GetNextKey(FindNodeParam fnp,TVAL&val);
	

	bool SetVal(TKEY key,TVAL & val); // �趨key��Ӧ��ֵ 
	int  GetNewBlockNum()
	{
		return myIndex->MaxCount++;
	}
	LPBPTN GetNode(int BlockNum);

//private:
	void InitTree();//��ʼ����
	bool AdjustAfterAdd(int Node);
	bool AdjustAfterRemove(int Node);
	void RemoveNode(int Node);
	void Search(int Node,TKEY &key,FindNodeParam & fnp);//����һֱ��Ҷ�ӽڵ�
	void SearchBranch(int Node,TKEY &key,FindNodeParam & fnp); //����֦��

public:
	void Print();
	void PrintLeaf(); //�����ӡҶ�ӽڵ�
	void PrintNode(int Node);
};

#endif
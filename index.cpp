
#include "Index.h"

//����һ���ڴ����в����ڵ�node
CBPlusTreeNode::CBPlusTreeNode(bool IsNewNode,CBPlusTree& tree,int blocknum,bool newLeaf):Tree(tree)
{
	IsLeaf=newLeaf;
	Rank=(tree.Degree-1)/2;
	BlockNum=blocknum;
	GetBuffer();
	if(IsNewNode)
	{
	SetParent(-1);
	/*
	Nodes.clear();	
	Keys.clear();*/
	SetNodeType(newLeaf?1:0);	
	SetCount(0);
	
	}
	//KeyLen=keyLen;
	//KeyType=keyType;
	//Degree=(4*1024-12)/(4+keyLen);
}
CBPlusTreeNode::~CBPlusTreeNode()
{
	for(int i=0;i<GetCount();i++)
	{
//		delete[]Keys[i].key;
	}
}

bool CBPlusTreeNode::LoadData1()
{/*
	static char s[5];
	for(int i=0;i<Tree.Degree;i++)
	{
		
		sprintf(s,"ABC%d",i);
		TKEY k(Tree.KeyType,Tree.KeyLen);
		memcpy(k.key,s,Tree.KeyLen);
		Keys.push_back(k);

	}
	for(int i=0;i<Tree.Degree+1;i++)
	{
		Nodes.push_back(i);
	}
	*/
	return true;
}

bool CBPlusTreeNode::LoadData()
{/*m
	Nodes.clear();
	Keys.clear();

	ifstream ifs("ab.dat",ios::binary);
	char *buffer=new char[4*1024];
	ifs.read(buffer,4*1024);
	nodeType=*((int*)(&buffer[0]));
	int count=*((int*)(&buffer[4]));
	int lenR=4+Tree.KeyLen;
	int base=8;
	for(int i=0;i<count;i++)
	{
		TKEY k(Tree.KeyType,Tree.KeyLen);
		memcpy(k.key,&buffer[base+i*lenR+4],Tree.KeyLen);
		Keys.push_back(k);
	}
	for(int i=0;i<count+1;i++)
	{
		Nodes.push_back(*((int*)(&buffer[base+i*lenR])));
	}

	for(int i=0;i<count;i++)
	{
		cout<<Nodes[i]<<"           ";

		switch(Tree.KeyType)
		{
		case  0:
			cout<<*(int*)(Keys[i].key);
		case 1:
			cout<<*(float*)(Keys[i].key);
		case 2:
			cout<<Keys[i].key;
		}
		cout<<endl;

	}
	delete[]buffer;
	*/
	return true;
}

bool CBPlusTreeNode::UpdateData()
{/*
		char *buffer=new char[4*1024];
		//*((int*)(&buffer[0]))=1024;
		int lenR=4+Tree.KeyLen;
		int base=8;
		*((int*)(&buffer[0]))=nodeType;
		*((int*)(&buffer[0+4]))=Keys.size();
		for(int i=0;i<Keys.size()+1;i++)
		{
			if(i<Keys.size())
				memcpy(&buffer[base+i*lenR+4],Keys[i].key,Tree.KeyLen);
			*((int*)(&buffer[base+i*lenR]))=Nodes[i];
			//*((float*)(&buffer[4+i*8+4]))=i*1.2;
		}
		
		
		//cout<<*((int*)(&buffer[0]))<<endl;
		ofstream ofs("ab.dat",ios::binary);
		ofs.write(buffer,4*1024);
		ofs.close();
		delete[]buffer;*/
		return true;
}


bool CBPlusTreeNode::GetIsLeaf()
{
	return GetNodeType()==1;
}
TKEY CBPlusTreeNode::GetKeys(int index)
{
	TKEY k(Tree.myIndex->KeyType,Tree.myIndex->KeyLen);
	int base=12;
	int lenR=4+Tree.myIndex->KeyLen;
	memcpy(k.key,&Buffer[base+index*lenR+4],Tree.myIndex->KeyLen);
	return k;
}
TVAL CBPlusTreeNode::GetValues(int index)
{
	TVAL val;
	int base =12;
	int lenR=4+Tree.myIndex->KeyLen;
	val=*((TVAL*)(&Buffer[base+index*lenR]));
	return val;
}
TVAL CBPlusTreeNode::GetNextLeaf()
{
	TVAL val;
	int base =12;
	int lenR=4+Tree.myIndex->KeyLen;
	val=*((TVAL*)(&Buffer[base+Tree.Degree*lenR]));
	return val;
}
TVAL CBPlusTreeNode::GetParent()
{
	TVAL val;
	val=*((int*)(&Buffer[8]));
	return val;
}
TVAL CBPlusTreeNode::GetNodeType()
{
	TVAL val;
	val=*((int*)(&Buffer[0]));
	return val;
}
TVAL CBPlusTreeNode::GetCount()
{
	TVAL val;
	val=*((int*)(&Buffer[4]));
	return val;
}
bool CBPlusTreeNode::SetKeys(int index,TKEY key)
{

	int base=12;
	int lenR=4+Tree.myIndex->KeyLen;
	memcpy(&Buffer[base+index*lenR+4],key.key,Tree.myIndex->KeyLen);
	return true;
}
bool CBPlusTreeNode::SetValues(int index,TVAL val)
{

	int base =12;
	int lenR=4+Tree.myIndex->KeyLen;
	*((TVAL*)(&Buffer[base+index*lenR]))=val;
	return true;
}
bool CBPlusTreeNode::SetNextLeaf(TVAL val)
{
	int base =12;
	int lenR=4+Tree.myIndex->KeyLen;
	*((TVAL*)(&Buffer[base+Tree.Degree*lenR]))=val;
	return true;
}
bool CBPlusTreeNode::SetParent(TVAL val)
{
	*((int*)(&Buffer[8]))=val;
	return true;
}
bool CBPlusTreeNode::SetNodeType(TVAL val)
{
	*((int*)(&Buffer[0]))=val;
	return true;
}
bool CBPlusTreeNode::SetCount(TVAL val)
{
	*((int*)(&Buffer[4]))=val;
	return true;
}
bool CBPlusTreeNode::SetIsLeaf(bool val)
{
	SetNodeType(val?1:0);
	return true;
}

bool CBPlusTreeNode::GetBuffer()
{
	blockInfo *block=hdl.get_file_block(ctlg.dbs[ctlg.curDB].name,Tree.myIndex->name,FILE_INDEX,BlockNum);
	Buffer=block->cBlock;
	block->dirtyBit=true;
	return true;
}



bool CBPlusTreeNode::WriteToBuffer()
{
	/*
		char *buffer=new char[4*1024];
		//*((int*)(&buffer[0]))=1024;
		int lenR=4+Tree.KeyLen;		//��¼�ĳ���
		
		nodeType=IsLeaf?0:1;
		
		int base=0;	
		*((int*)(&buffer[base]))=nodeType;	//��bufferд������
		base+=4;
		*((int*)(&buffer[base]))=Keys.size();//��bufferд���¼��
		base+=4;
		*((int*)(&buffer[base]))=Parent;
		base+=4;
		
		for(int i=0;i<Keys.size()+1;i++)
		{
			if(i<Keys.size())
				memcpy(&buffer[base+i*lenR+4],Keys[i].key,Tree.KeyLen);
			*((int*)(&buffer[base+i*lenR]))=Nodes[i];
			//*((float*)(&buffer[4+i*8+4]))=i*1.2;
		}

		*((int*)(&buffer[base+(Keys.size()+1)*lenR]))=NextLeaf;

		
		//cout<<*((int*)(&buffer[0]))<<endl;
		ofstream ofs(Tree.name.c_str(),ios::binary|ios::_Nocreate);

		int offset=4*1024*BlockNum;		//block������ļ�ͷ��ƫ��
		ofs.seekp(offset);
		ofs.write(buffer,4*1024);
		ofs.close();
		delete[]buffer;
		*/
		return true;
}

bool CBPlusTreeNode::Search(TKEY key,int &index)//����һ��key�����ض�Ӧ������ֵ
{
	bool ret =false;
	if(GetCount()==0) //����ǿռ�����0
	{
		index=0;
		return false;
	}

	// �߽�ֵ���� �󲿷ֵĽڵ����ֻ��ͨ���߽��ж�
	if(GetKeys(0) > key)
	{
		index =0;
		return false;
	}
	if(GetKeys(GetCount()-1) < key)
	{
		index = GetCount(); 
		return false;
	}


	if(GetCount() > 20) //�۰���� 2* log(n,2) < ˳����� (1+n)/2 ,��20�������۰���Ҹ���
	{
		int m,s,e;

		s=0;e=GetCount()-1;
		while(s<e)
		{
			m=(s+e)/2;

			if(key == GetKeys(m))
			{
				index = m;
				return true;    
			}
			else if(key <GetKeys(m))   
			{   
				e=m;
			}   
			else // key > Keys[m]
			{   
				s=m;
			}

			if(s==e-1)  //��s �� e �������ʱ
			{
				if(key == GetKeys(s))
				{
					index = s;
					return true;  
				}

				if(key == GetKeys(e))
				{
					index = e;
					return true;  
				}

				if(key < GetKeys(s))
				{   
					index=s;
					return false;
				}

				if(key <GetKeys(e))
				{   
					index=e;
					return false;
				}

				if(key > GetKeys(e))
				{   
					index=e+1;
					return false;
				}

			}
		};
		return false;
	}
	else //˳�����
	{

		for(int i=0;i<GetCount();i++)
		{
			if(key<GetKeys(i))
			{
				index = i;
				ret = false;
				break;
			}
			else if(key == GetKeys(i))
			{
				index = i;
				ret = true;
				break;
			}

		}
		return ret;
	}
}

LPBPTN CBPlusTreeNode::Splite(TKEY &key) //���ѽڵ�
{
	LPBPTN newNode = new BPTN(true,Tree,Tree.GetNewBlockNum(),GetIsLeaf());
	if(newNode == NULL)
	{
		throw CBPlusTreeException(CBPlusTreeException::Type::NoEnoughMemory);
		return NULL;
	}

	key = GetKeys(Rank); //�趨����ֵ

	if(GetIsLeaf()) // �����Ҷ�ӽڵ㣬��keyֵ����һ������ �ֳ�Rank+1 �� Rank
	{
		for(int i = Rank+1; i< Tree.Degree; i++) //������ֵ
		{
			newNode->SetKeys(i-Rank-1,GetKeys(i));
			//newNode->Keys[i]=TKEY(); //���Key����
			newNode->SetValues(i-Rank-1,GetValues(i));
			//Nodes[i]= NULL; //���ֵָ������
		}
		//������leaf֮ǰӦ��������count
		newNode->SetCount(Rank);
		SetCount(Rank+1);
		newNode->SetNextLeaf(GetNextLeaf());
		SetNextLeaf(newNode->BlockNum); 
		
		
		newNode->SetParent(GetParent());//����һ����
		
	}
	else //֦�ɽڵ� ÿ���ڵ��� Rank������һ���ڵ�
	{
		for(int i = Rank+1; i< Tree.Degree; i++) //������ֵ
		{
			newNode->SetKeys(i-Rank-1,GetKeys(i));
		}
		for(int i = Rank+1; i<= Tree.Degree;i++) //�����ӽڵ�ָ��
		{
			newNode->SetValues(i-Rank-1,GetValues(i));
		}
		newNode->SetParent(GetParent());
		newNode->SetCount(Rank);
		//�޸��½ڵ������ӽڵ㸸ָ���½ڵ㣬����ԭ�нڵ�
		int childNodeNum;
		for(int i = 0;i <= newNode->GetCount();i++)
		{
			childNodeNum = newNode->GetValues(i);
			LPBPTN node=Tree.GetNode(childNodeNum);
			if(node)
			{
				node->SetParent(newNode->BlockNum);

			}
		}


		//����νڵ�keyֵ���
// 		for(int i = Rank+1; i< Tree.Degree+1;i++) //����ӽڵ�ָ������
// 		{
// 			Nodes.pop_back();
// 		}

// 		for(int i = Rank; i< Tree.Degree;i++) //���Key����
// 		{
// 			Keys.pop_back();
// 		}
		SetCount(Rank);

	}	

	return newNode;
}

int CBPlusTreeNode::Add(TKEY &key) //��֦�ɲ���һ��key,���ض�Ӧ������ֵ
{
	int index=0;
	if(GetCount()==0) 
	{
		SetKeys(0,key);
		SetCount(1);
		return 0;
	}

	if(!Search(key,index)) //��ͬ��key������
	{

		for(int i = GetCount(); i > index;i--)
		{
			SetKeys(i,GetKeys(i-1));
		}

		for(int i = GetCount()+1; i > index;i--)
		{
			SetValues(i,GetValues(i-1));
		}

		SetKeys(index,key);
		SetValues(index,-1); //֦��������е���
		SetCount(GetCount()+1);
	}
	return index;
}


int CBPlusTreeNode::Add(TKEY &key,TVAL &val)
{
	int index=0;
	if(GetCount()==0) 
	{
		SetKeys(0,key);
		SetValues(0,val);
		SetCount(GetCount()+1);
		return 0;
	}

	if(!Search(key,index)) //��ͬ��key������
	{

		for(int i = GetCount(); i > index;i--)
		{
			SetKeys(i,GetKeys(i-1));
			SetValues(i,GetValues(i-1));
		}
		
		SetKeys(index,key);
		SetValues(index,val);
		SetCount(GetCount()+1);
	}
	return index;
}

void CBPlusTreeNode::Print()
{

	printf("----------------------\n");
	printf("BlockNum: %d Count: %d, Parent: %d  IsLeaf:%d\n",BlockNum,GetCount(),GetParent(),GetIsLeaf());
	printf("Keys: { ");
	for(int i =0; i<GetCount();i++)
	{
		cout<<GetKeys(i);
	}
	printf(" }\n");

	if(GetIsLeaf())
	{
		printf("Vals: { ");
		for(int i =0; i<GetCount();i++)
		{
			if(GetValues(i)==-1)
			{
				printf("{NUL}");
			}
			else
			{
				printf("%07d ",GetValues(i));
			}

		}
		printf(" }\n");

		printf("NextLeaf: %5d\n", GetNextLeaf());
	}
	else
	{
		printf("Ptrs: {");
		for(int i =0; i<=GetCount();i++)
		{
			printf("%07d ",GetValues(i));
		}
		printf("}\n");
	}
}

bool CBPlusTreeNode::RemoveAt(int index)
{
	if(index>GetCount()-1)
	{
		return false;
	}

	if(GetIsLeaf())
	{

		for(int i = index;i< GetCount()-1;i++)
		{
			SetKeys(i,GetKeys(i+1));
			SetValues(i,GetValues(i+1));
		}
		
	}
	else
	{
		for(int i = index;i< GetCount()-1;i++)
		{
			SetKeys(i,GetKeys(i+1));
		}

		for(int i = index;i< GetCount();i++)
		{
			SetValues(i,GetValues(i+1));
		}
		
	}
	SetCount(GetCount()-1);
	return true;
}
///////////////////////////////////////////////////////////////////CBPlusTree/////////////////////////////////////////////////////////

void CBPlusTree::InitTree()
{
	LPBPTN rootNode=new CBPlusTreeNode(true,*this,GetNewBlockNum(),true);
	myIndex->Root = 0;
	myIndex->LeafHead = myIndex->Root;
	myIndex->KeyCount =0;
	myIndex->NodeCount=1;
	myIndex->Level=1;
	rootNode->SetNextLeaf(-1);
	
}

//��ȡһ���Ѿ��ڴ����д��ڵĽڵ�
LPBPTN CBPlusTree::GetNode(int BlockNum)	
{
	LPBPTN PNode=new BPTN(false,*this,BlockNum);
	//PNode->LoadFromBuffer();
	PNode->GetBuffer();
	return PNode;
}

void CBPlusTree::Search(int Node,TKEY &key,FindNodeParam & fnp)
{
	int index=0;
	LPBPTN pNode=GetNode(Node);
	if(pNode->Search(key,index)) //�ҵ��˹ؼ���
	{
		if(pNode->GetIsLeaf())
		{
			fnp.flag = true; 
			fnp.index = index;
			fnp.pNode = pNode;
		}
		else
		{
			pNode = GetNode(pNode->GetValues(index)); //���ݷ��ѹ������õ�key�������ӽڵ�
			while(!pNode->GetIsLeaf()){ //�������Ҷ�Ӽ������²���
				//pNode->Nodes[pNode->Count]; //һ�����ӽڵ����һ��key������  
				pNode=GetNode(pNode->GetValues(pNode->GetCount()));
			}
			fnp.flag = true;
			fnp.index = pNode->GetCount()-1; //һ�����ҵ���Ҷ�ӽڵ����һ��
			fnp.pNode = pNode;
		}
	}
	else
	{
		if(pNode->GetIsLeaf()) //�����Ҷ�ӽڵ㣬�򲻼������²���
		{
			fnp.flag = false; 
			fnp.index = index;
			fnp.pNode = pNode;
		}
		else
		{
			Search((pNode->GetValues(index)),key,fnp);
		}
	}
}

bool CBPlusTree::Add(TKEY key,TVAL val)
{
	if(myIndex->Root==-1) //���û�г�ʼ�������³�ʼ��
	{
		InitTree();
	}

	FindNodeParam fnp;

	Search(myIndex->Root,key,fnp); 
	if(!fnp.flag) //��ͬkey,������
	{
		fnp.pNode->Add(key,val);//���ҵ���Ҷ�ӽڵ����key
		myIndex->KeyCount++;

		if(fnp.pNode->GetCount() == Degree ) //�����ֵ����Degree��������е���
		{

			return AdjustAfterAdd(fnp.pNode->BlockNum);
		}
		return true;
	}
	return false; //�����ֵ��ͬ
}
bool CBPlusTree::AdjustAfterAdd(int Node)
{
	LPBPTN pNode=GetNode(Node);
	TKEY key(this->myIndex->KeyType,this->myIndex->KeyLen);
	LPBPTN newNode = pNode->Splite(key); 
	myIndex->NodeCount++;
	int parent = pNode->GetParent();

	if(parent==-1) //���ڵ��ǿգ���ʾ��ǰ�ڵ��Ǹ��ڵ�
	{
		LPBPTN newRoot = new BPTN(true,*this,GetNewBlockNum()); //�½�һ���ӵ� 
		if(newRoot==NULL) return false;//����ڴ治������ʧ��

		myIndex->NodeCount++;
		myIndex->Root = newRoot->BlockNum; //LeafHead����Ķ�
		
		newRoot->Add(key);
		
		//�²����ĸ��ڵ�һ��ֻ������values
		newRoot->SetValues(0,pNode->BlockNum);
		newRoot->SetValues(1,newNode->BlockNum);

		pNode->SetParent(myIndex->Root);
		newNode->SetParent(myIndex->Root);
		newNode->SetNextLeaf(-1);
		myIndex->Level++; //�������1
		return true;
	}
	else
	{
		LPBPTN parentNode=GetNode(parent);
		int index = parentNode->Add(key); //ȡ�ø��ڵ��������ֵ�Ժ������

		parentNode->SetValues(index,pNode->BlockNum);
		parentNode->SetValues(index+1,newNode->BlockNum);

		if(parentNode->GetCount() == Degree) 
		{
			return AdjustAfterAdd(parentNode->BlockNum);
		}
		return true;
	}
}

void CBPlusTree::RemoveNode(TVAL Node)
{
	LPBPTN pNode=GetNode(Node);
	if(!pNode->GetIsLeaf()) //����ڵ㲻��Ҷ�ӽڵ�
	{
		for(int i=0;i <= pNode->GetCount();i++)
		{
			RemoveNode( pNode->GetValues(i));
			pNode->SetValues(i,-1);
		}
	}
	delete pNode;
	myIndex->NodeCount--;
	return;
}

void CBPlusTree::Clear()
{
	if(myIndex->Root)
	{
		RemoveNode(myIndex->Root);
		myIndex->KeyCount=0;
		myIndex->Level=0;
		myIndex->Root = NULL;
		myIndex->LeafHead = NULL;
	}
}

bool CBPlusTree::ContainKey(TKEY key)
{
	FindNodeParam fnp;
	Search(myIndex->Root,key,fnp);
	return fnp.flag;
}


FindNodeParam CBPlusTree::GetVal(TKEY key,TVAL&val)
{
	FindNodeParam fnp;
	Search(myIndex->Root,key,fnp);

	if(fnp.flag)
	{
		val = (fnp.pNode->GetValues(fnp.index));
		
	}
	return fnp;
}

FindNodeParam CBPlusTree::GetNextKey(FindNodeParam fnp,TVAL&val)
{
	if(fnp.index<fnp.pNode->GetCount()-1)
	{
		val=fnp.pNode->GetValues(fnp.index+1);
		fnp.index++;
		fnp.flag=true;
		return fnp;
	}
	else if(fnp.index==fnp.pNode->GetCount()-1)
	{
		TVAL numNode=fnp.pNode->GetNextLeaf();
		if(numNode<0)
		{
			fnp.flag=false;
			return fnp;
		}
		LPBPTN lpNode=GetNode(numNode);
		val=*(TVAL*)lpNode->GetBuffer();
		fnp.pNode=lpNode;
		fnp.index=0;
		fnp.flag=true;
		return fnp;
	}
	fnp.flag=false;
	return fnp;
}

bool CBPlusTree::SetVal(TKEY key,TVAL & val)
{
	FindNodeParam fnp;
	Search(myIndex->Root,key,fnp);
	if(fnp.flag)
	{
		fnp.pNode->SetValues(fnp.index,val);
		return true;
	}
	else return false;
}

void CBPlusTree::SearchBranch(int Node,TKEY &key,FindNodeParam & fnp)
{
	
	int index=0;
	LPBPTN pNode=GetNode(Node);
	//Ҷ�ӽڵ�ֱ�ӷ��ش���
	if(pNode->GetIsLeaf()) return; 
	if(pNode->Search(key,index)) 
	{//�ҵ��˹ؼ���
		fnp.flag = true; 
		fnp.index = index;
		fnp.pNode = pNode;
		return;
	}
	else //û�ҵ����²���
	{
		if(!GetNode(pNode->GetValues(index))->GetIsLeaf()) 
		{
			return SearchBranch(pNode->GetValues(index),key,fnp);
		}
		else //�Ѳ鵽Ҷ�ӽڵ����һ��
		{
			fnp.index =index;
			fnp.flag = false;
			fnp.pNode= pNode;
			return ;//δ�ҵ�
		}
	}
}


bool CBPlusTree::Remove(TKEY key)
{
	FindNodeParam fnp;

	
	if(myIndex->Root==-1 )
		return false;

	LPBPTN rootNode=GetNode(myIndex->Root);
	Search(myIndex->Root,key,fnp);

	if(fnp.flag) //�鵽key
	{
		//ֻ�����һ��Ҷ�ӽڵ���
		if(myIndex->Root == fnp.pNode->BlockNum)
		{
			rootNode->RemoveAt(fnp.index);
			myIndex->KeyCount--;
			return AdjustAfterRemove(fnp.pNode->BlockNum);
		}

		if( fnp.index == fnp.pNode->GetCount()-1) //���key��Ҷ�������һ������ô���϶���֦����
		{
			FindNodeParam fnpb; //֦�ɲ��Ҳ���

			SearchBranch(myIndex->Root,key,fnpb); //��֦���в���
			if(fnpb.flag)
			{
				//fnpb.pNode->Keys[fnpb.index] = fnp.pNode->Keys[fnp.pNode->Count-2];// ȡ�����ڶ���key
				fnpb.pNode->SetKeys(fnpb.index,fnp.pNode->GetKeys(fnp.pNode->GetCount()-2));
			}
		}

		fnp.pNode->RemoveAt(fnp.index); //�϶���Ҷ�ӽڵ㣬�����Ƴ�,
		myIndex->KeyCount--;
		
		return AdjustAfterRemove(fnp.pNode->BlockNum);
	}
	return false; //û�ҵ���ʧ��
}

//��ɾ���ڵ��Խڵ���е���
bool CBPlusTree::AdjustAfterRemove(TVAL Node)
{
	LPBPTN pNode=GetNode(Node);
	if(pNode->GetCount() >= myIndex->Rank)  //�����ɾ��key�Ľڵ���ؼ��ָ�����С��rank���账��
	{
		return true; 
	}

	//����������Ǹ��ڵ� 
	if( pNode->IsRoot() ) 
	{
		if(pNode->GetCount()==0) 
		{
			if(!pNode->GetIsLeaf())//��֦�ɽڵ�
			{
				
				myIndex->Root = pNode->GetValues(0); //���µ������ڵ�
				GetNode(pNode->GetValues(0))->SetParent(-1);//��ʱ����ڵ�ӦΪ���ڵ�
			}
			else
			{
				myIndex->Root = -1; //���һ���ڵ�Ҳû��
				myIndex->LeafHead = -1;
			}
			delete pNode;
			myIndex->NodeCount--;
			myIndex->Level--;
		}
		return true;
	}



	LPBPTN pBrother,pParent;
	int pos;

	pParent =GetNode(pNode->GetParent());
	pParent->Search(pNode->GetKeys(0),pos); //�ڸ��ڵ���ָ�����������

	if(pos == pParent->GetCount())  //������Count���ʱ����������value//���ֵ�
	{
		pBrother = GetNode(pParent->GetValues(pos-1)); 

		if(pBrother->GetCount() > myIndex->Rank)  //����Rank
		{
			// check
			if(pNode->GetIsLeaf()) //Ҷ�� 
			{
				//ȫ������
				for(int i = pNode->GetCount();i > 0 ; i--)   
				{
					pNode->SetKeys(i,pNode->GetKeys(i-1));
					pNode->SetValues(i,pNode->GetValues(i-1));
				}
				//���ֵܽڵ�����һ��ȡ���Լ���,�����·���
				pNode->SetKeys(0,pBrother->GetKeys(pBrother->GetCount()-1));
				pNode->SetValues(0,pBrother->GetValues(pBrother->GetCount()-1));

				pNode->SetCount(pNode->GetCount()+1);

				pBrother->SetCount(pBrother->GetCount()-1);

				pParent->SetKeys(pos-1, pBrother->GetKeys(pBrother->GetCount()-1));

				return true;
			}
			//check
			else  //��Ҷ��
			{
				//ȫ������
				for(int i = pNode->GetCount();i > 0 ; i--)   
				{
					pNode->SetKeys(i,pNode->GetKeys(i-1));
				}
				for(int i = pNode->GetCount()+1;i > 0 ; i--)   
				{
					pNode->SetValues(i,pNode->GetValues(i-1));
				}

				//��key����
				pNode->SetKeys(0,pParent->GetKeys(pos-1));
				pParent->SetKeys(pos-1,pBrother->GetKeys(pBrother->GetCount()-1));  //�ֵ����һ��key����
				//�������ڵ�
				pNode->SetValues(0,pBrother->GetValues(pBrother->GetCount())); //ָ���ֵܽڵ����һ����ָ��
				pNode->SetCount(pNode->GetCount()+1);
				//�����ֵܽڵ�
				//key����
			/*	pBrother->Keys[pBrother->Count-1]=TK(); */
				//�����ӽڵ�
				if(pBrother->GetValues(pBrother->GetCount())>=0)
				{
					//GetNode(pBrother->GetCount())->SetParent(pNode->BlockNum);
					GetNode(pBrother->GetValues(pBrother->GetCount()))->SetParent(pNode->BlockNum);
					pBrother->SetValues(pBrother->GetCount(),-1);
// 					pBrother->Nodes[pBrother->Count]->Parent = pNode; //�����丸ָ��
// 					pBrother->Nodes[pBrother->Count] = NULL; //����
				}
				pBrother->SetCount(pBrother->GetCount()-1);
				//pBrother->Count--; //�ֵܽڵ��һ
				return true; //���ڵ���δ�����ʲ��ٵ���
			}
		}
		else  //����ֵܽڵ�key������Rank
		{
			//check 2
			if(pNode->GetIsLeaf())
			{
				pParent->RemoveAt(pos-1); //���ڵ��Ƴ����һ��
				pParent->SetValues(pos-1,pBrother->BlockNum);//���������ӽڵ�ָ��

				//�����ڵ㵽�ֵ�
				for(int i = 0;i<pNode->GetCount();i++)
				{
					pBrother->SetKeys(pBrother->GetCount()+i,pNode->GetKeys(i));
					pBrother->SetValues(pBrother->GetCount()+i,pNode->GetValues(i));
					pNode->SetValues(i,-1);

				}

// 				pBrother->Count += pNode->Count; 
// 				pBrother->NextLeafNode = pNode->NextLeafNode;
				pBrother->SetCount(pBrother->GetCount()+pNode->GetCount());
				pBrother->SetNextLeaf(pNode->GetNextLeaf());
				delete  pNode;
				myIndex->NodeCount--;

				return AdjustAfterRemove(pParent->BlockNum); //���ϵ�����
			}
			//check
			else
			{
// 				pBrother->Keys[pBrother->Count]= pParent->Keys[pos-1]; //��key����
// 				pBrother->Count++; 
// 
// 				pParent->RemoveAt(pos-1);
// 				pParent->Nodes[pos-1] = pBrother; 
				pBrother->SetKeys(pBrother->GetCount(),pParent->GetKeys(pos-1));
				pBrother->SetCount(pBrother->GetCount()+1);
				pParent->RemoveAt(pos-1);
				pParent->SetValues(pos-1,pBrother->BlockNum);
				//�����ڵ�
				for(int i = 0;i<pNode->GetCount();i++)
 				{
// 					pBrother->Keys[pBrother->Count+i] = pNode->Keys[i];
					pBrother->SetKeys(pBrother->GetCount()+i,pNode->GetKeys(i));
				}

				for(int i = 0;i<= pNode->GetCount();i++)
				{
					/*pBrother->Nodes[pBrother->Count+i] = pNode->Nodes[i];*/
					//pNode->Nodes[i]->Parent = pBrother; 
					pBrother->SetValues(pBrother->GetCount()+i,pNode->GetValues(i));
					GetNode(pNode->GetValues(i))->SetParent(pBrother->BlockNum);
				}

				pBrother->SetCount( 2* myIndex->Rank); //һ�������ֵ

				delete pNode;
				myIndex->NodeCount--;

				return AdjustAfterRemove(pParent->BlockNum); //�������ϵ���

			}
		}

	} 

	else //���ֵ�
	{
		pBrother = GetNode(pParent->GetValues(pos+1)); 

		if(pBrother->GetCount() > myIndex->Rank)
		{
			//check
			if(pNode->GetIsLeaf()) //��ǰ�ڵ���Ҷ��
			{
// 				pParent->Keys[pos] = pBrother->Keys[0]; //���ֵܵĵ�һ��key�Ƶ�֦��
// 
// 				pNode->Keys[pNode->Count] = pBrother->Keys[0];    
// 				pNode->Vals[pNode->Count] = pBrother->Vals[0];    
// 				pBrother->Vals[0]=NULL;
// 				pNode->Count++;
				pParent->SetKeys(pos,pBrother->GetKeys(0));
				pNode->SetKeys(pNode->GetCount(),pBrother->GetKeys(0));
				pNode->SetValues(pNode->GetCount(),pBrother->GetValues(0));
				pBrother->SetValues(0,-1);
				pNode->SetCount(pNode->GetCount()+1);

				pBrother->RemoveAt(0);//�Ƴ���һ��

				return true;
			}
			//check
			else
			{
// 				pNode->Keys[pNode->Count] = pParent->Keys[pos];
// 				pNode->Nodes[pNode->Count+1] = pBrother->Nodes[0]; //�ֵܵ�һ��ָ��
// 				pNode->Count++;
// 
// 				pParent->Keys[pos]= pBrother->Keys[0];  //�ֵܵ�һ��key����
// 
// 				pBrother->Nodes[0]->Parent = pNode;

				pNode->SetKeys(pNode->GetCount(),pParent->GetKeys(pos));
				pNode->SetValues(pNode->GetCount()+1,pBrother->GetValues(0));
				pNode->SetCount(pNode->GetCount()+1);
				pParent->SetKeys(pos,pBrother->GetKeys(0));
				GetNode(pBrother->GetValues(0))->SetParent(pNode->BlockNum);


				pBrother->RemoveAt(0);//�Ƴ���һ��
				return true;
			}
		}
		else  //����ֵܽڵ�key������Rank
		{
			//check
			if(pNode->GetIsLeaf())
			{
				//�����ֵܽڵ�
				for(int i=0 ; i <myIndex->Rank;i++)
				{
// 					pNode->Keys[pNode->Count+i] = pBrother->Keys[i];
// 					pNode->Vals[pNode->Count+i] = pBrother->Vals[i];
// 					pBrother->Vals[i]=NULL; //����
					pNode->SetKeys(pNode->GetCount()+i,pBrother->GetKeys(i));
					pNode->SetValues(pNode->GetCount()+i,pBrother->GetValues(i));
					pBrother->SetValues(i,-1);
				}
//				pNode->Count += Rank;
				pNode->SetCount(pNode->GetCount()+myIndex->Rank);
				delete pBrother;
				myIndex->NodeCount--;

				pParent->RemoveAt(pos);
//				pParent->Nodes[pos] = pNode;
				pParent->SetValues(pos,pNode->BlockNum);
				return AdjustAfterRemove(pParent->BlockNum);
			}
			//check
			else
			{
//				pNode->Keys[pNode->Count]= pParent->Keys[pos]; //��key����
				pNode->SetKeys(pNode->GetCount(),pParent->GetKeys(pos));

				pParent->RemoveAt(pos);
//				pParent->Nodes[pos]= pNode;  //�������ڵ�ָ��
				pParent->SetValues(pos,pNode->BlockNum);
//				pNode->Count++; //Count+1
				pNode->SetCount(pNode->GetCount()+1);
				for(int i = 0;i<myIndex->Rank;i++)
				{
//					pNode->Keys[pNode->Count+i] = pBrother->Keys[i];
					pNode->SetKeys(pNode->GetCount()+i,pBrother->GetKeys(i));
				}

				for(int i = 0;i<=myIndex->Rank;i++)
				{
// 					pNode->Nodes[pNode->Count+i] = pBrother->Nodes[i];
// 					pBrother->Nodes[i]->Parent = pNode;
					pNode->SetValues(pNode->GetCount()+i,pBrother->GetValues(i));
					GetNode(pBrother->GetValues(i))->SetParent(pNode->BlockNum);
				}

//				pNode->Count += Rank;
				pNode->SetCount(pNode->GetCount()+myIndex->Rank);
				delete pBrother;
				myIndex->NodeCount--;

				return AdjustAfterRemove(pParent->BlockNum); //�������ϵ���
			}


		}
	}

}

//�����������

void CBPlusTree::Print()
{
	printf("*****************************************************\n");
	printf("KeyCount: %d, NodeCount: %d, Level: %d, Root: %d \n",myIndex->KeyCount,myIndex->NodeCount,myIndex->Level,myIndex->Root );

	if(myIndex->Root!=-1)
	{
		PrintNode(myIndex->Root);
	}


}


void CBPlusTree::PrintNode(int numNode)
{
	LPBPTN pNode=GetNode(numNode);

	pNode->Print(); 
	if(!pNode->GetIsLeaf()) //�������Ҷ�ӽڵ����
	{

		for(int i =0; i<= pNode->GetCount();i++)
		{
			PrintNode(pNode->GetValues(i));
		}
	}
}


void CBPlusTree::PrintLeaf()
{
	int p = myIndex->LeafHead;
	while(p!=-1)
	{
		LPBPTN pNode=GetNode(p);
		for(int i=0;i<pNode->GetCount();i++)
		{
			cout<<pNode->GetKeys(i);
		}
		printf("\n");
		for(int i=0;i<pNode->GetCount();i++)
		{
			printf("%5d ",pNode->GetValues(i));
		}
		printf("\n");
		p= pNode->GetNextLeaf();
	}

}
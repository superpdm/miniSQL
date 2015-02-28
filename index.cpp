
#include "Index.h"

//创建一个在磁盘中不存在的node
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
		int lenR=4+Tree.KeyLen;		//记录的长度
		
		nodeType=IsLeaf?0:1;
		
		int base=0;	
		*((int*)(&buffer[base]))=nodeType;	//向buffer写入类型
		base+=4;
		*((int*)(&buffer[base]))=Keys.size();//向buffer写入记录数
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

		int offset=4*1024*BlockNum;		//block相对于文件头的偏移
		ofs.seekp(offset);
		ofs.write(buffer,4*1024);
		ofs.close();
		delete[]buffer;
		*/
		return true;
}

bool CBPlusTreeNode::Search(TKEY key,int &index)//查找一个key，返回对应的索引值
{
	bool ret =false;
	if(GetCount()==0) //如果是空集返回0
	{
		index=0;
		return false;
	}

	// 边界值处理 大部分的节点查找只需通过边界判定
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


	if(GetCount() > 20) //折半查找 2* log(n,2) < 顺序查找 (1+n)/2 ,在20次以上折半查找更好
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

			if(s==e-1)  //当s 和 e 即将相会时
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
	else //顺序查找
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

LPBPTN CBPlusTreeNode::Splite(TKEY &key) //分裂节点
{
	LPBPTN newNode = new BPTN(true,Tree,Tree.GetNewBlockNum(),GetIsLeaf());
	if(newNode == NULL)
	{
		throw CBPlusTreeException(CBPlusTreeException::Type::NoEnoughMemory);
		return NULL;
	}

	key = GetKeys(Rank); //设定返回值

	if(GetIsLeaf()) // 如果是叶子节点，其key值复制一份上移 分成Rank+1 和 Rank
	{
		for(int i = Rank+1; i< Tree.Degree; i++) //拷贝键值
		{
			newNode->SetKeys(i-Rank-1,GetKeys(i));
			//newNode->Keys[i]=TKEY(); //后段Key置零
			newNode->SetValues(i-Rank-1,GetValues(i));
			//Nodes[i]= NULL; //后段值指针置零
		}
		//在设置leaf之前应该先设置count
		newNode->SetCount(Rank);
		SetCount(Rank+1);
		newNode->SetNextLeaf(GetNextLeaf());
		SetNextLeaf(newNode->BlockNum); 
		
		
		newNode->SetParent(GetParent());//共用一个父
		
	}
	else //枝干节点 每个节点变成 Rank，上移一个节点
	{
		for(int i = Rank+1; i< Tree.Degree; i++) //拷贝键值
		{
			newNode->SetKeys(i-Rank-1,GetKeys(i));
		}
		for(int i = Rank+1; i<= Tree.Degree;i++) //拷贝子节点指针
		{
			newNode->SetValues(i-Rank-1,GetValues(i));
		}
		newNode->SetParent(GetParent());
		newNode->SetCount(Rank);
		//修改新节点所有子节点父指向新节点，而非原有节点
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


		//将后段节点key值清空
// 		for(int i = Rank+1; i< Tree.Degree+1;i++) //后段子节点指针置零
// 		{
// 			Nodes.pop_back();
// 		}

// 		for(int i = Rank; i< Tree.Degree;i++) //后段Key置零
// 		{
// 			Keys.pop_back();
// 		}
		SetCount(Rank);

	}	

	return newNode;
}

int CBPlusTreeNode::Add(TKEY &key) //在枝干插入一个key,返回对应的索引值
{
	int index=0;
	if(GetCount()==0) 
	{
		SetKeys(0,key);
		SetCount(1);
		return 0;
	}

	if(!Search(key,index)) //相同的key不处理
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
		SetValues(index,-1); //枝干则需进行调整
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

	if(!Search(key,index)) //相同的key不处理
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

//获取一个已经在磁盘中存在的节点
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
	if(pNode->Search(key,index)) //找到了关键字
	{
		if(pNode->GetIsLeaf())
		{
			fnp.flag = true; 
			fnp.index = index;
			fnp.pNode = pNode;
		}
		else
		{
			pNode = GetNode(pNode->GetValues(index)); //根据分裂规则，上置的key属于左子节点
			while(!pNode->GetIsLeaf()){ //如果不是叶子继续往下查找
				//pNode->Nodes[pNode->Count]; //一定在子节点最后一个key的树里  
				pNode=GetNode(pNode->GetValues(pNode->GetCount()));
			}
			fnp.flag = true;
			fnp.index = pNode->GetCount()-1; //一定是找到的叶子节点最后一个
			fnp.pNode = pNode;
		}
	}
	else
	{
		if(pNode->GetIsLeaf()) //如果是叶子节点，则不继续往下查找
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
	if(myIndex->Root==-1) //如果没有初始化，重新初始化
	{
		InitTree();
	}

	FindNodeParam fnp;

	Search(myIndex->Root,key,fnp); 
	if(!fnp.flag) //相同key,不管它
	{
		fnp.pNode->Add(key,val);//在找到的叶子节点插入key
		myIndex->KeyCount++;

		if(fnp.pNode->GetCount() == Degree ) //插入后值等于Degree，必需进行调整
		{

			return AdjustAfterAdd(fnp.pNode->BlockNum);
		}
		return true;
	}
	return false; //如果键值相同
}
bool CBPlusTree::AdjustAfterAdd(int Node)
{
	LPBPTN pNode=GetNode(Node);
	TKEY key(this->myIndex->KeyType,this->myIndex->KeyLen);
	LPBPTN newNode = pNode->Splite(key); 
	myIndex->NodeCount++;
	int parent = pNode->GetParent();

	if(parent==-1) //父节点是空，表示当前节点是根节点
	{
		LPBPTN newRoot = new BPTN(true,*this,GetNewBlockNum()); //新建一个子点 
		if(newRoot==NULL) return false;//如果内存不够分配失败

		myIndex->NodeCount++;
		myIndex->Root = newRoot->BlockNum; //LeafHead不需改动
		
		newRoot->Add(key);
		
		//新产生的根节点一定只有两个values
		newRoot->SetValues(0,pNode->BlockNum);
		newRoot->SetValues(1,newNode->BlockNum);

		pNode->SetParent(myIndex->Root);
		newNode->SetParent(myIndex->Root);
		newNode->SetNextLeaf(-1);
		myIndex->Level++; //层次提升1
		return true;
	}
	else
	{
		LPBPTN parentNode=GetNode(parent);
		int index = parentNode->Add(key); //取得父节点插入提升值以后的索引

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
	if(!pNode->GetIsLeaf()) //如果节点不是叶子节点
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
	//叶子节点直接返回错误
	if(pNode->GetIsLeaf()) return; 
	if(pNode->Search(key,index)) 
	{//找到了关键字
		fnp.flag = true; 
		fnp.index = index;
		fnp.pNode = pNode;
		return;
	}
	else //没找到往下查找
	{
		if(!GetNode(pNode->GetValues(index))->GetIsLeaf()) 
		{
			return SearchBranch(pNode->GetValues(index),key,fnp);
		}
		else //已查到叶子节点的上一层
		{
			fnp.index =index;
			fnp.flag = false;
			fnp.pNode= pNode;
			return ;//未找到
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

	if(fnp.flag) //查到key
	{
		//只有最后一个叶子节点了
		if(myIndex->Root == fnp.pNode->BlockNum)
		{
			rootNode->RemoveAt(fnp.index);
			myIndex->KeyCount--;
			return AdjustAfterRemove(fnp.pNode->BlockNum);
		}

		if( fnp.index == fnp.pNode->GetCount()-1) //这个key在叶子是最后一个，那么它肯定在枝干中
		{
			FindNodeParam fnpb; //枝干查找参数

			SearchBranch(myIndex->Root,key,fnpb); //在枝干中查找
			if(fnpb.flag)
			{
				//fnpb.pNode->Keys[fnpb.index] = fnp.pNode->Keys[fnp.pNode->Count-2];// 取倒数第二个key
				fnpb.pNode->SetKeys(fnpb.index,fnp.pNode->GetKeys(fnp.pNode->GetCount()-2));
			}
		}

		fnp.pNode->RemoveAt(fnp.index); //肯定是叶子节点，进行移除,
		myIndex->KeyCount--;
		
		return AdjustAfterRemove(fnp.pNode->BlockNum);
	}
	return false; //没找到报失败
}

//在删除节点后对节点进行调整
bool CBPlusTree::AdjustAfterRemove(TVAL Node)
{
	LPBPTN pNode=GetNode(Node);
	if(pNode->GetCount() >= myIndex->Rank)  //如果被删除key的节点其关键字个数不小于rank不需处理
	{
		return true; 
	}

	//如果调整的是根节点 
	if( pNode->IsRoot() ) 
	{
		if(pNode->GetCount()==0) 
		{
			if(!pNode->GetIsLeaf())//是枝干节点
			{
				
				myIndex->Root = pNode->GetValues(0); //重新调整根节点
				GetNode(pNode->GetValues(0))->SetParent(-1);//此时这个节点应为根节点
			}
			else
			{
				myIndex->Root = -1; //最后一个节点也没了
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
	pParent->Search(pNode->GetKeys(0),pos); //在父节点中指向自身的索引

	if(pos == pParent->GetCount())  //返回与Count相等时代表是最右value//左兄弟
	{
		pBrother = GetNode(pParent->GetValues(pos-1)); 

		if(pBrother->GetCount() > myIndex->Rank)  //大于Rank
		{
			// check
			if(pNode->GetIsLeaf()) //叶子 
			{
				//全体右移
				for(int i = pNode->GetCount();i > 0 ; i--)   
				{
					pNode->SetKeys(i,pNode->GetKeys(i-1));
					pNode->SetValues(i,pNode->GetValues(i-1));
				}
				//把兄弟节点的最后一个取给自己用,即重新分配
				pNode->SetKeys(0,pBrother->GetKeys(pBrother->GetCount()-1));
				pNode->SetValues(0,pBrother->GetValues(pBrother->GetCount()-1));

				pNode->SetCount(pNode->GetCount()+1);

				pBrother->SetCount(pBrother->GetCount()-1);

				pParent->SetKeys(pos-1, pBrother->GetKeys(pBrother->GetCount()-1));

				return true;
			}
			//check
			else  //非叶子
			{
				//全体右移
				for(int i = pNode->GetCount();i > 0 ; i--)   
				{
					pNode->SetKeys(i,pNode->GetKeys(i-1));
				}
				for(int i = pNode->GetCount()+1;i > 0 ; i--)   
				{
					pNode->SetValues(i,pNode->GetValues(i-1));
				}

				//父key下移
				pNode->SetKeys(0,pParent->GetKeys(pos-1));
				pParent->SetKeys(pos-1,pBrother->GetKeys(pBrother->GetCount()-1));  //兄弟最后一个key上移
				//调整本节点
				pNode->SetValues(0,pBrother->GetValues(pBrother->GetCount())); //指向兄弟节点最后一个子指针
				pNode->SetCount(pNode->GetCount()+1);
				//调整兄弟节点
				//key清零
			/*	pBrother->Keys[pBrother->Count-1]=TK(); */
				//调整子节点
				if(pBrother->GetValues(pBrother->GetCount())>=0)
				{
					//GetNode(pBrother->GetCount())->SetParent(pNode->BlockNum);
					GetNode(pBrother->GetValues(pBrother->GetCount()))->SetParent(pNode->BlockNum);
					pBrother->SetValues(pBrother->GetCount(),-1);
// 					pBrother->Nodes[pBrother->Count]->Parent = pNode; //调整其父指针
// 					pBrother->Nodes[pBrother->Count] = NULL; //置零
				}
				pBrother->SetCount(pBrother->GetCount()-1);
				//pBrother->Count--; //兄弟节点减一
				return true; //父节点数未动，故不再调整
			}
		}
		else  //如果兄弟节点key数等于Rank
		{
			//check 2
			if(pNode->GetIsLeaf())
			{
				pParent->RemoveAt(pos-1); //父节点移除最后一个
				pParent->SetValues(pos-1,pBrother->BlockNum);//修正父的子节点指针

				//拷贝节点到兄弟
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

				return AdjustAfterRemove(pParent->BlockNum); //向上调整父
			}
			//check
			else
			{
// 				pBrother->Keys[pBrother->Count]= pParent->Keys[pos-1]; //父key下移
// 				pBrother->Count++; 
// 
// 				pParent->RemoveAt(pos-1);
// 				pParent->Nodes[pos-1] = pBrother; 
				pBrother->SetKeys(pBrother->GetCount(),pParent->GetKeys(pos-1));
				pBrother->SetCount(pBrother->GetCount()+1);
				pParent->RemoveAt(pos-1);
				pParent->SetValues(pos-1,pBrother->BlockNum);
				//拷贝节点
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

				pBrother->SetCount( 2* myIndex->Rank); //一定是这个值

				delete pNode;
				myIndex->NodeCount--;

				return AdjustAfterRemove(pParent->BlockNum); //继续向上调整

			}
		}

	} 

	else //右兄弟
	{
		pBrother = GetNode(pParent->GetValues(pos+1)); 

		if(pBrother->GetCount() > myIndex->Rank)
		{
			//check
			if(pNode->GetIsLeaf()) //当前节点是叶子
			{
// 				pParent->Keys[pos] = pBrother->Keys[0]; //将兄弟的第一个key移到枝干
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

				pBrother->RemoveAt(0);//移除第一个

				return true;
			}
			//check
			else
			{
// 				pNode->Keys[pNode->Count] = pParent->Keys[pos];
// 				pNode->Nodes[pNode->Count+1] = pBrother->Nodes[0]; //兄弟第一个指针
// 				pNode->Count++;
// 
// 				pParent->Keys[pos]= pBrother->Keys[0];  //兄弟第一个key上移
// 
// 				pBrother->Nodes[0]->Parent = pNode;

				pNode->SetKeys(pNode->GetCount(),pParent->GetKeys(pos));
				pNode->SetValues(pNode->GetCount()+1,pBrother->GetValues(0));
				pNode->SetCount(pNode->GetCount()+1);
				pParent->SetKeys(pos,pBrother->GetKeys(0));
				GetNode(pBrother->GetValues(0))->SetParent(pNode->BlockNum);


				pBrother->RemoveAt(0);//移除第一个
				return true;
			}
		}
		else  //如果兄弟节点key数等于Rank
		{
			//check
			if(pNode->GetIsLeaf())
			{
				//拷贝兄弟节点
				for(int i=0 ; i <myIndex->Rank;i++)
				{
// 					pNode->Keys[pNode->Count+i] = pBrother->Keys[i];
// 					pNode->Vals[pNode->Count+i] = pBrother->Vals[i];
// 					pBrother->Vals[i]=NULL; //置零
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
//				pNode->Keys[pNode->Count]= pParent->Keys[pos]; //父key下移
				pNode->SetKeys(pNode->GetCount(),pParent->GetKeys(pos));

				pParent->RemoveAt(pos);
//				pParent->Nodes[pos]= pNode;  //修正父节点指针
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

				return AdjustAfterRemove(pParent->BlockNum); //继续向上调整
			}


		}
	}

}

//调试输出函数

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
	if(!pNode->GetIsLeaf()) //如果不是叶子节点继续
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
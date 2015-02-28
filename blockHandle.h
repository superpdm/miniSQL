#ifndef BLOCKHANDLE_H
#define BLOCKHANDLE_H
#include "blockInfo.h"

class blockInfo;

class blockHandle{

public:
	blockInfo *BHandle;
	int bsize;//总共数目
	int countb;//可用数目

	blockHandle():BHandle(new blockInfo(0)),bsize(300),countb(0){add(BHandle);}//已修改
	
	//初始化使用
	blockInfo* add(blockInfo* BHandle){
		blockInfo* addr=BHandle;
		//for(int i = 1;i<=bsize;i++){
		blockInfo* adder=new blockInfo(0);
		adder->next=addr->next;
		addr->next=adder;
		countb++;
		if(countb==bsize)
			return adder;
		else
			return add(adder);
			
		//}
	}
	
	//利用block
	blockInfo* foruse(){
		if(BHandle->next!=NULL){
		blockInfo* use=BHandle->next;
		BHandle->next=BHandle->next->next;
		countb--;
		use->iTime=0;
		return use;
		}
		else{
			return NULL;
		}
	}
	
	//回收block
	void forfree(blockInfo* block){
		blockInfo* free=BHandle->next;
		block->next=free;
		BHandle->next=block;
		countb++;

	}


};


#endif
#ifndef BLOCKINFO_H
#define BLOCKINFO_H
#include<stdio.h>

class fileInfo;

class blockInfo{

public:

    int blockNum;	// the block number of the block,
                    // which indicate it when it be newed
	bool dirtyBit;     // 0 -> flase£¬ 1 -> indicate dirty, write back
	blockInfo *next;  	// the pointer point to next block 
	fileInfo *file;     	// the pointer point to the file, which the block belongs to
	char *cBlock;	   	// the array space for storing the records in the block in buffer
	int iTime;		// it indicate the age of the block in use 
	
	
	
	blockInfo(int i):dirtyBit(0),next(0),file(0),iTime(0)
	{
		blockNum = i;
		cBlock=new char[4*1024];
		
	}

};



#endif
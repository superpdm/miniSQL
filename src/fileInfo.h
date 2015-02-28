#ifndef FILEINFO_H
#define FILEINFO_H
#include<string>
using namespace std;

class blockInfo;

class fileInfo{

public:

	int type;				// 0-> data file£¬ 1 -> index file
	string fileName;		// the name of the file
	int recordAmount;		// the number of record in the file
	int recordLength;		// the length of the record in the file
	fileInfo *next;			// the pointer points to the next file
	blockInfo *firstBlock;	// point to the first block within the file
	
	fileInfo(int i,string file,int recA,int recL,fileInfo* nex,blockInfo* firB){
		type = i;
		fileName = file;
		recordAmount = recA;
		recordLength = recL;
		next = nex;
		firstBlock = firB;
	}



};



#endif
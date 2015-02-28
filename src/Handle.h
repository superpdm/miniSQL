#ifndef HANDLE_H
#define HANDLE_H

#include"blockHandle.h"
#include"fileHandle.h"
#include "constant.h"
#include<stdio.h>
#include<fstream>

class Handle{

public:
	blockHandle*  BlockH;
	fileHandle*   FileH;
	Handle():BlockH(new blockHandle()),FileH(new fileHandle()){}
	blockInfo*	findBlock(string DB_Name);

	blockInfo * get_file_block(string DB_Name,string Table_Name,int fileType,int blockNum);

	void closeDatabase(string DB_Name,bool m_flag);//use closefile to complete 

	void closeFile(string DB_Name,string m_fileName,int m_fileType,bool m_flag);

	void quitProg(string DB_Name);//use closefile and delete the rubbish block list

	void Read_Info(string DB_Name,string file_Name,int fileType,blockInfo* block);//read from file

	void Wirte_file_block(blockInfo* block);//set the dirty bit to 1
	
	void Used();//every itime add 1 

};


#endif
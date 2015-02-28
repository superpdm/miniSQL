#ifndef _RECORD_H
#define  _RECORD_H
#include<string>
#include<vector>
#include<iostream>
#include "SQLStatement.h"
#include "catalog.h"
#include "Handle.h"
#include "index.h"
class attribute;
class TKEY;

class Record
{
	
public:
		Handle* H;
		table*  tbl;	
		int Max_count;
		string DBName;
public:	
	Record(string dbName,table* tbl,Handle* H)
	{
		DBName=dbName;
		this->H = H;
		this->tbl = tbl;
		this->Max_count = this->get_max_record_count();
	}	
	int get_first_block_num()//get the first useful block number
	{
		return tbl->FirstBlockNum;
	}
	int get_first_Rubbish_num()//get the first rubbish block number
	{
		return tbl->FirstRubbishNum;
	}
	int get_block_count()//get the block count from the table
	{
		return tbl->BlockCount;
	}
	int get_record_count(int blockNum)//get the record count from a block
	{
		
		int i = *(int*)((this->get_block_content(blockNum ))+8);
		return i;
	}
	int get_max_record_count()//get the max number that a block can contain
	{
		int i = (4096-12)/(tbl->recordLen);
		return i;
	}
	void set_record_count(int blockNum,int count)   //set the record number of the block
	{

		*(int*)((this->get_block_content(blockNum ))+8)=count;//store the count into block content
	}
	int get_next_block_num(int BlockNum);

	
	
	std::string insert_into_block(vector<TKEY>keys,int&indexValue);	//把keys里面的插入到表中。其中indexValue(blockNum,offset各占16位)是返回的插入后值的位置
	
	std::string get_record(int BlockNum,int BlockOffset,vector<TKEY>&keys);	//get a record from block
	
	std::string get_record(int BlockNum,vector<vector<TKEY>>&keyss);//get all records from a block

	std::string delete_from_block(int BlockNum,int BlockOffset);


private:	
	char* get_block_content(int BlockNum)//get the content from the block
	{
		blockInfo* block = H->get_file_block(DBName,tbl->name,FILE_RECORD,BlockNum);
		return block->cBlock;
	}
	
	
	void WriteBack(string DB_Name,string Table_Name,int fileType,int blockNum)//for wirte back
	{
		blockInfo* block = H->get_file_block(DBName,tbl->name,fileType ,blockNum);
		H->Wirte_file_block(block);
	}
	/*int get_useable_block()	//当插入时如果可用的block都满了的时候，找到一个blockNum，然后插入到其中
	{return tbl->FirstRubbishNum;}*/
};

class exeSQL{
public:
	static void exeInsert(catalog& clog,SQLInsert &sqlInsert,Handle& hdl);//参数传递一个TKEY
	static void exeSelect(catalog& clog,SQLSelect &sqlSelect,Handle &hdl);
	static void exeDelete(catalog& clog,SQLDelete &sqlDelete,Handle &hdl);//返回要删除的记录的序号
	static CBPlusTree* exeCreateIndex(catalog& clog,SQLCreateIndex &sqlcreateindex,Handle &hdl);
};



#endif 
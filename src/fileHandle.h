#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include "fileInfo.h"
/*#include "constant.h"*/
#include<stdio.h>
#include<fstream>
class fileInfo;

class fileHandle{

public:
	fileInfo* FHandle;
	int fsize;//�ܹ���Ŀ
	int countf;//������Ŀ

	fileHandle():FHandle(new fileInfo(0,"",0,0,0,0)),fsize(5),countf(0){}
	
	//����file������block�ѳ�ʼ����������block�żӵ�
   
	void add(fileInfo* adder){
		
		
		if(countf<fsize)
		{//δ��,���ڵ�һ��
			fileInfo* addr=FHandle->next;
			adder->next=addr;
			FHandle->next=adder;
			countf++;
		}
		else
		{//���ˣ����ڵ�һ��
			
			fileInfo* addr=FHandle->next;
			adder->next=addr->next;
			FHandle->next=adder;
			delete addr;

	    }

	}

	//���block����û�пյĿ��ˣ��ʹ�file��ȡʱ�����
	
	blockInfo* forbuse(string DB_Name){
		fileInfo* fuse = FHandle;
		while(fuse->next->firstBlock==NULL){
			fuse=fuse->next;
		}
		blockInfo* buse1 = fuse->next->firstBlock;
		
		while(fuse->next!=NULL){
			blockInfo* buse2 = fuse->next->firstBlock;
			
			while(buse2!=NULL){
				if(buse1->iTime<buse2->iTime)
				{
					buse1=buse2;
				}
				
				buse2=buse2->next;
				
			}
			fuse=fuse->next;
		}
		
		fileInfo* f=this->findF(buse1->file->fileName,buse1->file->type);
		blockInfo* b=f->firstBlock;
		if(b->blockNum==buse1->blockNum){
			this->Write_Info(DB_Name,buse1->file->fileName,buse1->file->type,buse1);
			f->firstBlock=b->next;
		}

		while(b->next!=NULL){
			if(b->next->blockNum==buse1->blockNum){
				this->Write_Info(DB_Name,buse1->file->fileName,buse1->file->type,buse1);
				b->next=b->next->next;
				break;
			}
				b=b->next;
		}
		buse1->iTime=0;
		return buse1;
	}
	
	//����block���ѳ�ʼ������Ҫô������Ҫô��file��
	
	void addB(blockInfo* block){
		int i=0;
	    fileInfo* fuse = FHandle;
		while(fuse->next->fileName!=block->file->fileName){
		     if(fuse->next==NULL){
				 i=1;
				 break;
			 }
			 fuse=fuse->next;

		}
		if(i==0){
			blockInfo* buse = fuse->next->firstBlock;
			block->iTime=0;
			fuse->next->firstBlock=block;
			block->next=buse;
			fuse->next->recordAmount++;
			fuse->next->recordLength+=4096;
			block->file=fuse->next;
		}
		
	}

	//����file

	fileInfo* findF(string Table_Name,int fileType){

		fileInfo* fuse = FHandle;
		while(fuse->next!=NULL){
			if(fuse->next->type==fileType&&fuse->next->fileName==Table_Name){
				return fuse->next;
			
			}
			fuse = fuse->next;
		}
		if(fuse->next==NULL){
			return NULL;
		}
	}

	//����block

	blockInfo* findB(int blockNum,fileInfo* file){

		int i=0;
		blockInfo* b=file->firstBlock;
		if(b==NULL)
		{
			return NULL;
		}
		else{

			while(b->blockNum!=blockNum){
				if(b->next==NULL){
			
					i=1;
					break;
				}
				b=b->next;
			}
			if(i==0){
		
				return b;
			}
			else{
		
				return NULL;
			}
		}


	}

	//wirte back to the file
	void Write_Info(string DB_Name,string file_Name,int fileType,blockInfo* block){
	
		//string name=file_Name+".record";
		string name = "DBs\\"+DB_Name+"\\"+file_Name;
		if (fileType==1)
		{
			name+=".index";
		}
		else
			name+=".record";
		ofstream ofs(name,ios::binary|ios::_Nocreate);
		ofs.seekp(block->blockNum*4*1024);
		ofs.write(block->cBlock,4*1024);
		ofs.close();
	
	}

};


#endif
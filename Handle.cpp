#include"Handle.h"
#include "constant.h"
void Handle::Used()
{
	fileInfo* fuse = this->FileH->FHandle;
	if(fuse->next!=NULL){
		while(fuse->next->firstBlock==NULL){

			fuse=fuse->next;

			if(fuse==NULL)
				break;
		}
		
		while(fuse!=NULL){
			blockInfo* buse2 = fuse->firstBlock;
			
			while(buse2!=NULL){
				
				buse2->iTime++;
				buse2=buse2->next;
				
			}
			fuse=fuse->next;
		}
	}
}

blockInfo*	Handle::findBlock(string DB_Name){
	if(this->BlockH->countb>0){
	
		return BlockH->foruse();
	}
	else{
		
		return FileH->forbuse(DB_Name);
	}
}

blockInfo * Handle::get_file_block(string DB_Name,string Table_Name,int fileType,int blockNum){
	
	
	Used();
	fileInfo* file=FileH->findF(Table_Name,fileType);
	
	if(file){
		blockInfo* block=FileH->findB(blockNum,file);
		if(block){
			block->iTime=0;
			//block->dirtyBit=1;
			return block;
		}
		else{
			blockInfo* b=this->findBlock(DB_Name);
			b->blockNum=blockNum;
			fileInfo* f=new fileInfo(fileType,Table_Name,1,4096,NULL,NULL);
			b->file=f;
			this->Read_Info(DB_Name,Table_Name,fileType,b);
			this->FileH->addB(b);
			return b;
		}
	}

	else{
		blockInfo* b=this->findBlock(DB_Name);
		b->blockNum=blockNum;

		fileInfo* f=new fileInfo(fileType,Table_Name,1,4096,NULL,NULL);
		FileH->add(f);
		b->file=f;
		this->Read_Info(DB_Name,Table_Name,fileType,b);
		this->FileH->addB(b);
		return b;
	}

}

void Handle::closeDatabase(string DB_Name,bool m_flag){

	while(FileH->FHandle->next!=NULL){
		closeFile(DB_Name,FileH->FHandle->next->fileName,FileH->FHandle->next->type,m_flag);
	}

}

void Handle::closeFile(string DB_Name,string m_fileName,int m_fileType,bool m_flag){
	fileInfo* fuse = FileH->FHandle;
	 
	 while(fuse->next!=NULL){
		 if(fuse->next->fileName==m_fileName&&fuse->next->type==m_fileType){
			 break;
		 }
		fuse=fuse->next;
	}
	 if(fuse->next!=NULL){
		 fileInfo* unuse=fuse->next;
		
		 blockInfo* buse= unuse->firstBlock;
		 fuse->next=unuse->next;
		 delete unuse;
		 FileH->countf--;
		 
		 
			
		 while(buse!=NULL){
			 
				 if(buse->dirtyBit==1){
					 this->FileH->Write_Info(DB_Name,m_fileName,m_fileType,buse);
				 }
				 blockInfo* b=buse;
				 buse=buse->next;
				 BlockH->forfree(b);
			 
		 
		 }
	 }
}

void Handle::quitProg(string DB_Name){
	while(FileH->FHandle->next!=NULL){
		closeFile(DB_Name,FileH->FHandle->next->fileName,FileH->FHandle->next->type,true);
	}
	blockInfo* b=BlockH->BHandle;
	while(BlockH->countb!=0){
		//blockInfo* b=BlockH->BHandle->next;
		//BlockH->BHandle->next=BlockH->BHandle->next->next;
		//BlockH->countb--;
		//delete b;
		blockInfo* c=b->next;
		b->next=c->next;
		BlockH->countb--;
		delete c;

	}

}
	

void Handle::Read_Info(string DB_Name,string file_Name,int fileType,blockInfo* block){

		//string name=file_Name+".record";
	string name = "DBs\\"+DB_Name+"\\"+file_Name;
	if (fileType==FILE_INDEX)
	{
		name+=".index";
	}
	else
		name+=".record";
	ifstream ifs(name,ios::binary);
	ifs.seekg(block->blockNum*4*1024);
	ifs.read(block->cBlock,4*1024);
		ifs.close();


}

void Handle::Wirte_file_block(blockInfo* block){
	block->dirtyBit=1;
}
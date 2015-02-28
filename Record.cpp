
#include "constant.h"
#include "catalog.h"
#include "Record.h"
#include "Record.h"
#include "index.h"


#include<iostream>

using namespace std;

/////////////////////record////////////////////
int Record::get_next_block_num(int BlockNum)
{
	char* content =this->get_block_content(BlockNum );
	int num = *(int*)(content+4);
	return num;
}
std::string Record::insert_into_block(vector<TKEY>keys,int&indexValue)
{

	char* content ;
	int i =tbl->FirstBlockNum;
	int j =tbl->FirstRubbishNum;
	int set ;//use for record the last useful block

	while(i!=-1)//find the block for use from the useful block
	{
		set=i;
		content = this->get_block_content(i);
		if(get_record_count(i)==Max_count)
		{
			i = *(int*)(content+4);
			continue;
		}
		char* select = content+12+(this->get_record_count(i))*tbl->recordLen;
		indexValue = (i<<16)|((this->get_record_count(i)+1)&0xffff);
		vector<TKEY>::iterator iter = keys.begin();
		for(;iter!=keys.end();++iter)
		{
			memcpy(select,iter->key,iter->length);
			select=select+iter->length;
		}
		this->set_record_count(i,get_record_count(i)+1);
		this->WriteBack(DBName,tbl->name,FILE_RECORD ,i);
		return "";

	}


	if(j!=-1)//find the the first block from the rubbish 
	{
		content = this->get_block_content(j)+12;
		vector<TKEY>::iterator iter = keys.begin();
		for(;iter!=keys.end();++iter)
		{
			memcpy(content,iter->key,iter->length);
			content=content+iter->length;
		}
		this->set_record_count(j,1);
		indexValue = (j<<16)|(1&0xffff);


		*(int*)(this->get_block_content(set)+4)=tbl->FirstRubbishNum;

		tbl->FirstRubbishNum=*(int*)(this->get_block_content(j)+4);
		*(int*)(this->get_block_content(j))=set;

		*(int*)(this->get_block_content(j)+4)=-1;

		this->WriteBack(DBName,tbl->name,FILE_RECORD ,j);
		this->WriteBack(DBName,tbl->name,FILE_RECORD ,set);


		return "";

	}

	else//initial or add a block
	{

		int nextBlock=tbl->FirstBlockNum;
		if(tbl->FirstBlockNum!=-1)
		{
			char* upcontent = this->get_block_content(tbl->FirstBlockNum);
			*(int*)(upcontent) = tbl->BlockCount;
			this->WriteBack(DBName,tbl->name,FILE_RECORD ,tbl->FirstBlockNum);

		}

		tbl->FirstBlockNum=tbl->BlockCount;
		content = this->get_block_content(tbl->BlockCount);
		*(int*)(content) = -1;
		*(int*)(content+4) = nextBlock;

		content+=12;
		vector<TKEY>::iterator iter = keys.begin();
		for(;iter!=keys.end();++iter)
		{
			memcpy(content,iter->key,iter->length);
			content=content+iter->length;
		}
		this->set_record_count(tbl->BlockCount,1);
		indexValue = (tbl->BlockCount<<16)|(1&0xffff);
		this->WriteBack(DBName,tbl->name,FILE_RECORD ,tbl->BlockCount);
		++tbl->BlockCount;
		return "";
	}


}

std::string Record::get_record(int BlockNum,int BlockOffset,vector<TKEY>& keys)//get a record from block
{
	char* content = this->get_block_content(BlockNum );

	for(int i=0;i<tbl->attrNum;i++)
	{
		TKEY key(tbl->attrs[i].dataType,tbl->attrs[i].len);
		keys.push_back(key);
	}
	char* select = content+BlockOffset*tbl->recordLen+12;
	vector<TKEY>::iterator iter = keys.begin();

	for(;iter!=keys.end();++iter)

	{
		memcpy(iter->key,select,iter->length);

		select=select+iter->length;

	}
	return "";

}

std::string Record::get_record(int BlockNum,vector<vector<TKEY>>& keyss)//get all records from a block
{
	char* content = this->get_block_content(BlockNum );
	char* select = content+12;
	for(int i=0;i<(this->get_record_count(BlockNum));i++)
	{
		vector<TKEY> keys;

		for(int j=0;j<tbl->attrNum;j++)
		{
			TKEY key(tbl->attrs[j].dataType,tbl->attrs[j].len);
			keys.push_back(key);
		}
		vector<TKEY>::iterator iter = keys.begin();

		for(;iter!=keys.end();++iter)

		{
			memcpy(iter->key,select,iter->length);
			select=select+iter->length;

		}

		keyss.push_back(keys);
	}


	return "";
}

std::string Record::delete_from_block(int BlockNum,int BlockOffset)//delete a record from block 
{
	char* content = this->get_block_content(BlockNum); 
	char* select = content+BlockOffset*tbl->recordLen+12;
	int i =BlockOffset*tbl->recordLen+12;
	int j = this->get_record_count(BlockNum);
	char* k = content+12+(j-1)*(tbl->recordLen);

	memcpy(select,k,tbl->recordLen);

	set_record_count(BlockNum,get_record_count(BlockNum)-1);



	if(this->get_record_count(BlockNum)==0)//if null record in block ,add it to the rubbish
	{
		int upnum = *(int*)content;
		int downnum = *(int*)(content+4);
		char* contentup = this->get_block_content(upnum); 
		char* contentdown = this->get_block_content(downnum); 
		*(int*)(contentup+4)= downnum;
		*(int*)contentdown = upnum;

		int tmpRB=tbl->FirstRubbishNum;
		tbl->FirstRubbishNum=BlockNum;
		char* content1 = this->get_block_content(BlockNum );
		*(int*)(content1+4)=tmpRB;
		this->WriteBack(DBName,tbl->name,FILE_RECORD ,upnum);
		this->WriteBack(DBName,tbl->name,FILE_RECORD ,downnum);
	}

	this->WriteBack(DBName,tbl->name,FILE_RECORD ,BlockNum);

	return "";

}













//////////////////////execSQL///////////////////
//////////////////////execSQL///////////////////
void exeSQL::exeInsert(catalog& clog,SQLInsert &sqlInsert,Handle& hdl)
{
	string tableName;

	int valueSize;
	int i;
	int fileType=0;
	int valueType;
	int indexValue=0;
	int length;
	vector<TKEY> TKValues;

	tableName=sqlInsert.tblName;
	valueSize=sqlInsert.values.size();
	////////////////////////////////////////////yes or no?
	table *tbl = clog.dbs[clog.curDB].GetTable(tableName);
	Record insert_R8(clog.dbs[clog.curDB].name,tbl,&hdl);////////////////////


	for(i=0;i<valueSize;i++){
		//依次取出value的数据
		valueType=sqlInsert.values[i].dataType;
		string value=sqlInsert.values[i].value;
		length=tbl->attrs[i].len;
		TKEY tmpValue(valueType,length);
		int a;float a1;
		switch(valueType)
		{
		case  T_INT:
			a=std::atoi(value.c_str());
			memcpy(tmpValue.key,&a,tmpValue.length);break;
		case T_FLOAT:
			a1=std::atof(value.c_str());
			memcpy(tmpValue.key,&a1,tmpValue.length);break;
		case  T_CHAR:
			memcpy(tmpValue.key,value.c_str(),tmpValue.length);break;
		}
		TKValues.push_back(tmpValue);
	}
	//调用Insert方法写入buffer

	//myIndex->attrName==

	if(insert_R8.insert_into_block(TKValues,indexValue)!=""){
		//ERROR
		cout<<"INSERT ERROR"<<endl;
	}
	else{
		// 		index* myIndex=tbl->GetIndex();
		// 		if(myIndex!=NULL)
		// 		{
		// 			CBPlusTree T(*myIndex);
		// 			for(int ith=0;ith<tbl->attrs.size();ith++){
		// 				if(myIndex->attrName==tbl->attrs.at(ith).name){
		// 					if(!T.Add(TKValues.at(ith),indexValue)){
		// 						cout<<"INSERT INDEX ERROR"<<endl;
		// 					}
		// 				}
		// 			}		
		// 		}//found the ith column with an index
	}
}


void exeSQL::exeSelect(catalog& clog,SQLSelect &sqlSelect,Handle &hdl)
{

	string tableName;
	//SQLWhere sqlWhere;
	int whereSize;//where条件个数
	int fileName;
	int blockNum;
	int totalBlockCount;
	int listSize;
	int fileType=0;
	int valueType;
	int indexValue=0;
	int length;
	int blockRecordCount;
	string indexName;

	//catalog clog;

	tableName=sqlSelect.tblName;
	whereSize=sqlSelect.wheres.size();

	table *tbl = clog.dbs[clog.curDB].GetTable(tableName);
	Record select_R8(clog.dbs[clog.curDB].name,tbl,&hdl);

	if(whereSize==0){

		vector<vector<TKEY>> TKValues;

		totalBlockCount=select_R8.get_block_count();
		blockNum=select_R8.get_first_block_num();
		for(int i=0;i<totalBlockCount;i++){		
			blockRecordCount=select_R8.get_record_count(blockNum);
			for(int j=0;j<blockRecordCount;j++){
				vector<TKEY> tmp;
				select_R8.get_record(blockNum,j,tmp);
				TKValues.push_back(tmp);
			}
			blockNum=select_R8.get_next_block_num(blockNum);	//.............
		}
		//所有记录已取出。每一条记录为一个vector<TKEY>[i]
		for(int egg=0;egg<TKValues.size();egg++){
			for(int egg2=0;egg2<TKValues[egg].size();egg2++){
				cout<<TKValues[egg][egg2]<<' ';
			}
			cout<<endl;
		}
	}
	else{
		//Select records satisfying the condition(s)
		vector<vector<TKEY>> afterIthSelected;
		//vector的数组。tmpPtr的下标即第i个条件下所遴选出来的record

		//listSize=whereSize;	

		bool hasIndex=false;
		int ithCol=0;
		int jthCol=0;
		index* myIndex=tbl->GetIndex();
		//myIndex->attrName==
		if(myIndex!=NULL)
		{

			for(int ith=0;ith<tbl->attrs.size();ith++){
				if(myIndex->attrName==tbl->attrs.at(ith).name){
					ithCol=ith;
				}
			}		


			for(int ith=0;ith<sqlSelect.wheres.size();ith++){
				if(sqlSelect.wheres.at(ith).key==myIndex->attrName){
					jthCol=ith;
					if(sqlSelect.wheres.at(ith).SignType==SIGN_EQ)
						hasIndex=true;				
				}
			}
		}

		if(!hasIndex){
			totalBlockCount=select_R8.get_block_count();
			blockNum=select_R8.get_first_block_num();
			for(int ii=0;ii<totalBlockCount;ii++){		
				blockRecordCount=select_R8.get_record_count(blockNum );
				for(int j=0;j<blockRecordCount;j++){
					vector<TKEY> tmp;

					select_R8.get_record( blockNum,j,tmp);

					bool satisfied=true;
					for(int k=0;k<whereSize;k++){
						int dataType=sqlSelect.wheres.at(k).dataType;
						string attName=sqlSelect.wheres.at(k).key;

						int signType=sqlSelect.wheres.at(k).SignType;
						string destVal=sqlSelect.wheres.at(k).value;


						valueType=dataType;
						length=destVal.length();
						TKEY destValue(valueType,length);
						int a;float a1;
						switch(valueType)
						{
						case  T_INT:
							a=std::atoi(destVal.c_str());
							memcpy(destValue.key,&a,destValue.length);break;
						case T_FLOAT:
							a1=std::atof(destVal.c_str());
							memcpy(destValue.key,&a1,destValue.length);break;
						case  T_CHAR:
							memcpy(destValue.key,destVal.c_str(),destVal.length());break;
						}

						for(int kk=0;kk<tmp.size();kk++){
							if(tmp.at(kk).keyType==dataType && tbl->attrs.at(kk).name==attName){

								switch(signType){
								case 0:{
									if(tmp.at(kk)==destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 1:{
									if(tmp.at(kk)==destValue)
										satisfied=false;
									else
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
										break;
									   }
								case 2:{
									if(tmp.at(kk)<destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 3:{
									if(tmp.at(kk)>destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 4:{
									if(tmp.at(kk)<=destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 5:{
									if(tmp.at(kk)>=destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								}
							}
						}
					}
					if(satisfied==true){
						afterIthSelected.push_back(tmp);
					}
				}
				blockNum=select_R8.get_next_block_num(blockNum);
			}			
		}
		else{//hasIndex==true
			CBPlusTree T(myIndex);


			valueType=sqlSelect.wheres.at(jthCol).dataType;
			length=sqlSelect.wheres.at(jthCol).value.length();
			string destVal=sqlSelect.wheres.at(jthCol).value;
			TKEY destTK(valueType,length);
			int a;float a1;
			switch(valueType)
			{
			case  T_INT:
				a=std::atoi(destVal.c_str());
				memcpy(destTK.key,&a,destTK.length);break;
			case T_FLOAT:
				a1=std::atof(destVal.c_str());
				memcpy(destTK.key,&a1,destTK.length);break;
			case  T_CHAR:
				memcpy(destTK.key,destVal.c_str(),destVal.length());break;
			}//get the index value for search


			bool isGot=false;//whether get the indexvalue
			int blockNum=0;
			int blockOffset=0;

			int signType=sqlSelect.wheres.at(jthCol).SignType;
			switch(signType){
			case 0:{//'=='
				vector<TKEY> tmp;
				if(!T.GetVal(destTK,indexValue).flag){
					cout<<"B+TREE ERROR"<<endl;
				}//找到第一个index叶节点
				blockNum=indexValue;
				blockOffset=indexValue;					
				blockNum=blockNum>>16;
				blockNum=blockNum&0xffff;
				blockOffset=blockOffset&0xffff;
				if(select_R8.get_record( blockNum,blockOffset,tmp)!=""){
					//select from tree error
					cout<<"SELECT ERROR"<<endl;
				}
				else{
					bool satisfied=true;
					for(int k=0;k<whereSize;k++){

						int valueType=sqlSelect.wheres.at(k).dataType;
						int length=sqlSelect.wheres.at(k).value.length();
						string destVal=sqlSelect.wheres.at(k).value;
						string attName=sqlSelect.wheres.at(k).key;
						TKEY destTK(valueType,length);
						int a;float a1;
						switch(valueType)
						{
						case  T_INT:
							a=std::atoi(destVal.c_str());
							memcpy(destTK.key,&a,destTK.length);break;
						case T_FLOAT:
							a1=std::atof(destVal.c_str());
							memcpy(destTK.key,&a1,destTK.length);break;
						case  T_CHAR:
							memcpy(destTK.key,destVal.c_str(),destVal.length());break;
						}

						for(int kk=0;kk<tmp.size();kk++){
							if(tmp.at(kk).keyType==valueType && tbl->attrs.at(kk).name==attName){
								switch(signType){
								case 0:{
									if(tmp.at(kk)==destTK)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 1:{
									if(tmp.at(kk)==destTK)
										satisfied=false;
									else
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
										break;
									   }
								case 2:{
									if(tmp.at(kk)<destTK)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 3:{
									if(tmp.at(kk)>destTK)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 4:{
									if(tmp.at(kk)<=destTK)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 5:{
									if(tmp.at(kk)>=destTK)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								}
							}
						}
					}
					if(satisfied==true){
						//record selected being deleted
						//afterIthSelected[0].push_back(tmp);
						afterIthSelected.push_back(tmp);
					}
				}
				break;
				   }

			}
		}
		for(int egg=0;egg<afterIthSelected.size();egg++){
			for(int egg2=0;egg2<afterIthSelected[egg].size();egg2++){
				cout<<afterIthSelected[egg][egg2]<<' ';
			}
			cout<<endl;
		}
	}
}

void exeSQL::exeDelete(catalog& clog,SQLDelete &sqlDelete,Handle &hdl)
{//注释大致同上
	string tableName;
	//SQLWhere sqlWhere;
	int whereSize;//where条件个数
	int fileName;
	int blockNum;
	int totalBlockCount;
	int listSize;
	int fileType=0;
	int valueType;
	int indexValue=0;
	int length;
	int blockRecordCount;
	string indexName;

	//catalog clog;

	tableName=sqlDelete.tblName;
	whereSize=sqlDelete.wheres.size();

	table *tbl = clog.dbs[clog.curDB].GetTable(tableName);
	Record delete_R8(clog.dbs[clog.curDB].name,tbl,&hdl);

	if(whereSize==0){

		vector<vector<TKEY>> TKValues;

		totalBlockCount=delete_R8.get_block_count();
		blockNum=delete_R8.get_first_block_num();
		for(int i=0;i<totalBlockCount;i++){		
			blockRecordCount=delete_R8.get_record_count(blockNum );
			for(int j=0;j<blockRecordCount;j++){
				vector<TKEY> tmp;
				delete_R8.get_record(blockNum,j,tmp);
				TKValues.push_back(tmp);
				delete_R8.delete_from_block( blockNum,j);
			}
			blockNum=delete_R8.get_next_block_num(blockNum);
		}

	}
	else{
		//Select records satisfying the condition(s)
		vector<vector<TKEY>> afterIthSelected;
		//vector的数组。tmpPtr的下标即第i个条件下所遴选出来的record

		//listSize=whereSize;	

		bool hasIndex=false;
		int ithCol=0;
		int jthCol=0;
		index* myIndex=tbl->GetIndex();
		//myIndex->attrName==
		// 		if(myIndex!=NULL)
		// 		{
		// 			hasIndex=true;
		// 			
		// 			for(int ith=0;ith<tbl->attrs.size();ith++){
		// 				if(myIndex->attrName==tbl->attrs.at(ith).name){
		// 					ithCol=ith;
		// 				}
		// 			}		
		// 		}//found the ith column with an index


		if(!hasIndex){
			totalBlockCount=delete_R8.get_block_count();
			blockNum=delete_R8.get_first_block_num();
			for(int ii=0;ii<totalBlockCount;ii++){		
				blockRecordCount=delete_R8.get_record_count(blockNum );
				for(int j=0;j<blockRecordCount;j++){
					vector<TKEY> tmp;

					delete_R8.get_record( blockNum,j,tmp);

					bool satisfied=true;
					for(int k=0;k<whereSize;k++){
						int dataType=sqlDelete.wheres.at(k).dataType;
						string attName=sqlDelete.wheres.at(k).key;

						int signType=sqlDelete.wheres.at(k).SignType;
						string destVal=sqlDelete.wheres.at(k).value;


						valueType=dataType;
						length=destVal.length();
						TKEY destValue(valueType,length);
						int a;float a1;
						switch(valueType)
						{
						case  T_INT:
							a=std::atoi(destVal.c_str());
							memcpy(destValue.key,&a,destValue.length);break;
						case T_FLOAT:
							a1=std::atof(destVal.c_str());
							memcpy(destValue.key,&a1,destValue.length);break;
						case  T_CHAR:
							memcpy(destValue.key,destVal.c_str(),destVal.length());break;
						}

						for(int kk=0;kk<tmp.size();kk++){
							if(tmp.at(kk).keyType==dataType && tbl->attrs.at(kk).name==attName){

								switch(signType){
								case 0:{
									if(tmp.at(kk)==destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 1:{
									if(tmp.at(kk)==destValue)
										satisfied=false;
									else
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
										break;
									   }
								case 2:{
									if(tmp.at(kk)<destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 3:{
									if(tmp.at(kk)>destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 4:{
									if(tmp.at(kk)<=destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								case 5:{
									if(tmp.at(kk)>=destValue)
										if(satisfied){satisfied=true;}
										else{satisfied=false;}
									else
										satisfied=false;
									break;
									   }
								}
							}
						}
					}
					if(satisfied==true){
						afterIthSelected.push_back(tmp);
						//delete it
						delete_R8.delete_from_block( blockNum,j);
					}
				}
				blockNum=delete_R8.get_next_block_num(blockNum);
			}
		}

	}
}

CBPlusTree* exeSQL::exeCreateIndex(catalog& ctlg,SQLCreateIndex &sqlcreateindex,Handle &hdl)
{
	database& curDB=ctlg.dbs[ctlg.curDB];
	string error="";
	index index1;
	index1.name=sqlcreateindex.indexName;
	index1.attrName=sqlcreateindex.columnName;

	index1.KeyCount=0;
	index1.LeafHead=-1;
	index1.Level=-1;
	index1.NodeCount=0;
	index1.Root=-1;
	index1.Rubbish=-1;

	table *t1=ctlg.dbs[ctlg.curDB].GetTable(sqlcreateindex.tblName);	//根据表名找到表
	attribute *attr1=t1->GetAttr(sqlcreateindex.columnName);//根据属性名找到属性
	index1.KeyLen=attr1->len;	//

	index1.Rank=(4*1024-12)/(4+index1.KeyLen)/2-1;
	index1.KeyType=attr1->dataType;
	t1->indexs.push_back(index1);

	string Path="DBs\\"+ctlg.dbs[ctlg.curDB].name+"\\"+index1.name+".index";
	ofstream ofs(Path,ios::binary);
	ofs.close();
	/////有关于recordmanager和indexmanager的东东

	CBPlusTree tree(&t1->indexs[0]);

	int totalBlockCount=0;
	int blockNum;
	int blockRecordCount;

	Record select_R8(curDB.name,t1,&hdl);

	int columnIndex=t1->GetColumnIndex(attr1->name);
	totalBlockCount=select_R8.get_block_count();
	blockNum=select_R8.get_first_block_num();
	for(int i=0;i<totalBlockCount;i++){		
		blockRecordCount=select_R8.get_record_count(blockNum);
		for(int j=0;j<blockRecordCount;j++){
			vector<TKEY> tmp;
			select_R8.get_record(blockNum,j,tmp);
			int value=(blockNum<<16)|j;
			tree.Add(tmp[columnIndex],value);	///////////////////////
		}
		blockNum=select_R8.get_next_block_num(blockNum);
	}
	/*	tree.Print();*/
	return NULL;
}

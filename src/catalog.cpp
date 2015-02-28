#include "catalog.h"
#include "SQLStatement.h"
#include <fstream>
#include <direct.h>

bool database::loadData()
{
	ifstream ifs(name.c_str(),ios::binary);
	boost::archive::binary_iarchive iar( ifs );
	iar>>*(this);
	
	
	
	/*
	int numTable;
	
	

	ifs>>numTable;

	//表数 表名 属性数 记录长度 属性名 数据类型 数据长度 属性类型 索引名 

	for(int i=0;i<numTable;i++)
	{
		table curTable;
		
		ifs>>curTable.name>>curTable.attrNum>>curTable.recordLen;

		for(int j=0;j<curTable.attrNum;j++)
		{
			attribute tempAttr;
			ifs>>tempAttr.name>>tempAttr.dataType>>tempAttr.len>>tempAttr.attrType>>tempAttr.indexName;
			curTable.attrs.push_back(tempAttr);
		}
		tbls.push_back(curTable);

	}
	*/
	ifs.close();
	return true;
}
bool database::updateData()
{
	ofstream ofs(name.c_str(),ios::binary);
	boost::archive::binary_oarchive oar( ofs );
	oar<<*(this);
	/*
	ofs<<tbls.size()<<" ";
	for(int i=0;i<tbls.size();i++)
	{
		table &curTable=tbls[i];
		ofs<<curTable.name<<" "<<curTable.attrNum<<" "<<curTable.recordLen<<" ";
		for(int j=0;j<curTable.attrs.size();j++)
		{
			attribute curAttr=curTable.attrs[j];
			ofs<<curAttr.name<<" "<<curAttr.dataType<<" "<<curAttr.len<<" "<<curAttr.attrType<<" "<<curAttr.indexName<<" ";
		}
	}
	*/
	ofs.flush();
	ofs.close();
	return true;
}

table* database::GetTable(string tableName)
{
	for(int i=0;i<tbls.size();i++)
		if(tbls[i].name==tableName)
			return &tbls[i];
	return NULL;
}

/////////////////////////////验证语句////////////////////////////////////////////
string database::verify_insert(SQLInsert&sqlinsert)
{
	string error="";
	//根据表的名字，得到该表
	table *tbl;
	
	if((tbl=GetTable(sqlinsert.tblName))==NULL)
	{
		error="no_such_table:";
		error+=sqlinsert.tblName;
		return error;
	}
	//判断表的属性数目和SQL的Value数目是不是相等
	if(sqlinsert.values.size()!=tbl->attrs.size())
	{
		error="number_of_values_not_match";
		return error;
	}
	//判断每个SQL的value的类型是否和表的属性的类型一样
	for(int i=0;i<tbl->attrs.size();i++)
	{
		
		attribute &attr=tbl->attrs[i];
		SQLValue sqlvalue=sqlinsert.values[i];
		if(sqlvalue.dataType!=attr.dataType)
		{
			error="data_type_error:";
			error+=attr.name;
			error+="'s type should be ";
			error+=attr.dataType;
			error+=" rather than "+sqlvalue.dataType;
			return error;
		}
	}
	
	return error;
}
string database::verify_select(SQLSelect&sqlselect)
{
	string error="";
	//根据表的名字，得到该表
	table *tbl=GetTable(sqlselect.tblName);
	if(tbl==NULL)
	{
		error="no_such_table";
		error+=sqlselect.tblName;
		return error;
	}
	//对每个传入的where的条件进行验证，看他们类型是不是一样
	for(int i=0;i<sqlselect.wheres.size();i++)
	{
		SQLWhere where=sqlselect.wheres[i];
		//根据属性的名字，得到该属性
		attribute *attr=tbl->GetAttr(where.key);
		if (attr==NULL)
		{
			error="no_such_attribute:";
			error+=where.key;
			return error;
		}
		//验证类型是否一致 
		if(attr->dataType!=where.dataType)
		{
			error="type_of_where_not_match:";
			error+=where.key;
			return error;
		}
		
	}
	return error;
}
string database::verify_create_table(SQLCreateTable&sqlcreatetable)
{
	string error="";
	//根据表名在数据库查找表，验证是否存在
	table *tbl=GetTable(sqlcreatetable.tblName);
	if(tbl!=NULL)
	{
		error="table_already_exists:";
		error+=tbl->name;
		return error;
	}
	
	return error;
}
string database::verify_create_database(SQLCreateDatabase&sqlcreatedatabase)
{
	string error="";
	
	return error;
}
string database::verify_create_index(SQLCreateIndex&sqlcreateindex)
{
	string error="";
	for(int i=0;i<tbls.size();i++)
	{
		table &t1=tbls[i];
		for(int j=0;j<t1.indexs.size();j++)
		{
			index &index1=t1.indexs[j];
			if(index1.name==sqlcreateindex.indexName)
			{
				
				error="duplicated_index_name";
				return error;
			}
		}
	}
	return error;
}
string database::verify_help(SQLHelp&sqlhelp)
{
	string error="";
	return error;
}
string database::verify_delete(SQLDelete&sqldelete)
{
	string error="";
	table *tbl=GetTable(sqldelete.tblName);
	if(tbl==NULL)
	{
		error="no_such_table";
		error+=sqldelete.tblName;
		return error;
	}

	//对每个传入的where的条件进行验证，看他们类型是不是一样
	for(int i=0;i<sqldelete.wheres.size();i++)
	{
		SQLWhere where=sqldelete.wheres[i];
		//根据属性的名字，得到该属性
		attribute *attr=tbl->GetAttr(where.key);
		if (attr==NULL)
		{
			error="no_such_attribute:";
			error+=where.key;
			return error;
		}
		//验证类型是否一致 
		if(attr->dataType!=where.dataType)
		{
			error="type_of_where_not_match:";
			error+=where.key;
			return error;
		}
		
	}
	return error;
}
string database::verify_use(SQLUse&sqluse)
{
	string error="";
// 	ifstream ifs;
// 	string fileName=sqluse.dbName+".ctlg";
// 	ifs.open(fileName.c_str());
// 	if(ifs==NULL)
// 	{
// 		error="database_not_exists:";
// 		error+=sqluse.dbName;
// 		ifs.close();
// 		return error;
// 	}
	return error;
}
string database::verify_execfile(SQLExecFile&sqlexecfile)
{
	string error="";
	ifstream ifs(sqlexecfile.fileName.c_str());
	if(ifs==NULL)
	{
		error="file_not_exist";
		error+=sqlexecfile.fileName;
		ifs.close();
		return error;
	}
	return error;
}
string database::verify_drop_index(SQLDropIndex&sqldropindex)
{
	string error="";
// 	ifstream ifs(sqldropindex.indexName.c_str());
// 	if(ifs==NULL)
// 	{
// 		error="index_not_exist";
// 		error+=sqldropindex.indexName;
// 		ifs.close();
// 		return error;
// 	}
	return error;
}
string database::verify_drop_database(SQLDropDatabase&sqldrop)
{
	string error="";
// 	ifstream ifs(sqldrop.dbName.c_str());
// 	if(ifs==NULL)
// 	{
// 		error="database_not_exist";
// 		error+=sqldrop.dbName;
// 		ifs.close();
// 		return error;
// 	}
	return error;
}
string database::verify_drop_table(SQLDropTable&sqldroptable)
{
	string error="";
	table*tbl=GetTable(sqldroptable.tblName);
	if(tbl==NULL)
	{
		error="table_not_exist";
		error+=sqldroptable.tblName;
		return error;
	}
	return error;
}
string database::verify_quit(SQLQuit&sqlquit)
{
	string error="";
	return error;
}
//////////////////////////end of 验证语句////////////////////////////////////////////////

//创建和删除语句
string database::create_table(SQLCreateTable&sqlcreatetable)
{
	string error="";
	table tbl;
	tbl.name=sqlcreatetable.tblName;
	int recordLen=0;
	for(int i=0;i<sqlcreatetable.attrs.size();i++)
	{
		tbl.attrs.push_back(sqlcreatetable.attrs[i]);
		recordLen+=sqlcreatetable.attrs[i].len;
//		recordLen++;
	}
	tbl.attrNum=tbl.attrs.size();
	tbl.recordLen=recordLen;
	tbls.push_back(tbl);
	string Path="DBs\\"+name+"\\"+tbl.name+".record";
	ofstream ofs(Path,ios::binary);
	ofs.close();
	return error;
}
string database::create_index(SQLCreateIndex&sqlcreateindex)
{
	string error="";
	index index1;
	index1.name=sqlcreateindex.indexName;
	index1.attrName=sqlcreateindex.columnName;
	index1.Rank=100;
	index1.KeyCount=0;
	index1.LeafHead=-1;
	index1.Level=-1;
	index1.NodeCount=0;
	index1.Root=-1;
	index1.Rubbish=-1;
	
	table *t1=GetTable(sqlcreateindex.tblName);	//根据表名找到表
	attribute *attr1=t1->GetAttr(sqlcreateindex.columnName);//根据属性名找到属性
	index1.KeyLen=attr1->len;
	index1.KeyType=attr1->dataType;
	t1->indexs.push_back(index1);

	string Path="DBs\\"+name+"\\"+index1.name+".index";
	ofstream ofs(Path,ios::binary);
	ofs.close();
	/////有关于recordmanager和indexmanager的东东

	return error;
}


string database::drop_table(SQLDropTable&sqldroptable)
{
	string error="";
	
	string Path="DBs\\"+name+"\\"+sqldroptable.tblName+".record";
	string deleteCMD="erase "+Path;
	if(system(deleteCMD.c_str())==-1){error="drop_table_failed";return error;}

	tbls.erase(GetTableR(sqldroptable.tblName));
	return error;
}
string database::drop_index(SQLDropIndex&sqldropindex)
{
	string error="";
	
	string Path="DBs\\"+name+"\\"+sqldropindex.indexName+".index";
	string deleteCMD="erase "+Path;
	if(system(deleteCMD.c_str())==-1){error="drop_index_failed";return error;}
	for(int i=0;i<tbls.size();i++)
	{
		for(int j=0;j<tbls[i].indexs.size();j++)
		{
			if(tbls[i].indexs[j].name==sqldropindex.indexName)
			{/*tbls[i].indexs.erase(tbls[i].GetIndexR(sqldropindex.indexName));*/
					tbls[i].indexs.clear();
				return error;}
		}
	}
	error="index_not_exist";
	return error;
}

vector<table>::iterator database::GetTableR(string tableName)
{
	vector<table>::iterator i;
	for(i=tbls.begin();i!=tbls.end();i++)
	{
		if(i->name==tableName)
			return i;
	}
}

attribute*table::GetAttr(string attr)
{
	for(int i=0;i<attrs.size();i++)
		if(attrs[i].name==attr)
			return &attrs[i];
	return NULL;
}

index* table::GetIndex()
{
	if(indexs.size()==0) return NULL;

	return &indexs[0];
}

int table::GetColumnIndex(string name)
{
	for (int i=0;i<attrs.size();i++)
	{
		if(attrs[i].name==name)return i;
	}
	return -1;
}

vector<index>::iterator table::GetIndexR(string indexName)
{
	vector<index>::iterator i;
	for(i=indexs.begin();i!=indexs.end();i++)
	{
		if(i->name==indexName)
			return i;
	}
}

///////////catalog////////////////////////////////
catalog::catalog():PATH("DBs\\")
{
	curDB=-1;
	string fileName=PATH+"catalog.dbs";
	
	
	
	try{
		ifstream ifs(fileName,ios::binary);
		boost::archive::binary_iarchive iar( ifs );
		iar>>*this;
		ifs.close();
	}
	catch(exception& e){
		;
	}
	

}
catalog::~catalog()
{
	string fileName=PATH+"Catalog.dbs";
	ofstream ofs(fileName,ios::binary);
	boost::archive::binary_oarchive oar( ofs );
	try{
		oar<<(*this);
	}
	catch(exception& e)
	{;}
	ofs.close();
}
string catalog::create_database(SQLCreateDatabase&sqlcreatedatabase)
{
	string error="";
	dbs.push_back(database(sqlcreatedatabase.dbName));
	int i=mkdir((PATH+sqlcreatedatabase.dbName).c_str());
	if(i==-1)
	{
		error="create_db_failed";return error;
	}
	return error;
}
string catalog::drop_database(SQLDropDatabase&sqldropdatabase)
{
	string error="";
	
	string deleteCMD="rd "+PATH+sqldropdatabase.dbName+" /s";
	if(system(deleteCMD.c_str())==-1){error="drop_db_failed";return error;}	
		for(int i=0;i<dbs.size();i++)
		{
			if(dbs[i].name==sqldropdatabase.dbName)
			{
				for(int j=i;j<dbs.size()-1;j++)
				{
					dbs[j]=dbs[j+1];
				}
				dbs.pop_back();
			}
		}
	
	return error;
}
string catalog::use_database(SQLUse &sqluse)
{
	string error="";
	string fileName=PATH+"Catalog.dbs";
	ofstream ofs(fileName,ios::binary);
	boost::archive::binary_oarchive oar( ofs );
	try{
		oar<<(*this);
	}
	catch(exception& e)
	{;}
	ofs.close();

	if(sqluse.dbName!=dbs[curDB].name)
	{
		for(int i=0;i<dbs.size();i++)
		{
			if(sqluse.dbName==dbs[i].name)
			{
				curDB=i;return error;
			}
		}
	}
	error="no_such_database";
	return error;
}
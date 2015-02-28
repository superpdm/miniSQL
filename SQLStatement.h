

#ifndef _SQL_STATEMENT
#define _SQL_STATEMENT
#include <vector>
#include <string>
#include "constant.h"
using namespace std;
class attribute;
class TKEY;
class TKEY
{
public:
	int keyType;

	char *key;
	int length;
#pragma region constructors

	// 	TKEY()
	// 	{
	// 		keyType=-1;
	// 		length=0;
	// 		key=NULL;
	// 	}
	TKEY(int keytype,int length)
	{
		keyType=keytype;
		if(keytype==T_CHAR)
			this->length=length;
		else
			this->length=4;
		key=new char[this->length];
	}
	TKEY(const TKEY& t1)
	{
		this->keyType=t1.keyType;
		this->length=t1.length;
		this->key=new char[length];
		memcpy(this->key,t1.key,length);
	}

#pragma endregion constructors
#pragma region OperatorOverLoad

	friend ostream & operator<<(ostream& out,const TKEY& object);

	bool operator<(const TKEY t1)
	{
		switch(t1.keyType)
		{
		case  0:
			return *(int*)key<*(int*)t1.key;
		case  1:
			return *(float*)key<*(float*)t1.key;
		case  2:
			return (strnicmp(key,t1.key,length)<0);
		}

	}
	bool operator>(const TKEY t1)
	{
		switch(t1.keyType)
		{
		case  0:
			return *(int*)key>*(int*)t1.key;
		case  1:
			return *(float*)key>*(float*)t1.key;
		case  2:
			return (strnicmp(key,t1.key,length)>0);
		}
	}
	bool operator<=(const TKEY t1)
	{
		return !(operator>(t1));
	}
	bool operator>=(const TKEY t1)
	{
		return !(operator<(t1));
	}
	bool operator==(const TKEY t1)
	{
		switch(t1.keyType)
		{
		case  0:
			return *(int*)key==*(int*)t1.key;
		case  1:
			return *(float*)key==*(float*)t1.key;
		case  2:
			return (strnicmp(key,t1.key,length)==0);
		}
	}
	void operator=(TKEY k) // ��ֵ�����غ���
	{
		keyType=k.keyType;
		length=k.length;
		memcpy(key,k.key,length);
	}

#pragma endregion OperatorOverLoad


	~TKEY()
	{
		if(key!=NULL)
			delete []key;
	}
};

//////////////////////�����////////////////
//SQL����//
class SQL{
public:
	int SQLType;
	SQL(){}
	SQL(int sqltype)
	{
		this->SQLType=sqltype;
	}
};
//where���������
class SQLWhere{
public:
	int dataType;
	string key;
	int SignType;
	string value;
	
};
// insertʱ���value��
class SQLValue{
public:
	int dataType;
	string value;
};
//insert��
class SQLInsert:public SQL
{
public:
	string tblName;
	vector<SQLValue> values;	//����vector��ÿ��string�ĵ�һ����ǰ׺��������ʾ���͡�'0'->int,'1'->char,'2'->float
};
//select��
class SQLSelect:public SQL
{
public:
	string tblName;
	vector<SQLWhere> wheres;
};

//createtable��
class SQLCreateTable:public SQL
{
public:
	SQLCreateTable()
	{
		
	}
	string tblName;
	vector<attribute> attrs;
};
//createdatabase��
class SQLCreateDatabase:public SQL
{public:
	SQLCreateDatabase(){}

	string dbName;
};
//createindex��
class SQLCreateIndex:public SQL
{
public:
	string indexName;
	string tblName;
	string columnName;
};
//help��
class SQLHelp:public SQL
{
public:
	string help;
};
//delete��
class SQLDelete:public SQL
{
public:
	string tblName;
	vector<SQLWhere> wheres;
};
//use��
class SQLUse:public SQL
{
public:
	string dbName;
};
//execfile��
class SQLExecFile:public SQL
{
public:
	string fileName;
};
//dropindex��
class SQLDropIndex:public SQL
{
public:
	string indexName;
};
//dropdatabase��
class SQLDropDatabase:public SQL
{
public:
	string dbName;
};
//droptable��
class SQLDropTable:public SQL
{
public:
	string tblName;
};
//quit��
class SQLQuit:public SQL
{
	
};


#endif
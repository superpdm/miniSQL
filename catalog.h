#ifndef _CATALOG_H
#define _CATALOG_H
#pragma warning(disable:4786)
#pragma warning(disable:4251)
#pragma warning(disable:4018)
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <cstring>
#include <string>

#include "SQLStatement.h"
#include "constant.h"
using namespace std;






class table;
class attribute;
class database;
class index;

class catalog{
public:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize( Archive& ar, const unsigned int version )
	{
		ar & dbs;
	};
	const string PATH;
	vector<database> dbs;
	int curDB;
	catalog();
	string create_database(SQLCreateDatabase&sqlcreatedatabase);
	string drop_database(SQLDropDatabase&);
	string use_database(SQLUse &);
	~catalog();
};
/////////////////���ݿ���////////////////////
class database{
	

public:
	friend class boost::serialization::access;

	template <class Archive>

	void serialize( Archive& ar, const unsigned int version )
	{
		ar & name;
		ar & tbls;
	};

	vector<table> tbls;
	string name;
public:
	database(){}
	database(string name){
		this->name=name;
	}
	bool loadData();
	bool updateData();
	table* GetTable(string);
	vector<table>::iterator GetTableR(string);
	//��֤���
	string verify_insert(SQLInsert&);
	string verify_select(SQLSelect&);
	string verify_create_table(SQLCreateTable&);
	string verify_create_database(SQLCreateDatabase&);
	string verify_create_index(SQLCreateIndex&);
	string verify_help(SQLHelp&);
	string verify_delete(SQLDelete&);
	string verify_use(SQLUse&);
	string verify_execfile(SQLExecFile&);
	string verify_drop_index(SQLDropIndex&);
	string verify_drop_database(SQLDropDatabase&);
	string verify_drop_table(SQLDropTable&);
	string verify_quit(SQLQuit&);

	//������ɾ�����

	string create_table(SQLCreateTable&);
	string create_index(SQLCreateIndex&);
	

	string drop_table(SQLDropTable&);
	string drop_index(SQLDropIndex&);
	
	//
	
};
//////////////////////////����////////////////
class table
{
public:
	friend class boost::serialization::access;

	template <class Archive>

	void serialize( Archive& ar, const unsigned int version )
	{
		ar & name;
		ar & attrNum;
		ar & recordLen;
		ar & attrs;
		ar & indexs;	
		ar & FirstBlockNum;
		ar & FirstRubbishNum;
		ar & BlockCount;
	};
	string name;
	int attrNum;
	int recordLen;

	int FirstBlockNum;
	int FirstRubbishNum;
	int BlockCount;

	vector<attribute> attrs; 
	vector<index> indexs;
public:
	attribute* GetAttr(string attr);
	index* GetIndex();
	vector<index>::iterator GetIndexR(string indexName);
	int GetColumnIndex(string name);
	table()
	{
		this->FirstBlockNum=-1;
		this->FirstRubbishNum=-1;
		this->BlockCount=0;
	}
	table(string name,int attrNum,int recordLen)
	{
		this->name=name;
		this->attrNum=attrNum;
		this->recordLen=recordLen;
		this->FirstBlockNum=-1;
		this->FirstBlockNum=-1;
		this->BlockCount=0;

	}
};
////////////////////////������///////////////
class attribute{
public:
	friend class boost::serialization::access;

	template <class Archive>

	void serialize( Archive& ar, const unsigned int version )
	{
		ar & name;
		ar & dataType;
		ar & len;
		ar & attrType;
	};
	string name;
	int dataType;
	int len;
	int attrType;
};

class index
{
public:
	friend class boost::serialization::access;

	template <class Archive>

	void serialize( Archive& ar, const unsigned int version )
	{
		ar & MaxCount;
		ar & attrName;
		ar & name;
		ar & KeyLen;
		ar & KeyType;
		ar & Rank;
		ar & Rubbish;
		ar & Root;
		ar & LeafHead; //Ҷ��ָ�����
		ar & KeyCount; //key����
		ar & Level; //����
		ar & NodeCount; //�ڵ���
	};
	index(int keytype,int keylen,int rank)
	{
		KeyCount=0;
		Level=(0);
		NodeCount=(0);
		Root=(-1);
		LeafHead=-1;
		KeyType=keytype;
		KeyLen=keylen;
		Rank=rank;
		Rubbish=-1;
		MaxCount=0;
	}
	index(){

		KeyCount=0;
		Level=(0);
		NodeCount=(0);
		Root=(-1);
		LeafHead=-1;
		Rubbish=-1;
		MaxCount=0;
	}
	int MaxCount;
	int KeyLen;
	//0 for int,1 for float ,2 for char(n)
	int KeyType;
	int Rank;		//degree of CBPlusTree
	int Rubbish;	//������������
	int Root;	//��Root�����
	int LeafHead; //Ҷ��ָ�����
	int KeyCount; //key����
	int Level; //����
	int NodeCount; //�ڵ���
	string attrName;//��Ӧ���е�����
	string name;	//���Ե�����
};
#endif
